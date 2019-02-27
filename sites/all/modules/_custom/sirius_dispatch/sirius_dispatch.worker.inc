<?php

function sirius_dispatch_worker_recompute_from_dispatch($dispatch_node, $is_delete = FALSE) {
	$worker_nid = $dispatch_node->field_sirius_worker['und'][0]['target_id'];
	if (!$worker_nid) { return; }
	$worker_node = node_load($worker_nid);
	if (!$worker_node) { return; }

	// Fetch from the database
	$sql = "select field_sirius_dispatch_status_value as status, ";
	$sql .= "field_data_field_sirius_dispatch_status.entity_id as dispatch_nid, ";
	$sql .= "field_data_field_sirius_dispatch_job.field_sirius_dispatch_job_target_id as job_nid ";
	$sql .= "from field_data_field_sirius_worker, field_data_field_sirius_dispatch_status, field_data_field_sirius_dispatch_job ";

	$sql .= "where field_data_field_sirius_worker.entity_type = 'node' ";
	$sql .= "and field_data_field_sirius_dispatch_status.entity_type = 'node' ";
	$sql .= "and field_data_field_sirius_dispatch_job.entity_type = 'node' ";

	$sql .= "and field_data_field_sirius_worker.entity_id = field_data_field_sirius_dispatch_status.entity_id ";
	$sql .= "and field_data_field_sirius_worker.entity_id = field_data_field_sirius_dispatch_job.entity_id ";

	$sql .= "and field_data_field_sirius_worker.field_sirius_worker_target_id = :worker_nid ";

	$sql .= "order by field_data_field_sirius_worker.entity_id";
	$stmt = db_query($sql, array(':worker_nid' => $worker_nid));
	$dispatches = array();
	while ($hr = $stmt->fetchAssoc()) {
		$dispatches[$hr['dispatch_nid']] = $hr;
	}

	// Update with the current node, since it might not be written to the database yet
	if ($is_delete) {
		unset($dispatches[$dispatch_node->nid]);
	} else {
		$dispatches[$dispatch_node->nid] = array(
			'job_nid' => $dispatch_node->field_sirius_dispatch_job['und'][0]['target_id'],
			'status' => $dispatch_node->field_sirius_dispatch_status['und'][0]['value'],
		);
	}

	// Set the worker's employer
	$new_employer_nid = NULL;
	$new_dispatch_nid = NULL;
	foreach ($dispatches as $dispatch_nid => $hr) {
		if ($hr['status'] == 'accepted') {
			$job_node = node_load($dispatch_node->field_sirius_dispatch_job['und'][0]['target_id']);
			$new_employer_nid = $job_node->field_grievance_shop['und'][0]['target_id'];
			$new_dispatch_nid = $dispatch_nid;
		}
	}

	if ($new_employer_nid != $worker_node->field_grievance_shop['und'][0]['target_id']) {
		if ($new_employer_nid) {
			$new_employer_node = node_load($new_employer_nid);
			$worker_node->field_grievance_shop['und'][0]['target_id'] = $new_employer_nid;
			$dispstatus_tid = sirius_get_special_tid('sirius_worker_dispatch_status|employed_not_available');

			$category = 'worker:employer';
			$type = 'set';
			$message = $worker_node->title . "'s employer has been set to " . $new_employer_node->title;
			$handler_nids = array($worker_node->nid);
			sirius_log($category, $type, $message, $summary, '', $handler_nids);
		} else {
			unset($worker_node->field_grievance_shop['und']);

			$asi = $worker_node->field_sirius_dispatch_asi['und'][0]['value'];
			if ($asi == 'Yes') {
				$dispstatus_tid = sirius_get_special_tid('sirius_worker_dispatch_status|not_employed_available');
			} else {
				$dispstatus_tid = sirius_get_special_tid('sirius_worker_dispatch_status|not_employed_not_available');
			}
			$category = 'worker:employer';
			$type = 'clear';
			$message = $worker_node->title . "'s employer has been cleared";
			$handler_nids = array($worker_node->nid);
			sirius_log($category, $type, $message, $summary, '', $handler_nids);
		}

		node_save($worker_node);
		sirius_dispatch_worker_dispstatus_set($worker_node, $dispstatus_tid, NULL, NULL);
	}
}

function sirius_dispatch_worker_dispstatus_set($worker_node, $dispstatus_tid, $reason = NULL, $attachments = NULL) {
	if ($dispstatus_tid == $worker_node->field_sirius_worker_dispstatus['und'][0]['tid']) { return; }

	$worker_node->field_sirius_worker_dispstatus['und'][0]['tid'] = $dispstatus_tid;
	$dispstatus_term = taxonomy_term_load($dispstatus_tid);

	if ($dispstatus_tid == sirius_get_special_tid('sirius_worker_dispatch_status|not_employed_available')) {
		$availdate = date('Y-m-d H:i:s');
		$worker_node->field_sirius_dispatch_availdate['und'][0]['value'] = $availdate;
		$category = 'worker:availdate';
		$type = 'set';
		$message = $worker_node->title . "'s availability date has been set to " . $availdate;
		$handler_nids = array($worker_node->nid);
		sirius_log($category, $type, $message, $message, '', $handler_nids);
	}

	$category = 'worker:dispstatus';
	$type = 'set';
	$message = $worker_node->title . "'s dispatch status has been set to " . $dispstatus_term->name;
	$handler_nids = array($worker_node->nid);
	sirius_log($category, $type, $message, $reason, '', $handler_nids, $attachments);

	node_save($worker_node);
	return $message;
}
