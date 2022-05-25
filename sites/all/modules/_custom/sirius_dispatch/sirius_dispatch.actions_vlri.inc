<?php

function sirius_dispatch_vlri_possible($dispatch_node) {
	// Must be enabled
	if (!sirius_dispatch_vlri_enabled()) {
		return array('success' => FALSE, 'msg' => "VLRI is not enabled.");
	}

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

function sirius_dispatch_vlri($dispatch_node, $notes = '') {
	$possible = sirius_dispatch_vlri_possible($dispatch_node);
	if (!$possible['success']) {
		sirius_log('dispatch:vlri', 'fail', "VLRI failed - " . $possible['msg'], '', '', array($dispatch_node->nid));
		return $possible;
	}

	// Set the dispatch status to "resigned", save and log
	$dispatch_node->field_sirius_dispatch_status['und'][0]['value'] = 'vlri';
	node_save($dispatch_node);
	sirius_log('dispatch:vlri', 'pending', 'Voluntary layoff, requests that it be converted to involuntary.', $notes, '', array($dispatch_node->nid));

	return array('success' => TRUE, 'msg' => 'Completed a voluntary layoff with a request to convert to involuntary.');
}

function sirius_dispatch_vlri_resolve_possible($dispatch_node) {
	// Must be enabled
	if (!sirius_dispatch_vlri_enabled()) {
		return array('success' => FALSE, 'msg' => "VLRI is not enabled.");
	}

	// Verify the dispatch status
	$status = $dispatch_node->field_sirius_dispatch_status['und'][0]['value'];
	if ($status != 'vlri') {
		return array('success' => FALSE, 'msg' => "Dispatch status is $status");
	}

	return array('success' => TRUE, 'msg' => 'Possible');
}

function sirius_dispatch_vlri_resolve($dispatch_node, $notes, $resolution) {
	$possible = sirius_dispatch_vlri_resolve_possible($dispatch_node);
	if (!$possible['success']) { return $possible; }

	if ($resolution != 'accept' && $resolution != 'reject') {
		return array('success' => FALSE, 'msg' => 'Please indicate whether the layoff request should be accepted or rejected.');
	}

	$job_node = node_load($dispatch_node->field_sirius_dispatch_job['und'][0]['target_id']);
	$employer_nid = $job_node->field_grievance_shop['und'][0]['target_id'];
	$worker_nid = $dispatch_node->field_sirius_worker['und'][0]['target_id'];
	$worker_node = node_load($worker_nid);

	// Set the dispatch status, save and log
	if ($resolution == 'accept') {
		$dispatch_node->field_sirius_dispatch_status['und'][0]['value'] = 'terminated';
		$msg = "The layoff you reported for " . $employer_node->title . " has been accepted, and your layoff is now considered involuntary.";
	} else {
		$dispatch_node->field_sirius_dispatch_status['und'][0]['value'] = 'resigned';
		$msg = "The layoff you reported for " . $employer_node->title . " has been denied, and your layoff is now considered voluntary. You may dispute this by providing the Dispatch Office a copy of your layoff slip from the employer.";
	}
	node_save($dispatch_node);
	sirius_log('dispatch:vlri', $resolution, $msg, $notes, '', array($dispatch_node->nid, $worker_nid, $employer_nid));

	// Notify the worker.
	if (variable_get('sirius_dispatch_vlri_worker_notify', 'Yes') == 'Yes') {
		$unique_id = "UNIQUE/vlri/" . $dispatch_node->nid;
		global $base_url;
		$url = "$base_url/d/" . $dispatch_node->nid . "/" . $dispatch_node->field_sirius_aat['und'][0]['value'];

		$subject = "Dispatch system notification: Layoff Request";
		$body = $msg;
		$body .= "<br /><br />";
		$body .= "<a href=\"$url\">" . $dispatch_node->title . '</a>';
		if ($notes) {
			$body .= "<br /><br />";
			$body .= $notes;
		}

		$sms = "Dispatch system notification: Layoff Request\n";
		$sms .= "Dispatch: " . $dispatch_node->title . "\n";
		$sms .= $msg . "\n";
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

	return array('success' => TRUE, 'msg' => $msg);
}

function sirius_dispatch_page_employer_vlri() {
	global $user;

	drupal_set_title(t('Layoff Requests'));

	$user_full = user_load($user->uid);
	$shop_nid = $user_full->field_grievance_shops['und'][0]['target_id'];
	if (!$shop_nid) {
		return t('No employer found. This probably means that the user has been misconfigured, and has not been assigned to a shop. Please contact the system administrator. (No field_grievance_shops).');
	}
	return views_embed_view('sirius_dispatch_vlri', 'block_1', $shop_nid);
}

function sirius_dispatch_vlri_block_check($employer_nid) {
	if (!sirius_dispatch_vlri_enabled()) {
		return array('success' => TRUE, 'msg' => "VLRI is not enabled.");
	}

	$window = variable_get('sirius_dispatch_vlri_block_window', 24*60*60);
	$cutoff = time() - $window;
	$limit = variable_get('sirius_dispatch_vlri_block_limit', 1);
	$views_result = views_get_view_result('sirius_dispatch_vlri', 'block_2', $employer_nid);
	if (!$views_result) { return array('success' => TRUE, 'msg' => 'No pending layoff requests found.'); }

	$blocks = array();
	foreach ($views_result as $obj) {
		$ts = $obj->field_sirius_log_handler_node_created;
		if ($ts <= $cutoff) {
			$blocks[$obj->nid]++;
		}
	}

	$count = count($blocks);
	if ($count < $limit) {
		return array('success' => TRUE, 'msg' => "There are $count layoff requests that are at least " . round(($window/(60*60))) . " hours old.");
	}

	return array('success' => FALSE, 'msg' => "There are $count layoff requests that are at least " . round(($window/(60*60))) . " hours old.");
}

function sirius_dispatch_vlri_admin() {
	$form['sirius_dispatch_vlri_enable'] = array(
		'#type' => 'select',
		'#title' => 'Enable VLRI?',
		'#options' => array('no' => 'No', 'yes' => 'Yes'),
		'#default_value' => variable_get('sirius_dispatch_vlri_enable', 'no'),
	);

	$form['sirius_dispatch_vlri_block_window'] = array(
		'#type' => 'textfield',
		'#size' => 5,
		'#title' => 'Window (in seconds)',
		'#description' => 'Number of seconds of "grace period" before an employer must resolve a VLRI.',
		'#default_value' => variable_get('sirius_dispatch_vlri_block_window', 24*60*60),
	);

	$form['sirius_dispatch_vlri_block_limit'] = array(
		'#type' => 'textfield',
		'#size' => 5,
		'#title' => 'Limit',
		'#description' => 'Number of pending VLRI requests (older than the grace period) before an employer is blocked from creating new jobs.',
		'#default_value' => variable_get('sirius_dispatch_vlri_block_limit', 1),
	);

	$form['sirius_dispatch_vlri_worker_notify'] = array(
		'#title' => t('Notify worker?'),
		'#type' => 'select',
		'#options' => array(
			'No' => 'No', 
			'Yes' => 'Yes'
		),
		'#default_value' => variable_get('sirius_dispatch_vlri_worker_notify', 'Yes'),
		'#description' => t('Should the worker receive a notification when the VLRI request is resolved?'),
	);

  return system_settings_form($form);
}

function sirius_dispatch_vlri_enabled() { 
	return variable_get('sirius_dispatch_vlri_enable', 'no') == 'yes';
}