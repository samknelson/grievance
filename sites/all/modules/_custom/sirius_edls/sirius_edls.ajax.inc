<?php

function sirius_edls_ajax_worker_list() {
	$employer_nid = $_REQUEST['employer_nid'];
	if (!$employer_nid) { return sirius_edls_ajax_error('Please pass in an employer_nid.'); }

	$date = $_REQUEST['date'];
	if (!$date) { return sirius_edls_ajax_error('Please pass in an employer_nid.'); }

	$can_access = sirius_access_instance('edls')->access('worker_list', array('employer_nid' => $employer_nid));
	if (!$can_access) { return sirius_edls_ajax_error('Access denied'); }

	$filters = $_REQUEST['filters'];

	$workers = sirius_edls_worker_list($employer_nid, $date, $filters);

	// return drupal_json_output(array('success' => FALSE, 'msg' => print_r($filters, 1)));

	$result = array(
		'success' => TRUE,
		'data' => array(
			'workers' => $workers,
		),
	);
	
	return drupal_json_output($result);
}

function sirius_edls_ajax_worker_lookup() {
	$employer_nid = $_REQUEST['employer_nid'];
	if (!$employer_nid) { return sirius_edls_ajax_error('Please pass in an employer_nid.'); }

	$worker_id = $_REQUEST['worker_id'];
	if (!$worker_id) { return sirius_edls_ajax_error('Please pass in a worker_id.'); }

	$date = $_REQUEST['date'];
	if (!$date) { return sirius_edls_ajax_error('Please pass in an employer_nid.'); }

	$can_access = sirius_access_instance('edls')->access('worker_list', array('employer_nid' => $employer_nid));
	if (!$can_access) { return sirius_edls_ajax_error('Access denied'); }

	$worker = sirius_edls_worker_fetch($employer_nid, $worker_id, $date);

	$result = array(
		'success' => TRUE,
		'worker' => $worker,
		'render' => sirius_edls_worker_render_details($worker),
	);
	
	return drupal_json_output($result);
}

function sirius_edls_ajax_assign() {
	$worker_id = $_REQUEST['worker_id'];
	if (!$worker_id) { return sirius_edls_ajax_error('Please pass in a worker_id'); }

	$sheet_nid = $_REQUEST['sheet_nid'];
	if (!$sheet_nid) { return sirius_edls_ajax_error('Please pass in a sheet_nid'); }
	$sheet_node = node_load($sheet_nid);

	$crew_uuid = $_REQUEST['crew_uuid'];
	if (!$crew_uuid) { return sirius_edls_ajax_error('Please pass in a crew_uuid.'); }

	$extra = $_REQUEST['extra'];

	$can_access = sirius_access_instance('edls')->access('sheet_edit_workers', array('sheet_node' => $sheet_node));
	if (!$can_access) { return sirius_edls_ajax_error('Access denied'); }

	$date = $sheet_node->field_sirius_date_start['und'][0]['value'];

	$result = sirius_edls_assign($date, $worker_id, $sheet_nid, $crew_uuid, $extra);
	if (!$result['success']) { return sirius_edls_ajax_error($result['msg']); }
	$result['data']['stats'] = sirius_edls_ajax_sheet_stats($sheet_node);

	return drupal_json_output($result);
}

function sirius_edls_ajax_assignment_set_extra() {
	$worker_id = $_REQUEST['worker_id'];
	if (!$worker_id) { return sirius_edls_ajax_error('Please pass in a worker_id'); }

	$sheet_nid = $_REQUEST['sheet_nid'];
	if (!$sheet_nid) { return sirius_edls_ajax_error('Please pass in a sheet_nid'); }
	$sheet_node = node_load($sheet_nid);

	$extra = $_REQUEST['extra'];

	$can_access = sirius_access_instance('edls')->access('sheet_edit_workers', array('sheet_node' => $sheet_node));
	if (!$can_access) { return sirius_edls_ajax_error('Access denied'); }

	$date = $sheet_node->field_sirius_date_start['und'][0]['value'];

	$result = sirius_edls_assignment_set_extra($date, $worker_id, $sheet_nid, $extra);
	if (!$result['success']) { return sirius_edls_ajax_error($result['msg']); }
	$result['data']['stats'] = sirius_edls_ajax_sheet_stats($sheet_node);

	return drupal_json_output($result);
}

