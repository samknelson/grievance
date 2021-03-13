<?php

function sirius_dispatch_actions_create_possible($job_node, $worker_nid) {
	// @todo verify that the job isn't full
	
	// This worker must be dispatchable for this job
	/*
	$check = sirius_dispatch_job_get_dispatchable_workers($job_node, 'name', 1, array('worker_nid' => $worker_nid));
	if (!$check[$worker_nid]) {
		$worker_node = node_load($worker_nid);
		return array('success' => FALSE, 'msg' => "The worker " . $worker_node->title . " cannot be dispatched on this job.");
	}
	*/

	// Can't have multiple dispatches
  $query = new EntityFieldQuery;
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'sirius_dispatch');
  $query->fieldCondition('field_sirius_dispatch_job', 'target_id', $job_node->nid);
  $query->fieldCondition('field_sirius_worker', 'target_id', $worker_nid);
  $stmt = $query->execute();
  if ($stmt['node']) {
  	return array('success' => FALSE, 'msg' => 'There is already a dispatch for this worker and this job.');
  }

	return array('success' => TRUE);
}

function sirius_dispatch_actions_create($job_node, $worker_nid, $cbn = 'No', $status = 'requested', $completed_ts = NULL) {
	$possible = sirius_dispatch_actions_create_possible($job_node, $worker_nid);
	if (!$possible['success']) { return $possible; }

	global $user;
	$dispatch_node = new stdClass();
	$dispatch_node->type = 'sirius_dispatch';
	$dispatch_node->language = LANGUAGE_NONE;
	$dispatch_node->uid = $user->uid;
	$dispatch_node->status = 1;
	$dispatch_node->promote = 0;
	$dispatch_node->comment = 0;
	$dispatch_node->field_sirius_dispatch_job['und'][0]['target_id'] = $job_node->nid;
	$dispatch_node->field_sirius_worker['und'][0]['target_id'] = $worker_nid;
	$dispatch_node->field_sirius_dispatch_status['und'][0]['value'] = $status;
	$dispatch_node->field_sirius_dispatch_cbn['und'][0]['value'] = $cbn;
	$dispatch_node->field_sirius_domain = $job_node->field_sirius_domain;
	if ($completed_ts) {
		$dispatch_node->field_sirius_datetime['und'][0]['value'] = gmdate('Y-m-d H:i:s', $completed_ts);
		$dispatch_node->field_sirius_datetime_created['und'][0]['value'] = gmdate('Y-m-d H:i:s', $completed_ts);
		$dispatch_node->field_sirius_datetime_completed['und'][0]['value'] = gmdate('Y-m-d H:i:s', $completed_ts);
	}
	$dispatch_node = node_submit($dispatch_node);
	node_object_prepare($dispatch_node);
	node_save($dispatch_node);
	sirius_log('dispatch:create', 'complete', "Dispatch created", '', '', array($dispatch_node->nid));
	return array('success' => TRUE, 'dispatch_node' => $dispatch_node, 'msg' => 'Dispatch created.');
}
