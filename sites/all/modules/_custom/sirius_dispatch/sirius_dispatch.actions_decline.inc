<?php

function sirius_dispatch_decline_possible($dispatch_node) {
	// Verify the dispatch status
	$status = $dispatch_node->field_sirius_dispatch_status['und'][0]['value'];
	if ($status != 'requested' && $status != 'notified' && $status != 'notification failed') {
		return array('success' => FALSE, 'msg' => "Dispatch status is $status");
	}

	// Verify the job status
	$job_node = node_load($dispatch_node->field_sirius_dispatch_job['und'][0]['target_id']);
	$job_status = $job_node->field_sirius_dispatch_job_status['und'][0]['value'];
	if ($job_status != 'Unfilled' && $job_status != 'Running' && $job_status != 'Pending') {
		return array('success' => FALSE, 'msg' => "Job status is $job_status");
	}

	return array('success' => TRUE, 'msg' => 'Possible');
}

function sirius_dispatch_decline(&$dispatch_node, $notes = '') {
	$possible = sirius_dispatch_decline_possible($dispatch_node);
	if (!$possible['success']) {
		sirius_log('dispatch:decline', 'fail', "Decline failed - " . $possible['msg'], '', '', array($dispatch_node->nid));
		return $possible;
	}

	$dispatch_node->field_sirius_dispatch_status['und'][0]['value'] = 'declined';
	if (!$dispatch_node->field_sirius_datetime_completed['und'][0]['value']) {
		$dispatch_node->field_sirius_datetime_completed['und'][0]['value'] = date('Y-m-d H:i:s');
	}
	node_save($dispatch_node);
	sirius_log('dispatch:decline', 'complete', "Decline complete", $notes, '', array($dispatch_node->nid));

	return array('success' => TRUE, 'msg' => 'Declined');
}
