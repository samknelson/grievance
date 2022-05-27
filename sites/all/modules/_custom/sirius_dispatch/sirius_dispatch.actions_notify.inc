<?php

function sirius_dispatch_notify_medium_options($worker_node = NULL) {
	$options = array(
		'none' => t('None'),
		'sms' => t('Text message to the mobile phone number'),
		'voice' => t('Voice call to the mobile phone number'),
		'sms_alt' => t('Text message to the secondary phone number'),
		'voice_alt' => t('Voice call to the secondary phone number'),
		'email' => t('Email'),
		'none' => t('None'),
	);

	if ($worker_node) {
		if (!$worker_node->field_sirius_email['und'][0]['value']) { unset($options['email']); }
		if (!$worker_node->field_sirius_phone['und'][0]['value']) { unset($options['voice']); unset($options['sms']); }
		if (!$worker_node->field_sirius_phone_alt['und'][0]['value']) { unset($options['voice_alt']); unset($options['sms_alt']); }
	}

	return $options;
}

function sirius_dispatch_notify_possible($dispatch_node) {
	// Verify the dispatch status
	$status = $dispatch_node->field_sirius_dispatch_status['und'][0]['value'];
	if ($status != 'requested' && $status != 'notified' && $status != 'notification failed' && $status != 'expired') {
		return array('success' => FALSE, 'msg' => "Dispatch status is $status");
	}

	// Verify the job status
	$job_node = node_load($dispatch_node->field_sirius_dispatch_job['und'][0]['target_id']);
	$job_status = $job_node->field_sirius_dispatch_job_status['und'][0]['value'];
	if ($job_status != 'Unfilled' && $job_status != 'Running' && $job_status != 'Pending') {
		return array('success' => FALSE, 'msg' => "Job status is $job_status");
	}

	// Verify that the worker is dispatchable
	$worker_nid = $dispatch_node->field_sirius_worker['und'][0]['target_id'];
	$check = sirius_dispatch_job_get_dispatchable_workers_explain($job_node, $worker_nid, array('redispatch' => TRUE));
	if (!$check['success']) {
		return array('success' => FALSE, 'msg' => "Dispatch not possible: " . $check['msg']);
	}

	return array('success' => TRUE, 'msg' => 'Possible');
}

