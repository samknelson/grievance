<?php

function sirius_edls_sched_check_worker_day($employer_nid, $worker_nid, $date) {
	$date = date('Y-m-d', strtotime($date));
	$filters = array();
	$filters['entity_nid'] = $worker_nid;
	$filters['interval'] = array(
		'start_ts' => strtotime($date . ' 12:01 am'),
		'end_ts' => strtotime($date . '12:59 pm'),
	);
	$scheds = sirius_sched_db_search($filters);

	foreach ($scheds as $sched) {
		// For now, we're only interested in worker_tor
		if ($sched['type'] != 'worker_tor') { continue; }
		return $sched;
	}

	return NULL;
}

function sirius_edls_sched_sync($mode = 'test') {
	$employers = sirius_edls_employer_list();

	$dates = array();
	for ($i=0; $i<10; ++$i) {
		$dates[] = date('Y-m-d', strtotime("+$i days"));
	}

	foreach ($dates as $date) {
		sirius_minilog("Scanning for schedule conflicts on $date");
		foreach ($employers as $employer_nid => $employer_name) {
			sirius_minilog("Scanning employer $employer_name ($employer_nid)");

			$filters = array();
			$filters['skip_outsource'] = TRUE;
			$workers = sirius_edls_worker_list($employer_nid, $date, $filters);

			foreach ($workers as $worker_record) {
			  $worker_id = $worker_record['worker_id'];
			  if (!preg_match('/^I/', $worker_id)) { continue; }
		    $worker_id_external = preg_replace('/^I/', '', $worker_id);
		    $worker_nid =  sirius_worker_get_by_id($worker_id_external, TRUE);
		    sirius_minilog("Scanning worker $worker_id ($worker_nid)");
		    sirius_edls_sched_sync_worker_day($employer_nid, $worker_nid, $date, $worker_id);
			}
		}
	}
	return array('success' => TRUE, 'count' => $count);
}

