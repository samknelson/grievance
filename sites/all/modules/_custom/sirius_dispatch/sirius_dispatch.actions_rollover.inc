<?php

function sirius_dispatch_rollover_possible($dispatch_node, $rollover_employer_nid = NULL, $rollover_type = 'permanent', $end_date = NULL) {
	// Verify the dispatch status
	$status = $dispatch_node->field_sirius_dispatch_status['und'][0]['value'];
	if ($status != 'accepted') {
		return array('success' => FALSE, 'msg' => "Dispatch status is $status");
	}

	if ($rollover_employer_nid !== NULL) {
		// Get and verify the rollover target.
		if (!$rollover_employer_nid) { return array('success' => FALSE, 'msg' => 'Please select a rollover employer.'); }
		$rollover_employer_node = node_load($rollover_employer_nid);
		if (!$rollover_employer_node) { return array('success' => FALSE, 'msg' => "Unable to load node $rollover_employer_nid"); }
		if ($rollover_employer_node->type != 'grievance_shop') { return array('success' => FALSE, 'msg' => "Node $rollover_employer_nid is not a employer. " . $rollover_employer_node->title . ' is a ' . $rollover_employer_node->type); }
	}

	return array('success' => TRUE, 'msg' => 'Possible');
}

function sirius_dispatch_rollover($dispatch_node, $rollover_employer_nid, $rollover_type = 'permanent', $end_date = NULL, $notes = '') {
	$possible = sirius_dispatch_rollover_possible($dispatch_node, $rollover_employer_nid, $rollover_type, $end_date);
	if (!$possible['success']) {
		sirius_log('dispatch:rollover', 'fail', "Rollover failed - " . $possible['msg'], '', '', array($dispatch_node->nid));
		return $possible;
	}

	$rollover_employer_node = node_load($rollover_employer_nid);

	// For permanent rollovers, set the dispatch status to "terminated" and notify the worker
	if ($rollover_type == 'permanent') {
		
		// Set the "end" timestamp. 
		if (!$dispatch_node->field_sirius_date_end['und'][0]['value']) {
			$dispatch_node->field_sirius_date_end['und'][0]['value'] = date('Y-m-d');
		}

		$dispatch_node->field_sirius_dispatch_status['und'][0]['value'] = 'terminated';
		node_save($dispatch_node);

		// Notify the worker.
		$worker_node = node_load($dispatch_node->field_sirius_worker['und'][0]['target_id']);
		$unique_id = "UNIQUE/rollover/" . $dispatch_node->nid;
		global $base_url;
		$url = "$base_url/d/" . $dispatch_node->nid . "/" . $dispatch_node->field_sirius_aat['und'][0]['value'];

		$subject = "Dispatch system notification: Dispatch Rollover to " . $rollover_employer_node->title;
		$body = "Your dispatch has been laid off, and a rollover offer has been sent tp " . $rollover_employer_node->title . ". <br /><br />";
		$body .= "<a href=\"$url\">" . $dispatch_node->title . '</a>';
		if ($notes) {
			$body .= "<br /><br />";
			$body .= $notes;
		}

		$sms = "Dispatch system notification: Dispatch Rollover\n";
		$sms .= "Dispatch laid off: " . $dispatch_node->title . "\n";
		$sms .= "Rollover offer to: " . $rollover_employer_node->title . "\n";
		$sms .= "URL: " . $url;
		if ($notes) {
			$sms .= "\n\n";
			$sms .= $notes;
		}

		$result = sirius_worker_notify(
			$worker_node, 
			array(
				'subject' => $subject,
				'body' => $body,
				'sms' => $sms,
				'X-Sirius-Id' => $unique_id,
			)
		);
	}

	// Log the rollover. This is what enables the worker to be requested by the target employer.
	$msg = "Rollover ($rollover_type) to " . $rollover_employer_node->title;
	if ($end_date) {
		$msg .= " ending on $end_date";
	}
	sirius_log(
		'dispatch:rollover', 
		'complete', 
		$msg, 
		$notes, 
		'', 
		array(
			$dispatch_node->nid, 
			$rollover_employer_nid, 
			$dispatch_node->field_sirius_worker['und'][0]['target_id']
		),
		NULL,
		json_encode(array('end_date' => $end_date))
	);

	return array('success' => TRUE, 'msg' => 'Rollover');
}
