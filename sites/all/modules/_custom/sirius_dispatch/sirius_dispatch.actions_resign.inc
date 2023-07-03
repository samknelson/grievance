<?php

function sirius_dispatch_resign_possible($dispatch_node) {
	// Verify the dispatch status
	$status = $dispatch_node->field_sirius_dispatch_status['und'][0]['value'];
	if ($status != 'accepted' && $status != 'accepted_secondary') {
		return array('success' => FALSE, 'msg' => "Dispatch status is $status");
	}

	// Can't resign until four hours after the job's start date
	$job_node = node_load($dispatch_node->field_sirius_dispatch_job['und'][0]['target_id']);
	$start_ts = sirius_date_ts($job_node, 'field_sirius_datetime');
	$cutoff = strtotime('+4 hours', $start_ts);
	if ($cutoff > time()) {
		return array('success' => FALSE, 'msg' => t('It is not possible to voluntary layoff this dispatch until 4 hours after the start date (@date).', array('@date' => date('Y-m-d H:i:s', $cutoff))));
	}

	return array('success' => TRUE, 'msg' => 'Possible');
}

function sirius_dispatch_resign($dispatch_node, $notes = '') {
	$possible = sirius_dispatch_resign_possible($dispatch_node);
	if (!$possible['success']) {
		sirius_log('dispatch:resign', 'fail', "Voluntary layoff failed - " . $possible['msg'], '', '', array($dispatch_node->nid));
		return $possible;
	}

	// Set the "end" timestamp. 
	if (!$dispatch_node->field_sirius_date_end['und'][0]['value']) {
		$dispatch_node->field_sirius_date_end['und'][0]['value'] = date('Y-m-d');
	}

	// Set the dispatch status to "resigned", save and log
	$dispatch_node->field_sirius_dispatch_status['und'][0]['value'] = 'resigned';
	node_save($dispatch_node);
	sirius_log('dispatch:resign', 'complete', "Voluntary layoff complete", $notes, '', array($dispatch_node->nid));

	return array('success' => TRUE, 'msg' => 'Voluntary Layoff');
}
