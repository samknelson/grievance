<?php

function sirius_edls_sheet_title($sheet_node) {
	$e = sirius_node_title($sheet_node->field_grievance_shop['und'][0]['target_id']);
	if (!$e) { $x = '[no employer]'; }

	$d = date('m/d/Y', strtotime($sheet_node->field_sirius_date_start['und'][0]['value']));

	$j = $sheet_node->field_sirius_job_number['und'][0]['value'];

	return "$e $d - #$j";
}

function sirius_edls_sheet_status_options() {
	return array(
		'draft' => t('Draft'),
		'request' => t('Requested'),
		'lock' => t('Scheduled'),
		'trash' => t('Discarded'),
	);
}

function sirius_edls_sheet_clone($old_sheet_node, $options = array()) {
	$date = $options['date'];
	if (!$date) {  return array('success' => FALSE, 'msg' => "Please enter the date for the new sheet."); }

	// Create the new object
	global $user;
	$sheet_node = new stdClass();
	$sheet_node->type = 'sirius_edls_sheet';
	$sheet_node->title = 'EDSL Sheet';
	$sheet_node->language = LANGUAGE_NONE;
	$sheet_node->uid = $user->uid;
	$sheet_node->status = 1;
	$sheet_node->promote = 0;
	$sheet_node->comment = 0;

	// Copy all fields
	foreach ($old_sheet_node as $key => $value) {
		if (!preg_match('/field_/', $key)) { continue; }
		$sheet_node->{$key} = $old_sheet_node->{$key};
	}

	// Set the start date and status
	$sheet_node->field_sirius_date_start['und'][0]['value'] = date('Y-m-d 00:00:00', strtotime($date));
	$sheet_node->field_sirius_edls_sheet_status['und'][0]['value'] = 'draft';

	// Give each crew a new UUID
	$sheet_json = sirius_json_get($sheet_node);
	$crew_uuid_map = array();
	$old_crews = $sheet_json['edls']['crews'];
	$new_crews = array();
	foreach ($old_crews as $old_crew_uuid => $crew) {
		$new_crew_uuid = uuid_generate();
		$crew['uuid'] = $new_crew_uuid;
		$new_crews[$new_crew_uuid] = $crew;
		$crew_uuid_map[$old_crew_uuid] = $new_crew_uuid;
	}
	$sheet_json['edls']['crews'] = $new_crews;
	sirius_json_set($sheet_node, $sheet_json);

	// Save the node
	$sheet_node->title = sirius_edls_sheet_title($sheet_node);
	$sheet_node = node_submit($sheet_node);
	node_object_prepare($sheet_node);
	node_save($sheet_node);

	// Assign all workers. Failure is a warning
	$warnings = array();
	$assignments = sirius_edls_assignment_fetch_sheet($old_sheet_node->field_sirius_date_start['und'][0]['value'], $old_sheet_node->nid);

	foreach ($assignments as $old_crew_uuid => $records) {
		foreach ($records as $assignment) {
			$result = sirius_edls_assign(
				$date, 
				$assignment['worker_id'], 
				$sheet_node->nid, 
				$crew_uuid_map[$old_crew_uuid],
				$assignment['extra']
			);

			if (!$result['success']) { $warnings[] = "Unable to assign $assignment[worker_name]: " . $result['msg']; }
		}
	}

	return array(
		'success' => TRUE, 
		'warnings' => $warnings,
		'sheet_node' => $sheet_node
	);
}

function sirius_edls_sheet_status_set($sheet_node, $new_status) {
	$access = sirius_access_instance('edls');
	if (!$access->access("sheet_status_$new_status")) { return array('success' => FALSE, 'msg' => 'Access Denied'); }

	$date = $sheet_node->field_sirius_date_start['und'][0]['value'];
	if ($new_status == 'trash') {
		$assignments = sirius_edls_assignment_fetch_sheet($date, $sheet_node->nid);
		foreach ($assignments as $crew_nid => $records) {
			foreach ($records as $position => $assignment) {
				sirius_edls_unassign($date, $assignment['worker_id'], $sheet_node->nid);
			}
		}
	}

	$sheet_node->field_sirius_edls_sheet_status['und'][0]['value'] = $new_status;
	node_save($sheet_node);
	return array('success' => TRUE);
}