function sirius_dispatch_notify(&$dispatch_node, $requested_medium = NULL, $notes = '') {
	// Possible?
	$possible = sirius_dispatch_notify_possible($dispatch_node);
	if (!$possible['success']) {
		$unique_id = $dispatch_node->nid . '/dispatch_not_possible/' . $possible['msg'];
		sirius_log(
			'dispatch:notify', 
			'fail', 
			"Notify failed - " . $possible['msg'], 
			$notes . ' ' . $possible['msg'] . ' (subsequent messages of this type will be supressed.)', 
			$unique_id, 
			array($dispatch_node->nid),
			NULL,
			array('log_once' => TRUE)
		);

		return $possible;
	}

	// Load nodes and terms
	$worker_node = node_load($dispatch_node->field_sirius_worker['und'][0]['target_id']);
	$job_node = node_load($dispatch_node->field_sirius_dispatch_job['und'][0]['target_id']);
	$employer_node = node_load($job_node->field_grievance_shop['und'][0]['target_id']);
	$job_type_term = taxonomy_term_load($job_node->field_sirius_dispatch_job_type['und'][0]['tid']);
	global $base_url;
	$url = "$base_url/d/" . $dispatch_node->nid . "/" . $dispatch_node->field_sirius_aat['und'][0]['value'];

	// Set our media. NULL means the worker's default; there might be multiple defaults.
	$media = array();
	if ($requested_medium) {
		$media[$requested_medium] = array();
	} else {
		if ($worker_node->field_sirius_dispatch_medium['und']) {
			foreach ($worker_node->field_sirius_dispatch_medium['und'] as $delta => $tmp) {
				if ($delta <= 1) {
					$media[$tmp['value']] = array();
				}
			}
		}
	}
	if (!$media) { return array('success' => FALSE, 'msg' => 'This worker has no dispatch medium set.'); }

	// What is our "secondary" medium? I.e. if we can't send another call by voice?
	$secondary_medium = $worker_node->field_sirius_dispatch_medium['und'][2]['value'];
	if (!$secondary_medium) { $secondary_medium = 'sms'; }

	// We cannot call multiple times.
	$tmp = array_keys($media);
	foreach ($tmp as $medium) {
		if ($medium == 'voice' || $medium == 'voice_alt') {
			$unique_id = "UNIQUE/";
			$unique_id .= "dispatch-notification/";
			$unique_id .= $medium . "/";
			$unique_id .= $worker_node->nid . "/";
			$unique_id .= date('Y-m-d');

			if (sirius_voice_already_sent($unique_id)) {
				unset($media[$medium]);
				$media[$secondary_medium] = array();
			} else {
				$media[$medium]['X-Sirius-Id'] = $unique_id;
			}
		}
	}

	// Set the "to" for each medium
	foreach ($media as $medium => $ignore) {
		if ($medium == 'sms') 						{ $media[$medium]['to'] = sirius_phone_format_e164($worker_node->field_sirius_phone['und'][0]['value']); }
		else if ($medium == 'voice')			{ $media[$medium]['to'] = sirius_phone_format_e164($worker_node->field_sirius_phone['und'][0]['value']); }
		else if ($medium == 'sms_alt') 		{ $media[$medium]['to'] = sirius_phone_format_e164($worker_node->field_sirius_phone_alt['und'][0]['value']); }
		else if ($medium == 'voice_alt') 	{ $media[$medium]['to'] = sirius_phone_format_e164($worker_node->field_sirius_phone_alt['und'][0]['value']); }
		else if ($medium == 'email') 			{ $media[$medium]['to'] = $worker_node->field_sirius_email['und'][0]['value']; }
	}

	// All our "sends" will use these paramters.
	$params = array(
		'voice' => $voice_text,
		'sms' => $sms_text,
		'email_subject' => $email_subject,
		'email_body' => $email_body,
		'handler_nids' => array($dispatch_node->nid),
		'worker_nid' => $worker_node->nid,
	);

	// SMS parameters
	$params['sms'] = "Teamsters 631 is announcing a job dispatch.\n";
	$params['sms'] .= "Employer: " . $employer_node->title . "\n";
	$params['sms'] .= "Call type: " . $job_type_term->name . "\n";
	$params['sms'] .= "Report date: " . sirius_date_date($job_node, 'field_sirius_datetime', 'l, F j, y \a\t g:i A') . "\n";
	$params['sms'] .= "Link: $url\n";

	$params['subject'] = "Teamsters 631 job dispatch announcement: " . $employer_node->title;
	$params['body'] = "Teamsters 631 is announcing a job dispatch.<br /><br />";
	$params['body'] .= "Employer: <strong>" . $employer_node->title . "</strong><br />";
	$params['body']  .= "Call type: <strong>" . $job_type_term->name . "</strong><br />";
	$params['body']  .= "Report date: <strong>" . sirius_date_date($job_node, 'field_sirius_datetime', 'l, F j, y \a\t g:i A') . "</strong><br /><br />";
	$params['body']  .= "To view the full dispatch and accept or decline, please follow the link below:<br /><br />";
	$params['body']  .= "<a href=\"$url\">$url</a>";

	$params['voice'] = "Hi, " . $worker_node->field_sirius_name['und'][0]['given'] . ". ";
	$params['voice'] .= "This call is to notify you that work is available from " . variable_get('sirius_twilio_site_name_tts', '') . ". ";
	$params['voice'] .= "Any additional notifications today will be sent via ";
	if ($secondary_medium == 'email') { $params['voice'] .= "email. "; }
	if ($secondary_medium == 'sms') { $params['voice'] .= "text message to your primary phone number. "; }
	if ($secondary_medium == 'sms_alt') { $params['voice'] .= "text message to your secondary phone number. "; }

	$success = FALSE;
	$msg = '';
	foreach ($media as $medium => $hr) {
		$result = NULL;
		if ($medium == 'email') { $result = sirius_email_send($params + $hr); }
		if ($medium == 'voice' || $medium == 'voice_alt') { $result = sirius_voice_send($params + $hr); }
		if ($medium == 'sms' || $medium == 'sms_alt') { $result = sirius_sms_send($params + $hr); }
		$success = ($success || $result['success']);
		if ($msg) { $msg .= ' / '; }
		$msg .= $result['msg'];
	}

	if (!$success) {
		sirius_log('dispatch:notify', 'fail', $msg, $notes, '', array($dispatch_node->nid));
		$dispatch_node->field_sirius_datetime['und'][0]['value'] = date('Y-m-d H:i:s');
		$dispatch_node->field_sirius_dispatch_status['und'][0]['value'] = 'notification failed';
	} else {
		sirius_log('dispatch:notify', 'complete', "Notification sent by " . join(', ', array_keys($media)), $notes, '', array($dispatch_node->nid));
		if (!$dispatch_node->field_sirius_datetime['und'][0]['value']) {
			$dispatch_node->field_sirius_datetime['und'][0]['value'] = date('Y-m-d H:i:s');
		} 
		$dispatch_node->field_sirius_dispatch_status['und'][0]['value'] = 'notified';
	}
	node_save($dispatch_node);
	return array('success' => $success, 'msg' => $msg);
}
