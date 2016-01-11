<?php

function grievance_notify($node) {
	// Find the status 
	$status_tid = $node->field_grievance_status['und'][0]['tid'];
	if (!$status_tid) { return; }
	$status_term = taxonomy_term_load($status_tid);
	if (!$status_term) { return; }

	// Only notify if this status is set to send notifications. 
	if (!$status_term->field_grievance_notify['und'][0]['value']) { return; }

	// Only notify if the status has changed.
	$orig_status_tid = 0;
	if ($node->original) {
		$orig_status_tid = $node->original->field_grievance_status['und'][0]['tid'];
	}
	if ($status_tid == $orig_status_tid) { return; }

	$email = $node->field_grievance_co_email['und'][0]['email'];
	if (!$email) { 
		drupal_set_message(t('Warning: No notification has been sent, because no company representative email has been entered.  The company has NOT been notified!'), 'warning');
		return;
	}

	$cc = join(',', grievance_alert_list_recipients($node));

	// We're good.  Send a notification.
	$params = array();
	$params['body'] = $status_term->field_grievance_notify_body['und'][0]['value'];
	$params['subject'] = $status_term->field_grievance_notify_subject['und'][0]['value'];
	if ($cc) {
		$params['headers']['Cc'] = $cc;
	}
	$params['attachments'] = array();

	// Attach the grievance as a PDF
	$params['attachments'][] = array(
		'filecontent' => grievance_gform_pdf_contents($node),
		'filename' => 'grievance_' . $node->nid . '.pdf',
		'filemime' => 'grievance/pdf',
	);


	drupal_mail(
		'grievance', 
		'notify', 
		$email, 
		LANGUAGE_NONE, 
		$params
	);

	grievance_log_notify($node, $email);

	drupal_set_message(t('Notification sent to: ') . $email);
}

