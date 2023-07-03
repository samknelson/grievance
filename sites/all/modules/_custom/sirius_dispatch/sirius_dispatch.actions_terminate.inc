<?php

function sirius_dispatch_terminate_possible($dispatch_node) {
	// Verify the dispatch status
	$status = $dispatch_node->field_sirius_dispatch_status['und'][0]['value'];
	if ($status != 'accepted' && $status != 'accepted_secondary') {
		return array('success' => FALSE, 'msg' => "Dispatch status is $status");
	}

	return array('success' => TRUE, 'msg' => 'Possible');
}

function sirius_dispatch_terminate($dispatch_node, $notes = '') {
	$possible = sirius_dispatch_terminate_possible($dispatch_node);
	if (!$possible['success']) {
		sirius_log('dispatch:terminate', 'fail', "Layoff failed - " . $possible['msg'], '', '', array($dispatch_node->nid));
		return $possible;
	}

	$worker_node = node_load($dispatch_node->field_sirius_worker['und'][0]['target_id']);
	$job_node = node_load($dispatch_node->field_sirius_dispatch_job['und'][0]['target_id']);

	// Terminations of "Accepted" dispatches after less than 24 hours keep their seniority.
	if ($dispatch_node->field_sirius_dispatch_status['und'][0]['value'] == 'accepted') {
		$report_ts = sirius_date_ts($job_node, 'field_sirius_datetime');
		$duration = time() - $report_ts;
		if ($duration < (24*60*60)) { 
			$json = json_decode($worker_node->field_sirius_json['und'][0]['value'], TRUE);
			$json['dispatch']['keep_seniority'] = 'Yes';
			$worker_node->field_sirius_json['und'][0]['value'] = json_encode($json);
			node_save($worker_node);
		}
	}

	// Set the "end" timestamp. 
	if (!$dispatch_node->field_sirius_date_end['und'][0]['value']) {
		$dispatch_node->field_sirius_date_end['und'][0]['value'] = date('Y-m-d');
	}

	// Set the dispatch status to "terminated", save and log
	$dispatch_node->field_sirius_dispatch_status['und'][0]['value'] = 'terminated';
	node_save($dispatch_node);
	sirius_log('dispatch:terminate', 'complete', "Layoff complete", $notes, '', array($dispatch_node->nid));

	// Notify the worker.
	$unique_id = "UNIQUE/terminate/" . $dispatch_node->nid;
	global $base_url;
	$url = "$base_url/d/" . $dispatch_node->nid . "/" . $dispatch_node->field_sirius_aat['und'][0]['value'];

	$subject = "Dispatch system notification: Dispatch Layoff";
	$body = "Your dispatch has been terminated. <br /><br />";
	$body .= "<a href=\"$url\">" . $dispatch_node->title . '</a>';
	if ($notes) {
		$body .= "<br /><br />";
		$body .= $notes;
	}

	$sms = "Dispatch system notification: Dispatch Layoff\n";
	$sms .= "Dispatch: " . $dispatch_node->title . "\n";
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

	return array('success' => TRUE, 'msg' => 'Layoff');
}
