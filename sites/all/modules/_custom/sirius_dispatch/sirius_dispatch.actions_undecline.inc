<?php

function sirius_dispatch_undecline_possible($dispatch_node) {
	// Verify the dispatch status
	$status = $dispatch_node->field_sirius_dispatch_status['und'][0]['value'];
	if ($status != 'declined') {
		return array('success' => FALSE, 'msg' => "Dispatch status is $status");
	}

	// Verify the job status
	$job_node = node_load($dispatch_node->field_sirius_dispatch_job['und'][0]['target_id']);
	$job_status = $job_node->field_sirius_dispatch_job_status['und'][0]['value'];
	if ($job_status != 'Unfilled' && $job_status != 'Running') {
		return array('success' => FALSE, 'msg' => "Job status is $job_status");
	}

	return array('success' => TRUE, 'msg' => 'Possible');
}

function sirius_dispatch_undecline(&$dispatch_node, $notes = '') {
	$possible = sirius_dispatch_undecline_possible($dispatch_node);
	if (!$possible['success']) {
		sirius_log('dispatch:undecline', 'fail', "Decline failed - " . $possible['msg'], '', '', array($dispatch_node->nid));
		return $possible;
	}

	$dispatch_node->field_sirius_dispatch_status['und'][0]['value'] = 'notified';
	unset($dispatch_node->field_sirius_datetime_completed);
	node_save($dispatch_node);
	sirius_log('dispatch:undecline', 'complete', "Undecline complete", $notes, '', array($dispatch_node->nid));

	return array('success' => TRUE, 'msg' => 'Undeclined');
}
