<?php

function sirius_dispatch_accept_possible($dispatch_node) {
	// Verify the dispatch status
	$status = $dispatch_node->field_sirius_dispatch_status['und'][0]['value'];
	if ($status != 'requested' && $status != 'notified' && $status != 'notification failed' && $state != 'expired') {
		return array('success' => FALSE, 'msg' => "The dispatch status is $status");
	}

	// Verify the job status
	$job_node = node_load($dispatch_node->field_sirius_dispatch_job['und'][0]['target_id']);
	$job_status = $job_node->field_sirius_dispatch_job_status['und'][0]['value'];
	if ($job_status != 'Unfilled' && $job_status != 'Running') {
		return array('success' => FALSE, 'msg' => "The job status is $job_status");
	}

	// Verify that the job isn't full
	if ($job_node->field_sirius_count_yes['und'][0]['value'] >= $job_node->field_sirius_count['und'][0]['value']) {
		return array('success' => FALSE, 'msg' => 'The job is full.');
	}

	// @todo: Can accept if it's "expired", but only if the job isn't filled (count the dispatches)

	return array('success' => TRUE, 'msg' => 'Possible');
}

function sirius_dispatch_accept(&$dispatch_node, $notes = '') {
	$possible = sirius_dispatch_accept_possible($dispatch_node);
	if (!$possible['success']) {
		sirius_log('dispatch:accept', 'fail', "Accept failed - " . $possible['msg'], '', '', array($dispatch_node->nid));
		return $possible;
	}

	$dispatch_node->field_sirius_dispatch_status['und'][0]['value'] = 'accepted';
	if (!$dispatch_node->field_sirius_datetime_completed['und'][0]['value']) {
		$dispatch_node->field_sirius_datetime_completed['und'][0]['value'] = date('Y-m-d H:i:s');
	}
	node_save($dispatch_node);
	sirius_log('dispatch:accept', 'complete', "Accept complete", $notes, '', array($dispatch_node->nid));

	return array('success' => TRUE, 'msg' => 'Accepted');
}
