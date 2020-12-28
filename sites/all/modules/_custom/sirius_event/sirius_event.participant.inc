<?php

function sirius_event_participant_pstatus_set_possible($participant_node, $prole_tid, $pstatus_tid) {
	return array('success' => TRUE);	
}

function sirius_event_participant_pstatus_set($participant_node, $prole_tid, $pstatus_tid) {
	$possible = sirius_event_participant_pstatus_set_possible($participant_node, $prole_tid, $pstatus_tid);
	if (!$possible['success']) { return $possible; }

	if (!$prole_tid) { $prole_tid = $participant_node->field_sirius_event_prole['und'][0]['tid']; }
	if (!$pstatus_tid) { $pstatus_tid = $participant_node->field_sirius_event_pstatus['und'][0]['tid']; }

	if ($prole_tid == $participant_node->field_sirius_event_prole['und'][0]['tid'] && 
			$pstatus_tid == $participant_node->field_sirius_event_pstatus['und'][0]['tid']) {
		// return array('success' => TRUE, 'msg' => 'No changes.');
	}

	$participant_node->field_sirius_event_prole['und'][0]['tid'] = $prole_tid;
	$participant_node->field_sirius_event_pstatus['und'][0]['tid'] = $pstatus_tid;

	global $user;
	static $term_cache;
	if (!$term_cache[$prole_tid]) { $term_cache[$prole_tid] = taxonomy_term_load($prole_tid); }
	if (!$term_cache[$pstatus_tid]) { $term_cache[$pstatus_tid] = taxonomy_term_load($pstatus_tid); }
	sirius_jsonfield_log($participant_node, 'status', "Set prole = " . $term_cache[$prole_tid]->name . ", pstatus = " . $term_cache[$pstatus_tid]->name);
	node_save($participant_node);
}

/**
 * Return the full list of participant and events for a contact
 *
 * $result[participant_type_tid][0]['participant_node']
 * $result[participant_type_tid][0]['event_node']
 * $result[participant_type_tid][1]['participant_node']
 * $result[participant_type_tid][1]['event_node']
 *
 * @todo: This function loads too many things, especially when filtering by an event type and a contact has many participant records.
 */

function sirius_event_participants_by_contact($contact_nid, $event_type_tids = NULL) {
	// Get the full participant list for this contact
	$query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'sirius_event_participant');
  $query->fieldCondition('field_sirius_contact', 'target_id', $contact_nid);
  $query->propertyOrderBy('created');
  $stmt = $query->execute();
  $participant_nids = array();
  if ($stmt['node']) { $participant_nids = array_keys($stmt['node']); }
  $participant_nodes = node_load_multiple($participant_nids);

  // By type
  $by_type = array();
  foreach ($participant_nodes as $participant_nid => $participant_node) {
  	$event_node = node_load($participant_node->field_sirius_event['und'][0]['target_id']);
  	$event_type_tid = $event_node->field_sirius_event_type['und'][0]['tid'];
  	if ($event_type_tids && !in_array($event_type_tid, $event_type_tids)) { continue; }
		$by_type[$event_type_tid][] = array(
			'participant_node' => $participant_node,
			'event_node' => $event_node,
		);
  }

  return $by_type;
}

function sirius_event_participant_list_by_rule($event_nid, $rule, $limit = 0, $offset = 0) {
	$sql_args = array();
	$sql_args[':event_nid'] = $event_nid;
	$sql = "select node.nid as participant_nid ";
	$sql .= "from node ";
	$sql .= "left join field_data_field_sirius_event_prole on field_data_field_sirius_event_prole.entity_type = 'node' and field_data_field_sirius_event_prole.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_event_pstatus on field_data_field_sirius_event_pstatus.entity_type = 'node' and field_data_field_sirius_event_pstatus.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_event on field_data_field_sirius_event.entity_type = 'node' and field_data_field_sirius_event.entity_id = node.nid ";
	$sql .= "where node.type = 'sirius_event_participant' ";
	$sql .= "and field_sirius_event_target_id = :event_nid ";
	if ($rule['participants']) {
		$sql .= "and ( ";
		$delta = 0;
		foreach ($rule['participants'] as $tmp) {
			list ($prole_tid, $pstatus_tid) = explode('|', $tmp);
			if ($delta > 0) { $sql .= ' or '; }
			$sql .= " (field_sirius_event_prole_tid = :prole_tid_$delta and field_sirius_event_pstatus_tid = :pstatus_tid_$delta ) ";
			$sql_args[":prole_tid_$delta"] = $prole_tid;
			$sql_args[":pstatus_tid_$delta"] = $pstatus_tid;
			++$delta;
		}
		$sql .= ') ';
	}
	if ($limit) { $sql .= "limit $limit offset $offset "; }

	$stmt = db_query($sql, $sql_args);
	$participant_nids = array();
	while ($hr = $stmt->fetchAssoc()) {
		$participant_nids[] = $hr['participant_nid'];
	}
	return $participant_nids;
}

function sirius_event_participant_summary($event_nid, $prole_tid = NULL) {
	$sql_args = array();
	$sql_args[':event_nid'] = $event_nid;
	$sql_args[':prole_tid'] = $prole_tid;
	$sql = "select field_sirius_event_pstatus_tid, count(distinct node.nid) as c ";
	$sql .= "from node ";
	$sql .= "left join field_data_field_sirius_event on field_data_field_sirius_event.entity_type = 'node' and field_data_field_sirius_event.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_contact on field_data_field_sirius_contact.entity_type = 'node' and field_data_field_sirius_contact.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_event_prole on field_data_field_sirius_event_prole.entity_type = 'node' and field_data_field_sirius_event_prole.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_event_pstatus on field_data_field_sirius_event_pstatus.entity_type = 'node' and field_data_field_sirius_event_pstatus.entity_id = node.nid ";
	$sql .= "left join node contact_node on field_sirius_contact_target_id = contact_node.nid ";
	$sql .= "where node.type = 'sirius_event_participant' ";
	$sql .= "and field_sirius_event_target_id = :event_nid ";
	$sql .= "and field_sirius_event_prole_tid = :prole_tid ";
	$sql .= "group by field_sirius_event_pstatus_tid ";

	$stmt = sirius_sql_query($sql, $sql_args);
	$result = array();
	while ($hr = $stmt->fetchAssoc()) {
		$data[$hr['field_sirius_event_pstatus_tid']] = $hr['c'];
	}
	return $data;
}