<?php

/**
 * Alert the organizer when the assignment has changed
 */

function grievance_alert_assignment($grievance_node) {
	// Component must be enabled.
	if (!sirius_domain_component_enabled('grievance:alert:assignment')) {
		return array('success' => TRUE, 'msg' => 'Component grievance:alert:assignment is not enabled.');
	}

	$grievance_json = sirius_json_get($grievance_node);

	$alert_uids = array_filter(array_unique(array_merge(
		sirius_fieldvals($grievance_node, 'field_grievance_rep_assignee', 'target_id'),
		sirius_fieldvals($grievance_node, 'field_grievance_rep_organizer', 'target_id')
	)));
	$intake_uids = sirius_fieldvals($grievance_node, 'field_grievance_rep_filed', 'target_id');

	$need_save = FALSE;

	global $base_url;
	$url = "$base_url/node/" . $grievance_node->nid;

	$params = array();
	$params['subject'] = "Grievance assignment: $grievance_node->title";

	$params['sms'] = "You have been assigned a grievance\n\n";
	$params['sms'] .= "Grievance: $grievance_node->title\n\n";
	$params['sms'] .= "Link: $url";

	$params['body'] = "You have been assigned a grievance.<br />";
	$params['body'] .= "Grievance: $grievance_node->title<br />";
	$params['body'] .= "Link: <a href=\"$url\">$url</a>";

	$params['handler_nids'] = array($grievance_node->nid);

	$count_email = $count_sms = $count_notifications = 0;
	foreach ($alert_uids as $alert_uid) {
		// Don't send to intakes
		if (in_array($alert_uid, $intake_uids)) { continue; }

		// Don't send if it's already been sent.
		if ($grievance_json['grievance']['alert']['assignment'][$alert_uid]) { continue; }

		++$count_notifications;

		$user_full = user_load($alert_uid);
		$user_json = json_decode($user_full->field_sirius_json['und'][0]['value'], TRUE);
		$medium = $user_json['sirius']['grievance']['alert']['prefs']['medium'];
		if (!$medium) { $medium = '[default: email]'; }

		$msg = "Alerting user " . $user_full->name . " (" . $user_full->uid . ") by preferred medium $medium.";

		if ($medium == 'email' || $medium == '[default: email]' || $medium == 'both') {
			$params['to'] = $user_full->mail;
			if ($params['to']) {
				$email_result = sirius_email_send($params);
				++$count_email;
			}
		}

		if ($medium == 'sms' || $medium == 'both') {
			$params['to'] = $user_full->field_grievance_phone['und'][0]['value'];
			if ($params['to']) { 
				$sms_result = sirius_sms_send($params);
				++$count_sms;
			}
		}
	
		$grievance_json['grievance']['alert']['assignment'][$alert_uid] = array(
			'alerted' => TRUE,
			'ts' => time(),
			'medium' => $medium,
			'sms_result' => $sms_result,
			'email_result' => $email_result,
		);
		$need_save = TRUE;
	}

	if ($need_save) {
		sirius_json_set($grievance_node, $grievance_json);
		node_save($grievance_node);
	}

	return array('success' => TRUE, 'msg' => "Scanned $count_notifications users; sent $count_email email and $count_sms SMS.");
}

/**
 * Iterate through all grievances, and send alerts to any that have a due date in the future.
 */

function grievance_alert_deadline_all($mode = 'test') {
	if (variable_get('grievance_alert_disable', NULL) == 'Yes') { 
		return array('success' => TRUE, 'count' => 0, 'msg' => 'Grievance deadline alerts have been disabled');
	}

	sirius_minilog('Starting alert generation.');

	$query = new EntityFieldQuery();
	$query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'grievance');
  $query->fieldCondition('field_grievance_alert_date', 'value', date('Y-m-d'), '>=');
	$stmt = $query->execute();
	if (!$stmt['node']) {
		sirius_minilog('No grievances have alert dates in the future.');
		return array('success' => TRUE, 'count' => 0, 'msg' => 'No grievances have deadline alert dates in the future.');
	}

	$count = 0;
	$nids = array_keys($stmt['node']);
	if ($nids) {
		foreach ($nids as $nid) {
			++$count;
			$node = node_load($nid);
			grievance_alert_deadline($mode, $node);
		}
	}

	sirius_minilog('Alert generation complete.');

	return array('success' => TRUE, 'msg' => "Scanned $count nodes", 'count' => $count);
}

/**
 * Send email alerts to appropriate users based on the due date and status of the grievance.
 *
 * @param node The grievance node to examine.
 */

