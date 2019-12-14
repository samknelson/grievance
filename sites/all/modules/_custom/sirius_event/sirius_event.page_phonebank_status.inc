<?php

function sirius_node_tab_event_phonebank_status_access($event_node) {
  return sirius_node_tab_event_phonebank_settings_access($event_node);
}

function sirius_node_tab_event_phonebank_status($event_node) {
  $json = json_decode($event_node->field_sirius_json['und'][0]['value'], TRUE);
  $prole_tid = $json['phonebank']['prole_tid'];
  $pstatus_tid = $json['phonebank']['pstatus_tid'];
	$prole_term = taxonomy_term_load($prole_tid);

  $sql = "select field_sirius_event_pstatus_tid as status_tid, pstatus_term.name as status_name, count(*) as c from node ";
  $sql .= "left join field_data_field_sirius_event on field_data_field_sirius_event.entity_type = 'node' and field_data_field_sirius_event.entity_id = node.nid ";
  $sql .= "left join field_data_field_sirius_event_prole on field_data_field_sirius_event_prole.entity_type = 'node' and field_data_field_sirius_event_prole.entity_id = node.nid ";
  $sql .= "left join field_data_field_sirius_event_pstatus on field_data_field_sirius_event_pstatus.entity_type = 'node' and field_data_field_sirius_event_pstatus.entity_id = node.nid ";
  $sql .= "left join taxonomy_term_data pstatus_term on pstatus_term.tid = field_sirius_event_pstatus_tid ";
  $sql .= "where node.type = 'sirius_event_participant' ";
  $sql .= "and field_sirius_event_target_id = :event_nid ";
  $sql .= "and field_sirius_event_prole_tid = :prole_tid ";
  $sql .= "group by field_sirius_event_pstatus_tid ";
  $sql .= "order by pstatus_term.name ";
  $sql .= "limit 10 ";

  $rows = array();
  $stmt = db_query($sql, array(':event_nid' => $event_node->nid, ':prole_tid' => $prole_tid));
  while ($hr = $stmt->fetchAssoc()) {
  	$row = array();
  	$row[] = $hr['status_name'];
  	$row[] = number_format($hr['c']);
  	// $row[] = tools go here
  	$rows[] = $row;
  }

  if (!$rows) {
  	return t('No participants found with role @name.', array('@name' => $prole_term->name));
  }

  $header = array(
  	t('Status'),
  	t('Count'),
  );
  return theme_table(array('rows' => $rows, 'header' => $header, 'attributes' => array()));

}