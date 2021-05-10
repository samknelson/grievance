<?php

function sirius_dispatch_worker_dispstatus_set_possible($worker_node, $dispstatus_tid, $account = NULL) {
	// Load the user who is performing this action
	global $user;
	if (!$account) { $account = $user; }
	if (is_numeric($account)) { $account = user_load($account); }

	// Load the dispatch status term
	$dispstatus_term = taxonomy_term_load($dispstatus_tid);
	if (!$dispstatus_term) {
		return array('success' => FALSE, 'msg' => 'System error: no taxonomy term.');
	}

	// If we have an employer, can't sign out or in
	if ($worker_node->field_grievance_shop['und'][0]['target_id'] && 
		($dispstatus_tid == sirius_get_special_tid('sirius_worker_dispatch_status|not_employed_available') || $dispstatus_tid == sirius_get_special_tid('sirius_worker_dispatch_status|not_employed_not_available'))) {
		return array('success' => FALSE, 'msg' => 'The worker has an employer set, so it is not possible to sign in or out of the out-of-work list.');
	}

	// Can't sign in if we have a sign-in block
	$sib_term = taxonomy_term_load($worker_node->field_sirius_dispatch_sib_status['und'][0]['tid']);
	if ($sib_term && 
		$sib_term->field_sirius_dispatch_available['und'][0]['value'] == 'No' && 
		$dispstatus_tid == sirius_get_special_tid('sirius_worker_dispatch_status|not_employed_available') && 
		!user_access('sirius dispatch staff', $account)) {
		return array('success' => FALSE, 'msg' => 'The administrator has placed a sign-in block on this worker.');
	}

	// Can't sign in if we don't have any active worker status
	if ($dispstatus_tid == sirius_get_special_tid('sirius_worker_dispatch_status|not_employed_available')) {
		$work_status_term = taxonomy_term_load($worker_node->field_sirius_work_status['und'][0]['tid']);
		if ($work_status_term->field_sirius_dispatch_available['und'][0]['value'] != 'Yes') {
			return array('success' => FALSE, 'msg' => 'The work status ' . $work_status_term->name . ' is not available for dispatch.');
		}
	}

	// Do we have permission on this particular worker?
	$can_access = FALSE;
	if (user_access('sirius dispatch staff', $account)) { $can_access = TRUE; }
	if (user_access('sirius dispatch signin', $account)) {
		if (sirius_worker_get_uid($worker_node->nid) == $account->uid) {
			$can_access = TRUE;
		}
	}
	if (!$can_access) {
		return array('success' => FALSE, 'msg' => 'This user does not have permission to change the dispatch status.');
	}

	// We passed all the tests.
	return array('success' => TRUE);
}