function sirius_edls_ajax_unassign() {
	$worker_id = $_REQUEST['worker_id'];
	if (!$worker_id) { return sirius_edls_ajax_error('Please pass in a worker_id'); }

	$sheet_nid = $_REQUEST['sheet_nid'];
	if (!$sheet_nid) { return sirius_edls_ajax_error('Please pass in a sheet_nid'); }
	$sheet_node = node_load($sheet_nid);

	$can_access = sirius_access_instance('edls')->access('sheet_edit_workers', array('sheet_node' => $sheet_node));
	if (!$can_access) { return sirius_edls_ajax_error('Access denied'); }

	$date = $sheet_node->field_sirius_date_start['und'][0]['value'];

	$result = sirius_edls_unassign($date, $worker_id, $sheet_nid);
	if (!$result['success']) { return sirius_edls_ajax_error($result['msg']); }
	$result['data']['stats'] = sirius_edls_ajax_sheet_stats($sheet_node);

	return drupal_json_output($result);
}

function sirius_edls_ajax_sheet_assignments() {
	$sheet_nid = $_REQUEST['sheet_nid'];
	if (!$sheet_nid) { return sirius_edls_ajax_error('Please pass in a sheet_nid'); }
	$sheet_node = node_load($sheet_nid);

	$can_access = sirius_access_instance('edls')->access('sheet_edit_workers', array('sheet_node' => $sheet_node));
	if (!$can_access) { return sirius_edls_ajax_error('Access denied'); }

	$date = $sheet_node->field_sirius_date_start['und'][0]['value'];

	$assignments = sirius_edls_assignment_fetch_sheet($date, $sheet_nid);
	$result = array(
		'success' => TRUE,
		'data' => array(
			'assignments' => $assignments,
			'notes_render' => sirius_edls_sheet_notes_render($sheet_node),
			'notes' => sirius_edls_sheet_notes_get($sheet_node),
		),
	);
	$result['data']['stats'] = sirius_edls_ajax_sheet_stats($sheet_node);

	return drupal_json_output($result);
}

function sirius_edls_ajax_sheet_set_notes() {
	$sheet_nid = $_REQUEST['sheet_nid'];
	if (!$sheet_nid) { return sirius_edls_ajax_error('Please pass in a sheet_nid'); }
	$sheet_node = node_load($sheet_nid);

	$can_access = sirius_access_instance('edls')->access('sheet_edit_workers', array('sheet_node' => $sheet_node));
	if (!$can_access) { return sirius_edls_ajax_error('Access denied'); }

	$notes = $_REQUEST['notes'];

	sirius_edls_sheet_notes_set($sheet_node, $notes);

	$result = array(
		'success' => TRUE,
		'data' => array(
			'notes_render' => sirius_edls_sheet_notes_render($sheet_node),
			'notes' => sirius_edls_sheet_notes_get($sheet_node),
			'whatever' => $_REQUEST,
			'node' => $sheet_node,
			'json' => sirius_json_get($sheet_node),
		),
	);
	$result['data']['stats'] = sirius_edls_ajax_sheet_stats($sheet_node);

	return drupal_json_output($result);
}

function sirius_edls_ajax_error($msg) {
	return drupal_json_output(array('success' => FALSE, 'msg' => $msg));
}

function sirius_edls_ajax_sheet_stats($sheet_node) {
	$date = $sheet_node->field_sirius_date_start['und'][0]['value'];
	$assigned = sirius_edls_assignment_count_sheet($date, $sheet_node->nid);
	$count = $sheet_node->field_sirius_count['und'][0]['value'];
	return array('count' => $count, 'assigned' => $assigned);
}