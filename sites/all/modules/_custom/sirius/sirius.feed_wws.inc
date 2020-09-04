<?php

function sirius_feed_wws_info() {
  $items = array();
  $items['feed_wws'] = array(
    'name' => 'feed_wws',
    'title' => 'Worker Work Status',
    'description' => 'Upload a new list of worker work statuses.',
    'steps' => array(
			'upload' 		=> array('label' => 'Upload', 		'callback' => 'sirius_feed_wizard_upload'),
			'sheet' 		=> array('label' => 'Worksheet', 	'callback' => 'sirius_feed_wizard_sheet'),
			'map' 			=> array('label' => 'Mapping', 		'callback' => 'sirius_feed_wizard_map'),
			'verify' 		=> array('label' => 'Verify', 		'callback' => 'sirius_feed_wizard_verify'),
			'process' 	=> array('label' => 'Process', 		'callback' => 'sirius_feed_wizard_process'),
			'review' 		=> array('label' => 'Review', 		'callback' => 'sirius_feed_wizard_review'),
    ),
    'feed' => array(
    	'fields' => array(
    		'id' => array('title' => 'ID'),
    		'ssn' => array('title' => 'SSN'),
    		'work_status_id' => array('title' => 'Work Status ID', 'required' => TRUE),
    	),
      'pks' => array(
        'id' => 'ID',
        'ssn' => 'SSN',
      ),
    	'batch_size' => 500,
	    'callback_row_verify' => 'sirius_feed_wws_row_verify',
	    'callback_row_process' =>'sirius_feed_wws_row_process',
    ),
    'callback_launch' => 'sirius_feed_wizard_launch',
    'callback_launch_args' => array('feed_wws'),
  );
  return $items;
}  

function sirius_feed_wws_row_verify($feed_node, $info, $data, &$row, $precompute) {
	$domain_nid = sirius_domain_nid();
  $pk = $data['pk'];
  if (!$pk) { $pk = 'id'; }

  $row['ssn'] = sirius_ssn_format($row['ssn']);
  if ($pk == 'id') {
   if (!preg_match('/^[a-zA-Z0-9_]+$/', $row['id'])) { return array('success' => FALSE, 'msg' => 'Invalid Member ID'); }
  } else if ($pk == 'ssn') {
    if (!sirius_ssn_validate($row['ssn'])) { return array('success' => FALSE, 'msg' => 'Invalid SSN'); }
  }

  if ($precompute) {
    if ($pk == 'id') {
      $sql_args = array();
      $sql_args[':id'] = $row['id'];

      $sql = "select max(field_data_field_sirius_id.entity_id) as a, min(field_data_field_sirius_id.entity_id) as b from field_data_field_sirius_id ";
      if ($domain_nid) {
        $sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = field_data_field_sirius_id.entity_id ";
      }
      $sql .= "where field_data_field_sirius_id.entity_type = 'node' ";
      $sql .= "and field_data_field_sirius_id.bundle = 'sirius_worker' ";
      $sql .= "and field_sirius_id_value = :id ";
      if ($domain_nid) {
        $sql .= "and field_sirius_domain_target_id = :domain_nid";
        $sql_args[':domain_nid'] = $domain_nid;
      }
      $stmt = sirius_sql_query($sql, $sql_args);
      $hr = $stmt->fetchAssoc();
      $nid = $hr['a'];
      $other_nid = $hr['b'];
      if ($nid != $other_nid) {
        return array('success' => FALSE, 'msg' => 'Multiple workers found with this ID.');
      }
      $row['worker_nid'] = $nid;
    } else if ($pk == 'ssn') {
      $sql_args = array();
      $sql_args[':ssn'] = $row['ssn'];

      $sql = "select max(field_data_field_sirius_ssn.entity_id) as a, min(field_data_field_sirius_ssn.entity_id) as b from field_data_field_sirius_ssn ";
      if ($domain_nid) {
        $sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = field_data_field_sirius_ssn.entity_id ";
      }
      $sql .= "where field_data_field_sirius_ssn.entity_type = 'node' ";
      $sql .= "and field_data_field_sirius_ssn.bundle = 'sirius_worker' ";
      $sql .= "and field_sirius_ssn_value = :ssn ";
      if ($domain_nid) {
        $sql .= "and field_sirius_domain_target_id = :domain_nid";
        $sql_args[':domain_nid'] = $domain_nid;
      }
      $stmt = sirius_sql_query($sql, $sql_args);
      $hr = $stmt->fetchAssoc();
      $nid = $hr['a'];
      $other_nid = $hr['b'];
      if ($nid != $other_nid) {
        return array('success' => FALSE, 'msg' => 'Multiple workers found with this SSN.');
      }
      $row['worker_nid'] = $nid;
    }
  } 

	// Find the work status
	$work_status_id = $row['work_status_id'];
	if ($work_status_id === NULL || $work_status_id === '') { return array('success' => FALSE, 'msg' => 'No work status ID'); }

	$work_status_tid = sirius_taxonomy_lookup_by_anything('sirius_work_status', $work_status_id);
	if (!$work_status_tid) { return array('success' => FALSE, 'msg' => "Work status \"$work_status_id\" not found."); }
	if ($precompute) { $row['work_status_tid'] = $work_status_tid; }

  return array('success' => TRUE, 'msg' => 'Looks good.');
}

function sirius_feed_wws_row_process($feed_node, $info, $data, $row) {
	$result = sirius_feed_wws_row_verify($feed_node, $info, $data, $row, TRUE);
	if (!$result['success']) { return $result; }
	return sirius_worker_work_status_set(node_load($row['worker_nid']), $row['work_status_tid'], 'import', array());
}