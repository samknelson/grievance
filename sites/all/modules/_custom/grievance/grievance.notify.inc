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
	// if ($status_tid == $orig_status_tid) { return; }

	// We're good.  Send a notification.
	$params = array();
	$params['body'] = 'Hello, notification!';
	$params['subject'] = 'Hello, notification subject.';
	$params['attachments'] = array();

	// Attach the grievance as a PDF
	$params['attachments'][] = array(
		'filecontent' => grievance_gform_pdf_contents($node),
		'filename' => 'grievance_' . $node->nid . '.pdf',
		'filemime' => 'grievance/pdf',
	);

	$email = 'samknelson@gmail.com';

	drupal_mail(
		'grievance', 
		'notify', 
		$email, 
		LANGUAGE_NONE, 
		$params
	);

	drupal_set_message("Notifying.");
}

