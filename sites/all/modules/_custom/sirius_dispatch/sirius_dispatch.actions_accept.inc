<?php

function sirius_dispatch_accept_possible($dispatch_node) {
	// Verify the dispatch status
	$status = $dispatch_node->field_sirius_dispatch_status['und'][0]['value'];
	if ($status != 'requested' && $status != 'notified' && $status != 'notification failed' && $status != 'expired') {
		return array('success' => FALSE, 'msg' => "The dispatch status is $status");
	}

	// Verify the job status
	$job_node = node_load($dispatch_node->field_sirius_dispatch_job['und'][0]['target_id']);
	$job_status = $job_node->field_sirius_dispatch_job_status['und'][0]['value'];
	if ($job_status != 'Unfilled' && $job_status != 'Running') {
		return array('success' => FALSE, 'msg' => "The job status is $job_status");
	}

	// Verify that the job isn't full
	$worker_nid = $dispatch_node->field_sirius_worker['und'][0]['target_id'];
	if ($job_node->field_sirius_count['und'][0]['value'] > 0) {
		if ($job_node->field_sirius_count_yes['und'][0]['value'] >= $job_node->field_sirius_count['und'][0]['value']) {
			return array('success' => FALSE, 'msg' => 'The job is full.');
		}
	}

	// Verify that the worker is still dispatchable for this job
	$check = sirius_dispatch_job_get_dispatchable_workers($job_node, 'name', 1, array('worker_nid' => $worker_nid, 'redispatch' => TRUE));
	if (!$check) {
		return array('success' => FALSE, 'msg' => 'This worker can no longer be dispatched to this job.');
	}

	return array('success' => TRUE, 'msg' => 'Possible');
}

function sirius_dispatch_accept(&$dispatch_node, $notes = '') {
	// Is accept possible?
	$possible = sirius_dispatch_accept_possible($dispatch_node);
	if (!$possible['success']) {
		sirius_log('dispatch:accept', 'fail', "Accept failed - " . $possible['msg'], '', '', array($dispatch_node->nid));
		return $possible;
	}

	// Remove from the event
	$job_node = node_load($dispatch_node->field_sirius_dispatch_job['und'][0]['target_id']);
	$event_nid = $job_node->field_sirius_event['und'][0]['target_id'];
	if ($event_nid) {
		$contact_nid = sirius_contact_get_nid_from_worker($dispatch_node->field_sirius_worker['und'][0]['target_id']);
		$event_node = node_load($event_nid);
		$event_type_term = taxonomy_term_load($event_node->field_sirius_event_type['und'][0]['tid']);
		$json = json_decode($event_type_term->field_sirius_json['und'][0]['value'], TRUE);
		list ($prole_tid, $pstatus_tid) = explode('|', $json['bullpen']['post_dispatch']);
		$participant_node = sirius_event_participant_get($event_nid, $contact_nid);
		if ($participant_node && $prole_tid && $pstatus_tid) {
			$participant_node->field_sirius_event_prole['und'][0]['tid'] = $prole_tid;
			$participant_node->field_sirius_event_pstatus['und'][0]['tid'] = $pstatus_tid;
			node_save($participant_node);
		}
	}

	// Set the status
	$worker_node = node_load($dispatch_node->field_sirius_worker['und'][0]['target_id']);
	if ($worker_node->field_grievance_shop['und'][0]['target_id']) {
		$new_status = 'accepted_secondary';
	} else {
		$new_status = 'accepted';
	}
	$dispatch_node->field_sirius_dispatch_status['und'][0]['value'] = $new_status;

	// Set the timestamp
	if (!$dispatch_node->field_sirius_datetime_completed['und'][0]['value']) {
		$dispatch_node->field_sirius_datetime_completed['und'][0]['value'] = date('Y-m-d H:i:s');
	}

	// Save and log
	node_save($dispatch_node);
	sirius_log('dispatch:accept', 'complete', "Accept complete", $notes, '', array($dispatch_node->nid));

	// Send the worker a copy of their dispatch
	sirius_dispatch_send($dispatch_node, NULL, NULL, 'The dispatch has been accepted.');

	return array('success' => TRUE, 'msg' => 'Accepted');
}
