<?php

function sirius_event_bullpen_enabled_nid($event_nid) { return sirius_event_bullpen_enabled(node_load($event_nid)); }

function sirius_event_bullpen_enabled($event_node) { return sirius_event_bullpen_enabled_tid($event_node->field_sirius_event_type['und'][0]['tid']); }

function sirius_event_bullpen_enabled_tid($event_type_tid) { return sirius_event_bullpen_enabled_type(taxonomy_term_load($event_type_tid)); }

function sirius_event_bullpen_enabled_type($event_type_term) {
  $json = json_decode($event_type_term->field_sirius_json['und'][0]['value'], TRUE);
  return $json['bullpen']['enabled'] == 'Yes';
}

function sirius_event_bullpen_ts($event_node) {
	$ts = sirius_date_ts($event_node, 'field_sirius_daterepeat');
	$signin_opens_self = $ts - variable_get('sirius_dispatch_bullpen_signin_opens', 60*60*24*5);
	$signin_opens_others = $ts - variable_get('sirius_dispatch_bullpen_signin_opens_others', 60*60);
	$signin_closes = $ts + variable_get('sirius_dispatch_bullpen_signin_grace', 60*15);

	return array(
		'job_ts' => $ts,
		'signin_opens_self' => $signin_opens_self,
		'signin_opens_others' => $signin_opens_others,
		'signin_closes' => $signin_closes,
	);
}

function sirius_event_bullpen_job($event_nid, $include_nonsponsor = FALSE) {
	$sql = "select entity_id from field_data_field_sirius_event ";
	$sql .= "where entity_type = 'node' ";
	$sql .= "and bundle = 'sirius_dispatch_job' ";
	$sql .= "and field_sirius_event_target_id = :event_nid ";
	$sql .= "order by entity_id ";
	if (!$include_nonsponsor) {
		$sql .= "limit 1 ";
	}
	$stmt = db_query($sql, array(':event_nid' => $event_nid));

	if (!$include_nonsponsor) {
		$hr = $stmt->fetchAssoc();
		$job_nid = $hr['entity_id'];
		return node_load($job_nid);
	} 

	$jobs = array();
	while ($hr = $stmt->fetchAssoc()) {
		$jobs[$hr['entity_id']] = node_load($hr['entity_id']);
	}
	return $jobs;
}