<?php

function sirius_edls_sheet_reserved($sheet_node) {
	if (!$sheet_node) { return FALSE; }
	
	if ($sheet_node->field_sirius_edls_sheet_status['und'][0]['value'] == 'reserved') { return TRUE; }

	// Legacy. We might throw this away.
	$sheet_json = sirius_json_get($sheet_node);
	if ($sheet_json['edls']['sheet']['reserved']['active'] == 'Yes') { return TRUE; }

	return FALSE;
}

function sirius_edls_sheet_reserved_set($sheet_node, $record) {
	$sheet_json = sirius_json_get($sheet_node);
	$sheet_json['edls']['sheet']['reserved'] = $record;
	sirius_json_set($sheet_node, $sheet_json);
	node_save($sheet_node);
}

function sirius_edls_sheet_reserved_types() {
	// @todo: This should be a fancy callback framework.

	return array(
		'tos' => array(
			'title' => 'Out Sick',
			'job_number' => 'OUT_SICK',
			'count' => 100,
		),
		'tor' => array(
			'title' => 'Out Vacation',
			'job_number' => 'OUT_VACATION',
			'count' => 100,
		),
		'cdl' => array(
			'title' => 'CDL Rest',
			'job_number' => 'CDL_REST',
			'count' => 100,
		),
		'layoff' => array(
			'title' => 'Scheduled For Layoff',
			'job_number' => 'LAYOFF',
			'count' => 100,
		),
		'layoff' => array(
			'title' => 'Scheduled For Layoff',
			'job_number' => 'LAYOFF',
			'count' => 100,
		),
	);
}

function sirius_edls_sheet_reserved_type_options() {
	$types = sirius_edls_sheet_reserved_types();
	$options = array();
	foreach ($types as $key => $type) { $options[$key] = $type['title']; }
	return $options;
}

function sirius_edls_sheet_reserved_fetch($employer_nid, $date, $type_key, $create = FALSE) {
	$types = sirius_edls_sheet_reserved_types();
	$type = $types[$type_key];
	if (!$type) {
		return array('success' => FALSE, 'msg' => "Unknown type: $type_key");
	}

	$params = array();
	$params['employer_nid'] = $employer_nid;
	$params['type'] = $type_key;
	$params['date'] = $date;
	$params['reserved'] = 'Yes';
	$sheets = sirius_edls_sheet_search($params);
	$sheet_node = reset($sheets);
	if ($sheet_node) { 
		return array('success' => TRUE, 'msg' => 'Found existing sheet', 'sheet_node' => $sheet_node); 
	}
	if (!$create) {
		return array('success' => FALSE, 'msg' => 'Sheet not found.'); 
	}

	$sheet_node = new stdClass();
	$sheet_node->type = 'sirius_edls_sheet';
	$sheet_node->title = 'EDSL Sheet';
	$sheet_node->language = LANGUAGE_NONE;
	$sheet_node->uid = $user->uid;
	$sheet_node->status = 1;
	$sheet_node->promote = 0;
	$sheet_node->comment = 0;
	$sheet_node = node_submit($sheet_node);
	node_object_prepare($sheet_node);

	$employer_node = node_load($employer_nid);
	$sheet_node->field_sirius_domain = $employer_node->field_sirius_domain;
	$sheet_node->field_grievance_shop['und'][0]['target_id'] = $employer_nid;
	$sheet_node->field_sirius_job_number['und'][0]['value'] = $type['job_number'];
	$sheet_node->field_sirius_date_start['und'][0]['value'] = date('Y-m-d 00:00:00', strtotime($date));
	$sheet_node->field_sirius_edls_sheet_status['und'][0]['value'] = 'reserved';
	$sheet_node->field_sirius_type['und'][0]['value'] = $type_key;
	$sheet_node->field_sirius_count['und'][0]['value'] = $type['count'];

	$sheet_node->title = sirius_edls_sheet_title($sheet_node);

	$sheet_json = array();
	$sheet_json['edls']['sheet']['reserved']['active'] = 'Yes';
	$sheet_json['edls']['crews'] = array();

	$uuid = uuid_generate();
	$crew = array();
	$crew['name'] = 'Main';
	$crew['count'] = $type['count'];
	$crew['uuid'] = $uuid;
	$sheet_json['edls']['crews'][$uuid] = $crew;

	sirius_json_set($sheet_node, $sheet_json);
	node_save($sheet_node);

	return array('success' => TRUE, 'msg' => 'Created a new sheet', 'sheet_node' => $sheet_node);
}
