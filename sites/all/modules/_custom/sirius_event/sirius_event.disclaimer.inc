<?php

function sirius_event_disclaimer_enabled_tid($event_type_tid) { return sirius_event_disclaimer_enabled_type(taxonomy_term_load($event_type_tid)); }

function sirius_event_disclaimer_enabled_type($event_type_term) {
  $json = sirius_jsonfield_util_term_json_get($event_type_term);
  return $json['disclaimer']['enabled'] == 'Yes';
}


function sirius_event_disclaimer_enabled_nid($event_nid) { return sirius_event_disclaimer_enabled(node_load($event_nid)); }

function sirius_event_disclaimer_enabled($event_node) {
	$disclaimer_json = sirius_event_disclaimer_json($event_node); 
	return $disclaimer_json['enabled'] == 'Yes';
}

function sirius_event_disclaimer_json($event_node) {
	$json = sirius_jsonfield_util_node_json_get($event_node);
	if ($json['disclaimer']) { return $json['disclaimer']; }

	$event_type_term = taxonomy_term_load($event_node->field_sirius_event_type['und'][0]['tid']);
	$json = sirius_jsonfield_util_term_json_get($event_type_term);
	return $json['disclaimer'];
}

function sirius_event_disclaimer_nids($event_disclaimer_json) {
	if (!$event_disclaimer_json['nids']) { return array(); }
	return array_filter(array_values($event_disclaimer_json['nids']));
}

function sirius_event_disclaimer_nodes($event_disclaimer_json) {
	return node_load_multiple(sirius_event_disclaimer_nids($event_disclaimer_json));
}

function sirius_event_disclaimer_check($event_node) {
	if (!sirius_event_disclaimer_enabled($event_node)) { return TRUE; }

	$event_disclaimer_json = sirius_event_disclaimer_json($event_node);
	if (!$event_disclaimer_json) { return TRUE; }

	$disclaimer_nids = sirius_event_disclaimer_nids($event_disclaimer_json);
	if (!$disclaimer_nids) { return TRUE; }

	$participant_nid = sirius_event_participant_nid_from_user($event_node->nid);
	if (!$participant_nid) { return TRUE; }
	$participant_node = node_load($participant_nid);

	$participant_json = sirius_jsonfield_util_node_json_get($participant_node);
	foreach ($disclaimer_nids as $disclaimer_nid) {
		if (!$participant_json['disclaimer'][$disclaimer_nid]['acceptances'][0]) { return FALSE; }
	}

	return TRUE;
}

function sirius_event_disclaimer_redirect($event_node) {
	if (!sirius_event_disclaimer_check($event_node)) {
		drupal_set_message("Please complete the disclaimers below.", 'warning');
		drupal_goto('node/' . $event_node->nid . '/sirius_event_disclaimer', array('query' => array('destination' => current_path())));
	}
}

/**
 * List the participants who have or have not accepted all disclaimers. SQL for speed
 */
function sirius_event_disclaimer_list($event_node, $options) {
  $event_disclaimer_json = sirius_event_disclaimer_json($event_node);
  if (!$event_disclaimer_json) { return array(); } 

  // What role(s) are we interested in?
  if (!$event_disclaimer_json['prole_tids']) { return array(); }
  $prole_tids = array_filter(array_values($event_disclaimer_json['prole_tids']));
  if (!$prole_tids) { return array(); }

  // What disclaimer NIDs are we interested in?
  $disclaimer_nids = sirius_event_disclaimer_nids($event_disclaimer_json);
  if (!$disclaimer_nids) { return array(); }

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