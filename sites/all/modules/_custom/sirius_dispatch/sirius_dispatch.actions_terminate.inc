<?php

function sirius_dispatch_terminate_possible($dispatch_node) {
	// Verify the dispatch status
	$status = $dispatch_node->field_sirius_dispatch_status['und'][0]['value'];
	if ($status != 'accepted') {
		return array('success' => FALSE, 'msg' => "Dispatch status is $status");
	}

	return array('success' => TRUE, 'msg' => 'Possible');
}

function sirius_dispatch_terminate($dispatch_node, $notes = '') {
	$possible = sirius_dispatch_terminate_possible($dispatch_node);
	if (!$possible['success']) {
		sirius_log('dispatch:terminate', 'fail', "Terminate failed - " . $possible['msg'], '', '', array($dispatch_node->nid));
		return $possible;
	}

	// Set the dispatch status to "terminated", save and log
	$dispatch_node->field_sirius_dispatch_status['und'][0]['value'] = 'terminated';
	node_save($dispatch_node);
	sirius_log('dispatch:terminate', 'complete', "Terminate complete", $notes, '', array($dispatch_node->nid));

	return array('success' => TRUE, 'msg' => 'Terminated');
}
