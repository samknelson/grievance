<?php

function sirius_dispatch_worker_recompute_from_dispatch($dispatch_node, $is_delete = FALSE) {
	// Accepting one dispatch could potentially terminate another.
	// Just in case, let's make sure we only recompute once for each dispatch.
	static $no_infinite_loops_please;
	if (!isset($no_infinite_loops_please)) { $no_infinite_loops_please = array(); }
	if ($no_infinite_loops_please[$dispatch_node->nid]) { return; }
	$no_infinite_loops_please[$dispatch_node->nid] = TRUE;

	// Load the worker
	$worker_nid = $dispatch_node->field_sirius_worker['und'][0]['target_id'];
	if (!$worker_nid) { return; }
	$worker_node = node_load($worker_nid);
	if (!$worker_node) { return; }

  // Switch into the domain
  sirius_domain_push($dispatch_node->field_sirius_domain['und'][0]['target_id']);

	// Fetch all the "accepted" and "accepted_secondary" dispatches for this worker, in chronological order
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
	$sql .= "and field_sirius_dispatch_status_value in ('accepted', 'accepted_secondary') ";
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
			'dispatch_nid' => $dispatch_node->nid,
			'job_nid' => $dispatch_node->field_sirius_dispatch_job['und'][0]['target_id'],
			'status' => $dispatch_node->field_sirius_dispatch_status['und'][0]['value'],
		);
	}

	// Fetch the job policy for each dispatch node. We need this to determine whether to sign out and in
	foreach ($dispatches as $dispatch_nid => $hr) {
		$job_node = node_load($hr['job_nid']);
		$job_json = sirius_json_get($job_node);
		$dispatches[$dispatch_nid]['job_node'] = node_load($hr['job_nid']);
		$dispatches[$dispatch_nid]['job_json'] = sirius_json_get($dispatches[$dispatch_nid]['job_node']);
		$dispatches[$dispatch_nid]['job_type_term'] = taxonomy_term_load(
			$dispatches[$dispatch_nid]['job_node']->field_sirius_dispatch_job_type['und'][0]['tid']
		);
		$dispatches[$dispatch_nid]['job_type_json'] = sirius_json_get($dispatches[$dispatch_nid]['job_type_term']);
	}

	// Sort by report date
	uksort($dispatches, function ($a, $b) use ($dispatches) {
		return strcmp(
			$dispatches[$a]['job_node']->field_sirius_datetime['und'][0]['value'],
			$dispatches[$b]['job_node']->field_sirius_datetime['und'][0]['value']
		);
	});

	// Any accepted primary dispatch sets the worker's employer.
	$primary_employer_nid = NULL;
	$primary_dispatch_nid = NULL;
	foreach ($dispatches as $dispatch_nid => $hr) {
		if ($hr['status'] != 'accepted') { continue; }
		$primary_employer_nid = $hr['job_node']->field_grievance_shop['und'][0]['target_id'];
		$primary_dispatch_nid = $dispatch_nid;
	}
	if ($primary_employer_nid != $worker_node->field_grievance_shop['und'][0]['target_id']) {
		if ($primary_employer_nid) {
			$primary_employer_node = node_load($primary_employer_nid);
			$worker_node->field_grievance_shop['und'][0]['target_id'] = $primary_employer_nid;

			if (sirius_domain_component_enabled('dispatch:live')) {
				sirius_log('worker:employer', 'set', $worker_node->title . "'s employer has been set to " . $primary_employer_node->title, '', '', array($worker_node->nid));
			}
		} else {
			unset($worker_node->field_grievance_shop['und']);
			if (sirius_domain_component_enabled('dispatch:live')) {
				sirius_log('worker:employer', 'clear', $worker_node->title . "'s employer has been cleared from " . $primary_employer_node->title, '', '', array($worker_node->nid));
			}
		}
		node_save($worker_node);
	}

	// Sign out or in:
	// - Iterate through all (accepted, accepted_secondary) dispatches
	// - If ANY has "set status to employed" = ['run']['dispstatus']['primary|secondary'] = 'Yes', then dispatch status = 'Employed'
	// - Otherwise, dispatch status = not employed (with ASI)

	$old_dispstatus_tid = $worker_node->field_sirius_worker_dispstatus['und'][0]['tid'];
	foreach ($dispatches as $dispatch_nid => $hr) {
		$s = $hr['status'];
		$j = '';
		if ($s == 'accepted') {
			$j = $hr['job_type_json']['run']['dispstatus']['primary'];
			if (!$j) { $j = 'Yes'; }
		} else if ($s == 'accepted_secondary') {
			$j = $hr['job_type_json']['run']['dispstatus']['secondary'];
			if (!$j) { $j = 'No'; }
		}

		if ($j == 'Yes') {
			$dispstatus_tid = sirius_get_special_tid('sirius_worker_dispatch_status|employed');
		}
	}
	if (!$dispstatus_tid) {
		if ($old_dispstatus_tid == sirius_get_special_tid('sirius_worker_dispatch_status|employed')) {
			$asi = $worker_node->field_sirius_dispatch_asi['und'][0]['value'];
			if ($asi == 'Yes') {
				$dispstatus_tid = sirius_get_special_tid('sirius_worker_dispatch_status|not_employed_available');
			} else {
				$dispstatus_tid = sirius_get_special_tid('sirius_worker_dispatch_status|not_employed_not_available');
			}
		}
	}

	if ($dispstatus_tid && ($dispstatus_tid != $old_dispstatus_tid)) {
		sirius_dispatch_worker_dispstatus_set($worker_node, $dispstatus_tid, 1);
	}

	// If 
	// - The worker is currently EBA
	// - They have any active_secondary dispatches
	// - Any of those dispatches have dates that are today or later
	// then the worker is no longer EBA.
	if ($worker_node->field_sirius_dispatch_eba['und'][0]['value'] == 'Yes') {
		foreach ($dispatches as $dispatch_nid => $hr) {
			if ($hr['status'] != 'accepted_secondary') { continue; }

			$clear_eba = FALSE;
			$date = date('Y-m-d');
			if ($hr['job_node']->field_sirius_dispatch_eba_dates['und']) {
				foreach ($hr['job_node']->field_sirius_dispatch_eba_dates['und'] as $tmp) {
					if (date('Y-m-d', strtotime($tmp['value'])) >= $date) {
						$clear_eba = TRUE;
						$date = date('Y-m-d', strtotime($tmp['value']));
					}
				}
			}

			if ($clear_eba) {
				if (sirius_domain_component_enabled('dispatch:live')) {
					sirius_log('worker:eba', 'set', "Because the worker is on an EBA job until $date, EBA status has been set to 'No'.", '', '', array($worker_node->nid), array());
					sirius_log('worker:eba_dates', 'clear', "Because the worker is on an EBA job until $date, EBA dates have been cleared.", '', '', array($worker_node->nid), array());
				}
				$worker_node->field_sirius_dispatch_eba_dates['und'] = array();
				$worker_node->field_sirius_dispatch_eba['und'][0]['value'] = 'No';
				node_save($worker_node);
			}
		}
	}

	// We can't have more than one primary dispatch. So terminate any extras.
	foreach ($dispatches as $dispatch_nid => $hr) {
		if ($hr['status'] != 'accepted') { continue; }
		if ($dispatch_nid == $primary_dispatch_nid) { continue; }

		sirius_dispatch_terminate(
			node_load($dispatch_nid), 
			"This dispatch is being terminated because another primary dispatch has been accepted."
		);
	}

  // Switch out of the domain
  sirius_domain_pop($dispatch_node->field_sirius_domain['und'][0]['target_id']);
}

