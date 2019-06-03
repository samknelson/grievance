<?php

function sirius_dispatch_notify_medium_options($worker_node = NULL) {
	$options = array(
		'none' => t('None'),
		'sms' => t('Text message to the primary phone number'),
		'voice' => t('Voice call to the primary phone number'),
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
  if ($job_status != 'Unfilled' && $job_status != 'Running') {
    return array('success' => FALSE, 'msg' => "Job status is $job_status");
  }

  // Verify that the worker is dispatchable
  $worker_nid = $dispatch_node->field_sirius_worker['und'][0]['target_id'];
  $check = sirius_dispatch_job_get_dispatchable_workers($job_node, 'name', 1, array('worker_nid' => $worker_nid, 'redispatch' => TRUE));
  if (!$check[$worker_nid]) {
    $worker_node = node_load($worker_nid);
    return array('success' => FALSE, 'msg' => "The worker " . $worker_node->title . " cannot be dispatched on this job.");
  }

  return array('success' => TRUE, 'msg' => 'Possible');
}

function sirius_dispatch_notify(&$dispatch_node, $medium = NULL, $notes = '') {
	$possible = sirius_dispatch_notify_possible($dispatch_node);
	if (!$possible['success']) {
		sirius_log('dispatch:notify', 'fail', "Notify failed - " . $possible['msg'], $notes, '', array($dispatch_node->nid));
		return $possible;
	}

	$worker_node = node_load($dispatch_node->field_sirius_worker['und'][0]['target_id']);
	$job_node = node_load($dispatch_node->field_sirius_dispatch_job['und'][0]['target_id']);
	$employer_node = node_load($job_node->field_grievance_shop['und'][0]['target_id']);
	$job_type_term = taxonomy_term_load($job_node->field_sirius_dispatch_job_type['und'][0]['tid']);
	$tokens = array(
		'dispatch_node' => $dispatch_node,
		'job_node' => $job_node,
		'worker_node' => $worker_node,
	);
	$context = array();
	$context['handler'] = 'dispatch';
	$context['dispatch_nid'] = $dispatch_node->nid;
	$context['worker_nid'] = $worker_node->nid;

	global $base_url;
	$url = "$base_url/d/" . $dispatch_node->nid . "/" . $dispatch_node->field_sirius_aat['und'][0]['value'];

	$sms_text = "Teamsters 631 is announcing a job dispatch.\n";
	$sms_text .= "Employer: " . $employer_node->title . "\n";
	$sms_text .= "Call type: " . $job_type_term->name . "\n";
	$sms_text .= "Report date: " . sirius_date_date($job_node, 'field_sirius_datetime', 'l, F j, y \a\t g:i A') . "\n";
	$sms_text .= "Link: $url\n";
	$sms_text .= "\n";
	$sms_text .= "To accept this job, reply:\n";
	$sms_text .= $dispatch_node->nid . " accept\n";
	$sms_text .= "\n";
	$sms_text .= "To decline this job, reply:\n";
	$sms_text .= $dispatch_node->nid . " decline\n";

	$email_subject = "Teamsters 631 job dispatch announcement: " . $employer_node->title;
	$email_body = "Teamsters 631 is announcing a job dispatch.<br /><br />";
	$email_body .= "Employer: <strong>" . $employer_node->title . "</strong><br />";
	$email_body .= "Call type: <strong>" . $job_type_term->name . "</strong><br />";
	$email_body .= "Report date: <strong>" . sirius_date_date($job_node, 'field_sirius_datetime', 'l, F j, y \a\t g:i A') . "</strong><br /><br />";
	$email_body .= "To view the full dispatch and accept or decline, please follow the link below:<br /><br />";
	$email_body .= "<a href=\"$url\">$url</a>";

	// No medium: use all the workers defauls
	if (!$medium) {

		$is_primary = TRUE;
		if (!$worker_node->field_sirius_dispatch_medium['und'][0]) {
			return array('success' => FALSE, 'msg' => 'This worker has no dispatch medium set.');
		}
		foreach ($worker_node->field_sirius_dispatch_medium['und'] as $delta => $tmp) {
			if ($delta > 1) { break; }
			$medium = $tmp['value'];
			if (!$medium) { continue; }
			if ($medium == 'none') { continue; }
			if ($is_primary) { 
				$notes = "Primary notification.";
				$is_primary = FALSE;
			} else {
				$notes = "Secondary notification.";
			}
			$result = sirius_dispatch_notify($dispatch_node, $medium, $notes);
		}
		return $result;

	} else if ($medium == 'sms') {	

		$phone = sirius_phone_format_e164($worker_node->field_sirius_phone['und'][0]['value']);
		$result = sirius_twilio_outgoing_sms($phone, $sms_text, $dispatch_node->nid);

	}
	else if ($medium == 'voice') {

		$phone = sirius_phone_format_e164( $worker_node->field_sirius_phone['und'][0]['value']);
		$result = sirius_twilio_outgoing_voice($phone, $context);

	} else if ($medium == 'sms_alt') {

		$phone = sirius_phone_format_e164($worker_node->field_sirius_phone_alt['und'][0]['value']);
		$result = sirius_twilio_outgoing_sms($phone, $sms_text, $dispatch_node->nid);

	} else if ($medium == 'voice_alt') {

		$phone = sirius_phone_format_e164($worker_node->field_sirius_phone_alt['und'][0]['value']);
		$result = sirius_twilio_outgoing_voice($phone, $context);

	} else if ($medium == 'email') {

		$email = $worker_node->field_sirius_email['und'][0]['value'];
		$result = sirius_email_send(array('to' => $email, 'subject' => $email_subject, 'body' => $email_body, 'handler_nids' => array($dispatch_node->nid)));

	} else if ($medium == 'none') {

		$result = array('success' => TRUE, 'msg' => 'Medium "none" selected, no notification sent.');

	} else {

		$result = array('success' => FALSE, 'msg' => "Unknown medium: $medium");

	}

	if (!$result['success']) {
		sirius_log('dispatch:notify', 'fail', $result['msg'], $notes, '', array($dispatch_node->nid));
		$dispatch_node->field_sirius_datetime['und'][0]['value'] = date('Y-m-d H:i:s');
		$dispatch_node->field_sirius_dispatch_status['und'][0]['value'] = 'notification failed';
	} else {
		sirius_log('dispatch:notify', 'complete', "Notification sent by $medium", $notes, '', array($dispatch_node->nid));
		if (!$dispatch_node->field_sirius_datetime['und'][0]['value']) {
			$dispatch_node->field_sirius_datetime['und'][0]['value'] = date('Y-m-d H:i:s');
		} 
		$dispatch_node->field_sirius_dispatch_status['und'][0]['value'] = 'notified';
	}
	node_save($dispatch_node);
	return $result;
}

function sirius_dispatch_notify_schedule_ok() {
	$day = date('N');
	$hour = date('G');
	$minute = (int) date('i');

	$enabled = variable_get("sirius_dispatch_run_schedule_${day}_enable}", "Yes");
	$start_hour 	= variable_get("sirius_dispatch_run_schedule_${day}_start_hour}", 0);
	$start_minute = variable_get("sirius_dispatch_run_schedule_${day}_start_minute}", 0);
	$start_minute_display = str_pad($start_minute, 2, '0', STR_PAD_LEFT);
	$stop_hour 		= variable_get("sirius_dispatch_run_schedule_${day}_stop_hour}", 0);
	$stop_minute 	= variable_get("sirius_dispatch_run_schedule_${day}_stop_minute}", 0);
	$stop_minute_display = str_pad($stop_minute, 2, '0', STR_PAD_LEFT);

	if ($enabled != 'Yes') {
		return array('success' => FALSE, 'msg' => 'Notifications cannot be sent today.');
	}

	if ($start_hour > $hour || ($start_hour == $hour && $start_minute > $minute)) {
		return array('success' => FALSE, 'msg' => "Notifications cannot be sent until $start_hour:$start_minute_display.");
	}

	if ($hour > $stop_hour || ($hour == $stop_hour && $minute > $stop_minute)) {
		return array('success' => FALSE, 'msg' => "Notifications cannot be sent after $stop_hour:$stop_minute_display.");
	}

	return array('success' => TRUE);
}