function sirius_dispatch_worker_dispstatus_set(&$worker_node, $dispstatus_tid = NULL, $account = NULL, $reason = NULL, $attachments = NULL) {
	global $user;
	if (!$account) { $account = $user; }
	if (is_numeric($account)) { $account = user_load($account); }

	// If we're not given a status, use the worker's auto-sign-in settings.
	if (!$dispstatus_tid) {
		$asi = $worker_node->field_sirius_dispatch_asi['und'][0]['value'];
		if ($asi == 'Yes') {
			$dispstatus_tid = sirius_get_special_tid('sirius_worker_dispatch_status|not_employed_available');
		} else {
			$dispstatus_tid = sirius_get_special_tid('sirius_worker_dispatch_status|not_employed_not_available');
		}
	}

	// If we're already there, do nothing.
	$orig_dispstatus_tid = $worker_node->field_sirius_worker_dispstatus['und'][0]['tid'];
	if ($dispstatus_tid == $orig_dispstatus_tid) { return array('success' => TRUE, 'msg' => 'The dispatch status is unchanged.'); }

	// Find out if it's possible
	$possible = sirius_dispatch_worker_dispstatus_set_possible($worker_node, $dispstatus_tid, $account);

	// It might be impossible to sign in -- i.e. we're sign-in blocked, but we just terminated a dispatch. In that
	// case, log it and set the status to signed out.
	if (!$possible['success'] && $dispstatus_tid == sirius_get_special_tid('sirius_worker_dispatch_status|not_employed_available')) {
		$dispstatus_term = taxonomy_term_load($dispstatus_tid);
		sirius_log('worker:dispstatus', 'error', "Unable to set " . $worker_node->title . "'s dispatch status to " . $dispstatus_term->name . ": " . $possible['msg'], $reason, '', array($worker_node->nid), $attachments);
		return sirius_dispatch_worker_dispstatus_set($worker_node, sirius_get_special_tid('sirius_worker_dispatch_status|not_employed_not_available'), $override_blocks, $reason, $attachments);
	}

	// Assign
	$worker_node->field_sirius_worker_dispstatus['und'][0]['tid'] = $dispstatus_tid;

	// Load the term
	$dispstatus_term = taxonomy_term_load($dispstatus_tid);

	// If we're signing in, set the availability date
	if ($dispstatus_tid == sirius_get_special_tid('sirius_worker_dispatch_status|not_employed_available')) {

		$json = json_decode($worker_node->field_sirius_json['und'][0]['value'], TRUE);
		if ($json['dispatch']['keep_seniority'] == 'Yes') {
			$json['dispatch']['keep_seniority'] = 'No';
			$worker_node->field_sirius_json['und'][0]['value'] = json_encode($json);
			$availdate = sirius_date_date($worker_node, 'field_sirius_dispatch_availdate', $format = 'Y-m-d H:i:s');

			$msg = $worker_node->title . "'s last dispatch was laid off under 24 hours, keeping previous availability date of $availdate.";
			$handler_nids = array($worker_node->nid);
			sirius_log('worker:availdate', 'keep', $msg, $reason, '', $handler_nids);
		} else {
			$availdate = gmdate('Y-m-d H:i:s');
			$worker_node->field_sirius_dispatch_availdate['und'][0]['value'] = $availdate;

			$msg = $worker_node->title . "'s availability date has been set to " . $availdate;
			$handler_nids = array($worker_node->nid);
			sirius_log('worker:availdate', 'set', $msg, $reason, '', $handler_nids);
		}
	}

	// Log the change
	$category = 'worker:dispstatus';
	$type = 'set';
	$msg = $worker_node->title . "'s dispatch status has been set to " . $dispstatus_term->name;
	$handler_nids = array($worker_node->nid);
	sirius_log($category, $type, $msg, $reason, '', $handler_nids, $attachments);

	// Notify the worker
	if ($orig_dispstatus_tid != sirius_get_special_tid('sirius_worker_dispatch_status|employed')) {
		if ($dispstatus_tid == sirius_get_special_tid('sirius_worker_dispatch_status|not_employed_available') ||
			$dispstatus_tid == sirius_get_special_tid('sirius_worker_dispatch_status|not_employed_not_available')) {

			if ($dispstatus_tid == sirius_get_special_tid('sirius_worker_dispatch_status|not_employed_available')) {
				$availdate = date('Y-m-d H:i:s', sirius_date_ts($worker_node, 'field_sirius_dispatch_availdate'));
				$subject = "Dispatch system notification: Signed in";
				$body = "You have been signed in to the out-of-work list, and are now eligible to receive dispatches. Your seniority date is $availdate.";
				$sms = "Dispatch system notification: you have been signed in to the out-of-work list, and are now eligible to receive dispatches.  Your seniority date is $availdate.";
			} else {
				$subject = "Dispatch system notification: Signed out";
				$body = "You have been signed out of the out-of-work list, and are not currently eligible to receive dispatches.";
				$sms = "Dispatch system notification: you have been signed out of the out-of-work list, and are not currently eligible to receive dispatches.";
			}
			sirius_worker_notify(
				$worker_node, 
				array(
					'subject' => $subject,
					'body' => $body,
					'sms' => $sms,
				)
			);
		}
	}

	// Save the worker
	node_save($worker_node);

	return array('success' => TRUE, 'msg' => $msg);
}

function sirius_dispatch_worker_dispstatus_block_set(&$worker_node, $block_tid, $reason = NULL, $attachments = NULL) {
	if ($block_tid == $worker_node->field_sirius_dispatch_sib_status['und'][0]['tid']) { 
		return array('success' => TRUE, 'msg' => 'Block status unchanged.');
	}

	$json = array();
	if ($block_tid) {
		$worker_node->field_sirius_dispatch_sib_status['und'][0]['tid'] = $block_tid;
		$block_term = taxonomy_term_load($block_tid);
		if ($block_term->field_sirius_dispatch_available['und'][0]['value'] == 'No') {
			$worker_node->field_sirius_dispatch_asi['und'][0]['value'] = 'No';
		}
		$category = 'worker:dispstatus_block';
		$type = 'set';
		$name = $block_term->name;
		$display_name = $block_term->field_sirius_name_display['und'][0]['value'];
		if (!$display_name) { $display_name = $name; }
		$msg = $worker_node->title . "'s dispatch status block has been set to " . $display_name;
		$json['block_display_name'] = $display_name;
		$json['block_real_name'] = $name;
		$json['block_tid'] = $block_tid;
		$handler_nids = array($worker_node->nid);
	} else {
		unset($worker_node->field_sirius_dispatch_sib_status['und']);
		$category = 'worker:dispstatus_block';
		$type = 'clear';
		$msg = $worker_node->title . "'s dispatch status block has been cleared.";
		$handler_nids = array($worker_node->nid);
	}
	node_save($worker_node);
	sirius_log($category, $type, $msg, $reason, '', $handler_nids, $attachments, $json);
	return array('success' => TRUE, 'msg' => $msg);
}