function grievance_alert_deadline($mode, $node) {
		sirius_minilog('Processing alerts for: ' . $node->title);

	// Load previous alert data for this node.  We should only send one alert per person per status
	$node_json = sirius_json_get($node);
	$alert_data = $node_json['grievance']['alert']['deadline'];
	if (!$alert_data) { $alert_data = array(); }

	// Get the deadline and alert term
	$deadline = $node->field_grievance_alert_date['und'][0]['value'];
	$alert = '';
	$alert_tid = $node->field_grievance_alert_tid['und'][0]['tid'];
	$alert_term = taxonomy_term_load($alert_tid);
	$alert = $alert_term->name;
	if (!$alert) { $alert = $node->field_grievance_alert['und'][0]['value']; }

	$need_save = FALSE;

	// We send up to five alerts.  For each possible deliver ...
	for ($i=1; $i<=5; ++$i) {
		// Have we already sent this alert?
		if ($alert_data[$alert][$deadline][$i]) { 
			sirius_minilog("Alert #$i for $alert: Already sent.");
			continue;
		}

		// How many days ahead should we warn?
		$days = variable_get('grievance_alert_days_warning' . $i, -999);
		if ($days == -999 || $days < 0) {
			sirius_minilog("Alert #$i for $alert: Alert #$i not configured.");
			continue;
		}

		// Is it due yet?
		if (strtotime("+$days days") < strtotime($deadline)) { 
			sirius_minilog("Alert #$i for $alert: Deadline hasn't arrived.");
			continue;
		}

		// Send the alert
		$alert_data[$alert][$deadline][$i] = "SENT";
		$need_save = TRUE;

		$uids = grievance_alert_list_recipients($node);

		global $base_url;
		$url = "$base_url/node/" . $node->nid;

		$params = array();
		$params['subject'] = "Grievance Deadline Alert #$i: Only $days days remain to $alert ($node->title)";

		$params['sms'] = "Grievance Deadline Alert #$i: Only $days days remain to $alert\n\n";
		$params['sms'] .= "Alert #: $i\n\n";
		$params['sms'] .= "Grievance: $node->title\n\n";
		$params['sms'] .= "Deadline: $deadline\n\n";
		$params['sms'] .= "Union Must: $alert\n\n";
		$params['sms'] .= "Link: $url";

		$params['body'] = "Alert #: $i<br />";
		$params['body'] .= "Grievance: $node->title<br />";
		$params['body'] .= "Deadline: $deadline<br />";
		$params['body'] .= "Union Must: $alert<br />";
		$params['body'] .= "Link: <a href=\"$url\">$url</a>";

		$params['handler_nids'] = array($node->nid);

		$params['to'] = $to;

		foreach ($uids as $uid) {
			$user_full = user_load($uid);
			$user_json = json_decode($user_full->field_sirius_json['und'][0]['value'], TRUE);
			$medium = $user_json['sirius']['grievance']['alert']['prefs']['medium'];
			if (!$medium) { $medium = '[default: email]'; }

			$log_msg = "Alert #$i for $alert to " . $user_full->name . ", medium = $medium";

			if ($medium == 'none') {
				sirius_minilog("$log_msg: No alert sent.");
				continue; 
			}

			if ($medium == 'email' || $medium == '[default: email]' || $medium == 'both') {
				$params['to'] = $user_full->mail;
				if (!$params['to']) { sirius_minilog("$log_msg: No email address found.", 'warning');  continue;  }
				
				if ($mode == 'live') {
					$result = sirius_email_send($params);
					if ($result['success']) {
						sirius_minilog("$log_msg: Sent to $params[to].");
					} else {
						sirius_minilog("$log_msg: Failed to send to $params[to]: " . $result['msg'], 'warning');
					}
				} else {
					sirius_minilog("$log_msg: (Skipping sending to $params[to] because mode = $mode)");
				}
			}

			if ($medium == 'sms' || $medium == 'both') {
				$params['to'] = $user_full->field_grievance_phone['und'][0]['value'];
				if (!$params['to']) { sirius_minilog("$log_msg: No phone number found.", 'warning');  continue;  }

				if ($mode == 'live') {
					$result = sirius_sms_send($params);
					if ($result['success']) {
						sirius_minilog("$log_msg: Sent to $params[to].");
					} else {
						sirius_minilog("$log_msg: Failed to send to $params[to]: " . $result['msg'], 'warning');
					}
				} else {
					sirius_minilog("$log_msg: (Skipping sending to $params[to] because mode = $mode)");
				}
			}
		}
	}

	if ($mode == 'live' && $need_save) {
		$node_json['grievance']['alert']['deadline'] = $alert_data;
		sirius_json_set($node, $node_json);
		$node->grievance_no_recurse = TRUE;
		node_save($node);
		sirius_minilog('Alerts sent for: ' . $node->title);
	}

	return array('success' => TRUE);
}

/**
 * List the possible recipients for an alert.
 *
 * @param node The grievance node to examine.
 * @return string[] A unique array of email addresses who should receive the alert.
 */

function grievance_alert_list_recipients($node) {
	$fields = array();
	$tmp = variable_get('grievance_alert_recipients', NULL);
	if (is_null($tmp)) {
		$fields = array(
			'field_grievance_rep_filed',
			'field_grievance_rep_organizer',
			'field_grievance_rep_lead',
			'field_grievance_rep_manager',
			'field_grievance_rep_watching'
		);
	} else {
		foreach ($tmp as $key => $value) {
			if ($value) {
				$fields[] = $value;
			}
		}
	}

	$result = array();
	foreach ($fields as $field) {
		if (isset($node->{$field}['und'])) {
			foreach ($node->{$field}['und'] as $tmp) {
				$uid = $tmp['target_id'];
				if (!$uid) { continue; }
				$result[] = $uid;
			}
		}
	}

	$result = array_unique($result);

	return $result;
}