<?php

function sirius_event_session_enabled_tid($event_type_tid) { return sirius_event_session_enabled_type(taxonomy_term_load($event_type_tid)); }

function sirius_event_session_enabled_type($event_type_term) {
  $json = sirius_jsonfield_util_term_json_get($event_type_term);
  return $json['session']['enabled'] == 'Yes';
}


function sirius_event_session_enabled_nid($event_nid) { return sirius_event_session_enabled(node_load($event_nid)); }

function sirius_event_session_enabled($event_node) {
	$session_json = sirius_event_session_json($event_node); 
	return $session_json['enabled'] == 'Yes';
}

function sirius_event_session_json($event_node) {
	$json = sirius_jsonfield_util_node_json_get($event_node);
	return $json['session'];
}

function sirius_event_session_counts($event_node) {
	$dates = sirius_fieldvals($event_node, 'field_sirius_daterepeat', 'ts');
	return $dates;

  // What role(s) are we interested in?
  $event_json = sirius_jsonfield_util_node_json_get($event_node);
  $event_session_json = $event_json['session'];
  if (!$event_session_json['prole_tids']) { return array(); }
  $prole_tids = array_filter(array_values($event_session_json['prole_tids']));
  if (!$prole_tids) { return array(); }

  // Write a query
	$sql_args = array();
	$sql_args[':event_nid'] = $event_node->nid;
	$sql = "select node.nid as participant_nid, ";
	$sql .= "contact_node.nid as contact_nid, ";
	$sql .= "contact_node.title as contact_title, "; 
	$sql .= "field_sirius_event_pstatus_tid as pstatus_tid, ";
	$sql .= "field_sirius_event_prole_tid as prole_tid ";
	$sql .= "from node ";
	$sql .= "left join field_data_field_sirius_event_pstatus on field_data_field_sirius_event_pstatus.entity_type = 'node' and field_data_field_sirius_event_pstatus.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_event_prole on field_data_field_sirius_event_prole.entity_type = 'node' and field_data_field_sirius_event_prole.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_event on field_data_field_sirius_event.entity_type = 'node' and field_data_field_sirius_event.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_json on field_data_field_sirius_json.entity_type = 'node' and field_data_field_sirius_json.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_contact on field_data_field_sirius_contact.entity_type = 'node' and field_data_field_sirius_contact.entity_id = node.nid ";
	$sql .= "left join node contact_node on field_sirius_contact_target_id = contact_node.nid ";
	$sql .= "where node.type = 'sirius_event_participant' ";
	$sql .= "and field_sirius_event_target_id = :event_nid ";
	$sql .= "and (";
	foreach ($prole_tids as $prole_tid) {
		if ($delta > 0) { $sql .= ' or '; }
		$sql .= " field_sirius_event_prole_tid = :prole_tid_$delta ";
		$sql_args[":prole_tid_$delta"] = $prole_tid;
		++$delta;
	}
	$sql .= ') ';
	foreach ($disclaimer_nids as $disclaimer_nid) {
		$sql .= "and json_extract(field_sirius_json_value, '\$.disclaimer.\"$disclaimer_nid\".acceptances') ";
		if ($options['yes']) { $sql .= "is not null "; } else { $sql .= "is null "; }
	}
	$sql .= "limit 1000 ";

	$stmt = sirius_sql_query($sql, $sql_args);
	$participants = array();
	while ($hr = $stmt->fetchAssoc()) {
		$participants[] = $hr;
	}
	return $participants;
}


