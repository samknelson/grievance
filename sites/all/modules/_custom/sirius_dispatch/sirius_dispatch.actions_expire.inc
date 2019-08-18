<?php

function sirius_dispatch_expire_possible($dispatch_node) {
	// Verify the dispatch status
	$status = $dispatch_node->field_sirius_dispatch_status['und'][0]['value'];
	if ($status != 'notified') {
		return array('success' => FALSE, 'msg' => "Dispatch status is $status");
	}

	return array('success' => TRUE, 'msg' => 'Possible');
}

function sirius_dispatch_expire($dispatch_node, $notes = '') {
	$possible = sirius_dispatch_expire_possible($dispatch_node);
	if (!$possible['success']) {
		sirius_log('dispatch:expire', 'fail', "Expire failed - " . $possible['msg'], '', '', array($dispatch_node->nid));
		return $possible;
	}

	// Set the dispatch status to "expired", save and log
	$dispatch_node->field_sirius_dispatch_status['und'][0]['value'] = 'expired';
	node_save($dispatch_node);
	sirius_log('dispatch:expire', 'complete', "Expire complete", $notes, '', array($dispatch_node->nid));

	return array('success' => TRUE, 'msg' => 'Expired');
}
