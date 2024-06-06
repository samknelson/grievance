<?php

function sirius_edls_sheet_reserved($sheet_node) {
	$sheet_json = sirius_json_get($sheet_node);
	return ($sheet_json['edls']['sheet']['reserved']['active'] == 'Yes');
}

function sirius_edls_sheet_reserved_set($sheet_node, $record) {
	$sheet_json = sirius_json_get($sheet_node);
	$sheet_json['edls']['sheet']['reserved'] = $record;
	sirius_json_set($sheet_node, $sheet_json);
	node_save($sheet_node);
}

function sirius_edls_sheet_reserved_fetch($employer_nid, $date, $create = FALSE) {
	$params = array();
	$params['employer_nid'] = $employer_nid;
	$params['date'] = $date;
	$params['reserved'] = 'Yes';
	$sheets = sirius_edls_sheet_search($params);
	$sheet_node = reset($sheets);
	if ($sheet_node) { return $sheet_node; }
	if (!$create) { return NULL; }

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
	$sheet_node->field_sirius_job_number['und'][0]['value'] = 'RESERVED';
	$sheet_node->field_sirius_date_start['und'][0]['value'] = date('Y-m-d 00:00:00', strtotime($date));
	$sheet_node->field_sirius_edls_sheet_status['und'][0]['value'] = 'lock';

	$sheet_node->title = sirius_edls_sheet_title($sheet_node);

	$sheet_json = array();
	$sheet_json['edls']['sheet']['reserved']['active'] = 'Yes';
	$sheet_json['edls']['crews'] = array();
	sirius_json_set($sheet_node, $sheet_json);
	node_save($sheet_node);

	return $sheet_node;
}
