<?php

function grievance_page_alert_test() {
	return drupal_get_form('grievance_page_alert_test_form');
}

function grievance_page_alert_test_form($form, &$form_state) {
	$form['action'] = array(
		'#type' => 'select',
		'#title' => t('Action'),
		'#options' => array(
			'all' => t('Run the standard alert cron job'),
			'single' => t('Execute the alerts for a specific grievance'),
			'reset' => t('Reset the alerts for a specific grievance'),
		),
	);

	$form['grievance_nid'] = array(
		'#type' => 'textfield',
		'#title' => t('Grievance NID'),
		'#size' => 8,
		'#states' => array(
			'visible' => array(
				array(":input[name=\"action\"]" => array('value' => 'single')), 
				'or',
				array(":input[name=\"action\"]" => array('value' => 'reset')),
			),
		),
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

  $log = sirius_minilog_render_html();
  if ($log) { $form['results'] = array('#markup' => '<hr>' . $log); }

	return $form;
}

function grievance_page_alert_test_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;

	$action = $form_state['values']['action'];
	if ($action == 'all') {
		grievance_alert_all();
	} else {
		$node = node_load($form_state['values']['grievance_nid']);
		if (!$node) { drupal_set_message("Not a node.", 'error'); return; }
		if ($node->type != 'grievance') { drupal_set_message("Not a grievance node.", 'error'); return; }
		if ($node->field_sirius_domain['und'][0]['target_id'] != sirius_domain_nid()) { drupal_set_message("Not in your domain.", 'error'); return; }

		if ($action == 'reset') {
			unset($node->field_grievance_data_alert['und']);
			node_save($node);
			drupal_set_message("Alert data reset for " . $node->title);
		}

		if ($action == 'single') {
			grievance_alert($node);
		}
	}
}

/**
 * Iterate through all grievances, and send alerts to any that have a due date in the future.
 */

function grievance_alert_all() {
	if (variable_get('grievance_alert_disable', NULL) == 'Yes') { return; }

	sirius_minilog('Starting alert generation.');

	$query = new EntityFieldQuery();
	$query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'grievance');
  $query->fieldCondition('field_grievance_alert_date', 'value', date('Y-m-d'), '>=');
	$stmt = $query->execute();
	if (!$stmt['node']) {
		sirius_minilog('No grievances have alert dates in the future.');
		return;
	}

	$nids = array_keys($stmt['node']);
	if ($nids) {
		foreach ($nids as $nid) {
			$node = node_load($nid);
			grievance_alert($node);
		}
	}

	sirius_minilog('Alert generation complete.');

	return;
}

/**
 * Send email alerts to appropriate users based on the due date and status of the grievance.
 *
 * @param node The grievance node to examine.
 */

function grievance_alert($node) {
		sirius_minilog('Processing alerts for: ' . $node->title);

	// Load previous alert data for this node.  We should only send one alert per person per status
	$alert_data_serialized = $node->field_grievance_data_alert['und'][0]['value'];
	if ($alert_data_serialized) {
		$alert_data = unserialize($alert_data_serialized);
	} else {
		$alert_data = array();
	}

	// Get the deadline and alert term
	$deadline = $node->field_grievance_alert_date['und'][0]['value'];
	$alert = '';
	$alert_tid = $node->field_grievance_alert_tid['und'][0]['tid'];
	$alert_term = taxonomy_term_load($alert_tid);
	$alert = $alert_term->name;

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
				
				$result = sirius_email_send($params);
				if ($result['success']) {
					sirius_minilog("$log_msg: Sent to $params[to].");
				} else {
					sirius_minilog("$log_msg: Failed to send to $params[to]: " . $result['msg'], 'warning');
				}
			}

			if ($medium == 'sms' || $medium == 'both') {
				$params['to'] = $user_full->field_grievance_phone['und'][0]['value'];
				if (!$params['to']) { sirius_minilog("$log_msg: No phone number found.", 'warning');  continue;  }

				$result = sirius_sms_send($params);
				if ($result['success']) {
					sirius_minilog("$log_msg: Sent to $params[to].");
				} else {
					sirius_minilog("$log_msg: Failed to send to $params[to]: " . $result['msg'], 'warning');
				}
			}
		}
	}

	if ($need_save) {
		$node->field_grievance_data_alert['und'][0]['value'] = serialize($alert_data);
		$node->grievance_no_recurse = TRUE;
		node_save($node);
		sirius_minilog('Alerts sent for: ' . $node->title);
	}

	return;
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