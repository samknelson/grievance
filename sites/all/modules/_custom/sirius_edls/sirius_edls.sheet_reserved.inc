<?php

function sirius_edls_sheet_reserved($sheet_node) {
	if (!$sheet_node) { return FALSE; }

	if ($sheet_node->field_sirius_edls_sheet_status['und'][0]['value'] == 'reserved') { return TRUE; }

	// Legacy. We might throw this away.
	$sheet_json = sirius_json_get($sheet_node);
	if ($sheet_json['edls']['sheet']['reserved']['active'] == 'Yes') { return TRUE; }

	return FALSE;
}

function sheet_reserved_type($sheet_node) {
	return $sheet_node->field_sirius_type['und'][0]['value'];
}

function sirius_edls_sheet_reserved_set($sheet_node, $record) {
	$sheet_json = sirius_json_get($sheet_node);
	$sheet_json['edls']['sheet']['reserved'] = $record;
	sirius_json_set($sheet_node, $sheet_json);
	node_save($sheet_node);
}

function sirius_edls_sheet_reserved_info_all() {
	$info_all = &drupal_static(__FUNCTION__, NULL);

	if (!isset($info_all)) {
		$info_all = module_invoke_all('sirius_edls_sheet_reserved_info');
		ksort($info_all);
	}

	return $info_all;
}

function sirius_edls_sheet_reserved_info($plugin_key) {
	$info_all = sirius_edls_sheet_reserved_info_all();
	return $info_all[$plugin_key];
}

/**
 * Implements hook_sirius_edls_sheet_reserved_info
 */

function sirius_edls_sirius_edls_sheet_reserved_info() {
	return array(
		'tos' => array(
			'title' => 'Out Sick',
			'job_number' => 'OUT_SICK',
			'count' => 100,
			'start_time' => '23:58',
			'end_time' => '23:59',
		),
		'cdl' => array(
			'title' => 'CDL Rest',
			'job_number' => 'CDL_REST',
			'count' => 100,
			'start_time' => '23:58',
			'end_time' => '23:59',
		),
		'layoff' => array(
			'title' => 'Layoff',
			'job_number' => 'LAYOFF',
			'count' => 100,
			'start_time' => '23:58',
			'end_time' => '23:59',
		),
	);
}

function sirius_edls_sheet_reserved_options() {
	$info_all = sirius_edls_sheet_reserved_info_all();
	$options = array();
	foreach ($info_all as $key => $info) { $options[$key] = $info['title']; }
	return $options;
}

function sirius_edls_sheet_reserved_fetch($employer_nid, $date, $type_key, $create = FALSE) {
	$types = sirius_edls_sheet_reserved_info_all();
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
	$sheet_json['edls']['sheet']['reserved']['type'] = $type_key;
	$sheet_json['edls']['crews'] = array();

	$uuid = uuid_generate();
	$crew = array();
	$crew['name'] = 'Main';
	$crew['count'] = $type['count'];
	$crew['uuid'] = $uuid;
	$crew['start_time'] = $type['start_time'];
	$crew['end_time'] = $type['end_time'];

	$sheet_json['edls']['crews'][$uuid] = $crew;

	sirius_json_set($sheet_node, $sheet_json);
	node_save($sheet_node);

	return array('success' => TRUE, 'msg' => 'Created a new sheet', 'sheet_node' => $sheet_node);
}