function sirius_dispatch_worker_recompute_from_work_status($worker_node) {
	$old_tid = $worker_node->original->field_sirius_work_status['und'][0]['tid'];
	$new_tid = $worker_node->field_sirius_work_status['und'][0]['tid'];

	// No change
	if ($old_tid == $new_tid) { return; }

	// We only care if the new status is non-dispatchable.
	$new_term = taxonomy_term_load($new_tid); 
	if ($new_term->field_sirius_dispatch_available['und'][0]['value'] == 'Yes') { return; }

	// Terminate any current dispatch
	/*
	$dispatch_node = sirius_dispatch_get_current($worker_node->nid);
	if ($dispatch_node) {
		$possible = sirius_dispatch_terminate_possible($dispatch_node);
		if ($possible['success']) {
			sirius_dispatch_terminate($dispatch_node, 'Your work status is now ' . $new_term->name . '.');
		}
	}
	*/

	// Sign out
	if ($worker_node->field_sirius_worker_dispstatus['und'][0]['tid'] == sirius_get_special_tid('sirius_worker_dispatch_status|not_employed_available')) { 
		sirius_dispatch_worker_dispstatus_set($worker_node, sirius_get_special_tid('sirius_worker_dispatch_status|not_employed_not_available'), NULL, "Work status was changed to " . $new_term->name);
	}
}