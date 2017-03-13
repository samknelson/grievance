<?php

/**
 * Iterate through all grievances, and send alerts to any that have a due date in the future.
 */

function grievance_alert_all() {
	watchdog('grievance_alert', t('Starting alert generation.'));

	$query = new EntityFieldQuery();
	$query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'grievance');
  $query->fieldCondition('field_grievance_alert_date', 'value', date('Y-m-d'), '>=');
	$stmt = $query->execute();
	if (!$stmt['node']) {
		watchdog('grievance_alert', t('No grievances have alert dates in the future.'));
	}

	$nids = array_keys($stmt['node']);
	foreach ($nids as $nid) {
		$node = node_load($nid);
		grievance_alert($node);
	}

	watchdog('grievance_alert', t('Alert generation complete'));

	return;
}

/**
 * Send email alerts to appropriate users based on the due date and status of the grievance.
 *
 * @param node The grievance node to examine.
 */

function grievance_alert($node) {
	watchdog('grievance_alert', t('Processing alerts for: ') . $node->title);

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
	$alert_tid = $node->field_grievance_alert['und'][0]['tid'];
	$alert_term = taxonomy_term_load($alert_tid);
	$alert = $alert_term->name;

	$need_save = FALSE;

	// We send up to five alerts.  For each possible deliver ...
	for ($i=1; $i<=5; ++$i) {
		// Have we already sent this alert?
		if ($alert_data[$alert][$deadline][$i]) { continue; }

		// How many days ahead should we warn?
		$days = variable_get('grievance_alert_days_warning' . $i, -999);
		if ($days == -999) { continue; }
		if ($days < 0) { continue; }

		// Is it due yet?
		if (strtotime("+$days days") < strtotime($deadline)) { continue; }

		// Send the alert
		$alert_data[$alert][$deadline][$i] = "SENT";
		$need_save = TRUE;

		$emails = grievance_alert_list_recipients($node);
		$to = array_shift($emails);
		if (!$to) { continue; }
		$cc = join(',', $emails);
		global $base_url;
		$params = array();

		$params['subject'] = "Grievance Deadline Alert #$i: Only $days days remain to $alert ($node->title)";

		$params['body'] =  "Alert #   : $i\n";
		$params['body'] .= "Grievance : $node->title\n";
		$params['body'] .= "Deadline  : $deadline\n";
		$params['body'] .= "Union Must: $alert\n";
		$params['body'] .= "Link      : $base_url/node/$node->nid\n";

		if ($cc) {
			$params['headers']['Cc'] = $cc;
		}

		drupal_mail(
			'grievance', 
			'alert', 
			$to, 
			LANGUAGE_NONE, 
			$params
		);
	}

	if ($need_save) {
		$node->field_grievance_data_alert['und'][0]['value'] = serialize($alert_data);
		$node->grievance_no_recurse = TRUE;
		node_save($node);
		watchdog('grievance_alert', t('Alerts sent for: ') . $node->title);
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
				$user = user_load($uid);
				if (!$user) { continue; }
				$result[] = $user->mail;
			}
		}
	}

	$result = array_unique($result);

	return $result;
}