<?php

function sirius_dispatch_send_medium_options($worker_node = NULL) {
	$options = array(
		'none' => t('None'),
		'sms' => t('Text message to the primary phone number'),
		'sms_alt' => t('Text message to the secondary phone number'),
		'email' => t('Email'),
		'none' => t('None'),
	);

	if ($worker_node) {
		if (!$worker_node->field_sirius_email['und'][0]['value']) { $options['email'] .= t(' - missing'); }
		if (!$worker_node->field_sirius_phone['und'][0]['value']) { $options['sms'] .= t(' - missing'); }
		if (!$worker_node->field_sirius_phone_alt['und'][0]['value']) { $options['sms_alt'] .= t(' - missing'); }
	}

	return $options;
}

function sirius_dispatch_send(&$dispatch_node, $medium = NULL, $to = NULL, $notes = '') {
	$worker_node = node_load($dispatch_node->field_sirius_worker['und'][0]['target_id']);
	$job_node = node_load($dispatch_node->field_sirius_dispatch_job['und'][0]['target_id']);

	$context = array();
	$context['handler'] = 'dispatch';
	$context['dispatch_nid'] = $dispatch_node->nid;
	$context['worker_nid'] = $worker_node->nid;


	// No medium: use all the workers defaults
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
			$result = sirius_dispatch_send($dispatch_node, $medium, $to, $notes);
		}
		return $result;
	}

	global $base_url;
	$url = "$base_url/d/" . $dispatch_node->nid . "/" . $dispatch_node->field_sirius_aat['und'][0]['value'];

	if ($medium == 'sms') {	
		if (!$to) { $to = sirius_phone_format_e164($worker_node->field_sirius_phone['und'][0]['value']); }
		if (!sirius_phone_validate($to)) { return array('success' => FALSE, 'msg' => 'No valid phone number.'); }
		$text = "Your dispatch can be found at:\n$url";
		if ($notes) { $text .= "\n\n$notes\n"; }
		return sirius_twilio_outgoing_sms($to, $text, $dispatch_node->nid);
	} else if ($medium == 'sms_alt') {
		if (!$to) { $to = sirius_phone_format_e164($worker_node->field_sirius_phone_alt['und'][0]['value']); }
		if (!sirius_phone_validate($to)) { return array('success' => FALSE, 'msg' => 'No valid phone number.'); }
		$text = "Your dispatch can be found at:\n$url";
		if ($notes) { $text .= "\n\n$notes\n"; }
		return sirius_twilio_outgoing_sms($to, $text, $dispatch_node->nid);
	} else if ($medium == 'email') {
		if (!$to) { $to = $worker_node->field_sirius_email['und'][0]['value']; }
		if (!sirius_email_validate($to)) { return array('success' => FALSE, 'msg' => 'No valid email address.'); }
		$subject = "Dispatch #" . $dispatch_node->nid;
		$body = "Your dispatch can be found at:<br /><br /><a href=\"$ur;\">$url</a>";
		if ($notes) { $body .= $notes; }
		return sirius_email_send(array('to' => $to, 'subject' => $subject, 'body' => $body, 'handler_nids' => array($dispatch_node->nid)));
	} else if ($medium == 'voice' || $medium == 'voice_alt') {
		return array('success' => FALSE, 'msg' => 'Cannot send by voice.');
	} else if ($medium == 'none') {
		return array('success' => TRUE, 'msg' => 'Medium "none" selected, no notification sent.');
	} else {
		return array('success' => FALSE, 'msg' => "Unknown medium: $medium");
	}
}