function sirius_edls_sched_sync_worker_day($employer_nid, $worker_nid, $date, $worker_id = NULL) {
	// Find the worker ID if we don't have it
	if (!$worker_id) {
		$worker_node = node_load($worker_nid);
		$worker_id_external = $worker_node->field_sirius_id['und'][0]['value'];
		$worker_id = 'I' . $worker_id_external;
	}

	sirius_minilog("Scanning for schedule conflict: Employer $employer_nid, worker $worker_id ($worker_nid), date $date");
	$sched = sirius_edls_sched_check_worker_day($employer_nid, $worker_nid, $date);

	//
	// If there are no schedule conflicts, we need to remove the worker from the reserved sheet
	// 

	if (!$sched) {
		sirius_minilog("-- No conflicts found.");

		$sheet_node = sirius_edls_sheet_reserved_fetch($employer_nid, $date, FALSE);
		if (!$sheet_node) { return array('success' => TRUE, 'msg' => 'No conflicts found.'); }

		$assignment = sirius_edls_assignment_fetch_worker($date, $employer_nid, $worker_id);
		if (!$assignment) { return array('success' => TRUE, 'msg' => 'No conflicts found.'); }
		if ($assignment['sheet_nid'] != $sheet_node->nid) { return array('success' => TRUE, 'msg' => 'No conflicts found.'); }

		sirius_minilog("-- Removing the worker from the reserved sheet.");
		$result = sirius_edls_unassign($date, $worker_id, $assignment['sheet_nid']);
		if (!$result['success']) {
			sirius_minilog("Unable to remove worker: " . $result['msg'], 'error');
			return $result;
		}

		return array('success' => TRUE, 'msg' => 'No conflicts found.');
	}

	//
	// There is a schedule conflict. We need to put the work on the reserved sheet.
	//

	sirius_minilog("-- Conflict found for worker");

	// Fetch (or create) the reserved sheet
	$sheet_node = sirius_edls_sheet_reserved_fetch($employer_nid, $date, TRUE);
	$sheet_json = sirius_json_get($sheet_node);
	$crews = $sheet_json['edls']['crews'];
	if (!$crews) { $crews = array(); }

	// Remove their current assignment
	$assignment = sirius_edls_assignment_fetch_worker($date, $employer_nid, $worker_id);
	if ($assignment) {
		if ($assignment['sheet_nid'] == $sheet_node->nid) {
			sirius_minilog("-- Worker is already on the reserve sheet. No actions needed.");
			return array('success' => TRUE, 'msg' => "Worker is already on the reserve sheet."); 
		}

		sirius_minilog("-- Removing $worker_id from their current sheet");
		$result = sirius_edls_unassign($date, $worker_id, $assignment['sheet_nid']);
		if (!$result['success']) {
			sirius_minilog("Unable to remove worker: " . $result['msg'], 'error');
			return $result;
		}
	}

	// Find the correct crew. Might need to create one.
	$crew_name = $sched['json']['display']['type'];
	foreach ($sheet_json['edls']['crews'] as $crew_uuid => $check_crew) {
		if ($check_crew['name'] == $crew_name) {
			$crew = $check_crew;
			break;
		}
	}
	if (!$crew) {
		$crew_uuid = uuid_generate();
		sirius_minilog("-- Creating a crew: $crew_name");
		$crew['uuid'] = $crew_uuid;
		$crew['name'] = $crew_name;
		$crew['count'] = 1;
		$sheet_json['edls']['crews'][$crew_uuid] = $crew;
		sirius_json_set($sheet_node, $sheet_json);
		node_save($sheet_node);
	}

	// Make sure that there's enough room on the crew
  $assignments = sirius_edls_assignment_fetch_crew($sheet_node, $crew_uuid);
  if (count($assignments) <= $crew['count']) { 
  	$sheet_json['edls']['crews'][$crew_uuid]['count'] = count($assignments) + 1;
		sirius_json_set($sheet_node, $sheet_json);

		$sheet_count = 0;
		foreach ($sheet_json['edls']['crews'] as $check_crew) { 
			$sheet_count += $check_crew['count'];
		}
		$sheet_node->field_sirius_count['und'][0]['value'] = $sheet_count;
		node_save($sheet_node);
  }

	// Assign the worker to the crew
	sirius_minilog("-- Assigning $worker_id to the reserved sheet");
	$result = sirius_edls_assign($date, $worker_id, $sheet_node->nid, $crew['uuid']);
	if (!$result['success']) {
		sirius_minilog("Unable to assign worker: " . $result['msg'], 'error');
		return $result;
	}

	return array('success' => TRUE, 'msg' => "Worker $worker_id has been placed on the $crew_name crew for the reserved sheet.");
}

/**
 * Implements hook_sirius_sched_post_update
 * 
 * When a schedule item is created or updated, re-sync the affected worker
 */

function sirius_edls_sirius_sched_post_update($sched) {
	// Only if EDLS is enabled
	if (!sirius_domain_component_enabled('edls')) { return; }

	// Only if we have a worker
	$worker_nid = $sched['entity_nid'];
	if (!$worker_nid) { return; }
	$worker_node = node_load($worker_nid);
	if ($worker_node->type != 'sirius_worker') { return; }
	$worker_id_external = $worker_node->field_sirius_id['und'][0]['value'];
	if (!$worker_id_external) { return; }
	$worker_id = "I$worker_id_external";

	// Get the list of employers and dates
	$employers = sirius_edls_employer_list();
	$dates = array();
	for ($i=0; $i<10; ++$i) {
		$dates[] = date('Y-m-d', strtotime("+$i days"));
	}

	foreach ($employers as $employer_nid => $employer_name) {
		// Only if the worker is dispatched to the employer
		$filters = array();
		$filters['skip_outsource'] = TRUE;
		$filters['worker_id'] = $worker_id;
		$workers = sirius_edls_worker_list($employer_nid, $date, $filters);

		foreach ($workers as $worker_record) {
			foreach ($dates as $date) {
		    sirius_edls_sched_sync_worker_day($employer_nid, $worker_nid, $date, $worker_id);
			}
		}
	}

	return array('success' => TRUE, 'count' => $count);
}