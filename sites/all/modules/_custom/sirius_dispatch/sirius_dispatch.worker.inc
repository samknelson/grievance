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
	$sql .= "from field_data_field_sirius_worker, field_data_field_sirius_dispatch_status, field_data_field_sirius_dispatch_job, node ";

	$sql .= "where field_data_field_sirius_worker.entity_type = 'node' ";
	$sql .= "and field_data_field_sirius_dispatch_status.entity_type = 'node' ";
	$sql .= "and field_data_field_sirius_dispatch_job.entity_type = 'node' ";

	$sql .= "and field_data_field_sirius_worker.entity_id = field_data_field_sirius_dispatch_status.entity_id ";
	$sql .= "and field_data_field_sirius_worker.entity_id = field_data_field_sirius_dispatch_job.entity_id ";

	$sql .= "and field_data_field_sirius_worker.field_sirius_worker_target_id = :worker_nid ";

	$sql .= "and node.nid = field_data_field_sirius_worker.entity_id ";
	$sql .= "order by node.created ";

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
	$accepted_dispatch_nids = array();
	foreach ($dispatches as $dispatch_nid => $hr) {
		if ($hr['status'] == 'accepted') {
			$accepted_dispatch_nids[] = $dispatch_nid;
			$job_node = node_load($hr['job_nid']);
			$new_employer_nid = $job_node->field_grievance_shop['und'][0]['target_id'];
			$new_dispatch_nid = $dispatch_nid;
		}
	}

	// Terminate any old dispatches. 
	// @todo - This is some pretty ugly code here.
	if (count($accepted_dispatch_nids) > 1) {
		$sql = "update field_data_field_sirius_dispatch_status set field_sirius_dispatch_status_value = 'terminated' ";
		$sql .= "where field_sirius_dispatch_status_value = 'accepted' ";
		$sql .= "and entity_type = 'node' ";
		$sql .= "and bundle = 'sirius_dispatch' ";
		$sql .= "and entity_id != :new_dispatch_nid ";
		$sql .= "and entity_id in (:accepted_dispatch_nids) ";

		$stmt = db_query($sql, array(':new_dispatch_nid' => $new_dispatch_nid, ':accepted_dispatch_nids' => $accepted_dispatch_nids));

		$sql = "update field_revision_field_sirius_dispatch_status set field_sirius_dispatch_status_value = 'terminated' ";
		$sql .= "where field_sirius_dispatch_status_value = 'accepted' ";
		$sql .= "and entity_type = 'node' ";
		$sql .= "and bundle = 'sirius_dispatch' ";
		$sql .= "and entity_id != :new_dispatch_nid ";
		$sql .= "and entity_id in (:accepted_dispatch_nids) ";

		$stmt = db_query($sql, array(':new_dispatch_nid' => $new_dispatch_nid, ':accepted_dispatch_nids' => $accepted_dispatch_nids));
	}

	if ($new_employer_nid != $worker_node->field_grievance_shop['und'][0]['target_id']) {
		if ($new_employer_nid) {
			$new_employer_node = node_load($new_employer_nid);
			$worker_node->field_grievance_shop['und'][0]['target_id'] = $new_employer_nid;
			$dispstatus_tid = sirius_get_special_tid('sirius_worker_dispatch_status|employed_not_available');

			sirius_log('worker:employer', 'set', $worker_node->title . "'s employer has been set to " . $new_employer_node->title, '', '', array($worker_node->nid));
		} else {
			unset($worker_node->field_grievance_shop['und']);

			if ($worker_node->field_sirius_worker_dispstatus['und'][0]['tid'] == sirius_get_special_tid('sirius_worker_dispatch_status|employed_not_available')) {
				$asi = $worker_node->field_sirius_dispatch_asi['und'][0]['value'];
				if ($asi == 'Yes') {
					$dispstatus_tid = sirius_get_special_tid('sirius_worker_dispatch_status|not_employed_available');
				} else {
					$dispstatus_tid = sirius_get_special_tid('sirius_worker_dispatch_status|not_employed_not_available');
				}
			}
			sirius_log('worker:employer', 'clear', $worker_node->title . "'s employer has been cleared" . $new_employer_node->title, '', '', array($worker_node->nid));
		}

		node_save($worker_node);
		if ($dispstatus_tid) {
			sirius_dispatch_worker_dispstatus_set($worker_node, $dispstatus_tid, 1);
		}
	}

	// If 
	// - The worker is currently EBA
	// - They have any active_secondary dispatches
	// - Any of those dispatches have dates that are today or later
	// then the worker is no longer EBA.
	if ($worker_node->field_sirius_dispatch_eba['und'][0]['value'] == 'Yes') {
		foreach ($dispatches as $dispatch_nid => $hr) {
			if ($hr['status'] != 'accepted_secondary') { continue; }
			$job_node = node_load($hr['job_nid']);

			$clear_eba = FALSE;
			$date = date('Y-m-d');
			if ($job_node->field_sirius_dispatch_eba_dates['und']) {
				foreach ($job_node->field_sirius_dispatch_eba_dates['und'] as $tmp) {
					if (date('Y-m-d', strtotime($tmp['value'])) >= $date) {
						$clear_eba = TRUE;
						$date = date('Y-m-d', strtotime($tmp['value']));
					}
				}
			}

			if ($clear_eba) {
				sirius_log('worker:eba', 'set', "Because the worker is on an EBA job until $date, EBA status has been set to 'No'.", '', '', array($worker_node->nid), array());
				sirius_log('worker:eba_dates', 'clear', "Because the worker is on an EBA job until $date, EBA dates have been cleared.", '', '', array($worker_node->nid), array());
				$worker_node->field_sirius_dispatch_eba_dates['und'] = array();
				$worker_node->field_sirius_dispatch_eba['und'][0]['value'] = 'No';
				node_save($worker_node);
			}
		}
	}
}

function sirius_dispatch_worker_recompute_from_work_status($worker_node) {
	$old_tid = $worker_node->original->field_sirius_work_status['und'][0]['tid'];
	$new_tid = $worker_node->field_sirius_work_status['und'][0]['tid'];

	// No change
	if ($old_tid == $new_tid) { return; }

	// We don't care unless we're currently available
	if ($worker_node->field_sirius_worker_dispstatus['und'][0]['tid'] != sirius_get_special_tid('sirius_worker_dispatch_status|not_employed_available')) { return; }

	// We only care if the new status is non-dispatchable.
	$new_term = taxonomy_term_load($new_tid); 
	if ($new_term->field_sirius_dispatch_available['und'][0]['value'] == 'Yes') { return; }

	// Sign out.
	sirius_dispatch_worker_dispstatus_set($worker_node, sirius_get_special_tid('sirius_worker_dispatch_status|not_employed_not_available'), NULL, "Work status was changed to " . $new_term->name);
}