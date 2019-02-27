<?php
function sirius_dispatch_alert_employer_dispatch($dispatch_node) {
	$job_node = node_load($dispatch_node->field_sirius_dispatch_job['und'][0]['target_id']);
	if (!$job_node) { return; }
	if ($job_node->field_sirius_notify['und'][0]['value'] != 'Yes') { return; }
	$email = $job_node->field_sirius_email['und'][0]['value'];
	if (!$email) { return; }
	if (!$job_node->field_sirius_dispatch_job_nfcns['und'][0]) { return; }

	$notifications = array();
	foreach ($job_node->field_sirius_dispatch_job_nfcns['und'] as $tmp) {
		$notifications[$tmp['value']] = TRUE; 
	}

	dpm($notifications);
	// drupal_set_message("Updating dispatch with " . $node->type);
}

function sirius_dispatch_alert_employer_job($node) {
	drupal_set_message("Updfating job with " . $node->type);
}