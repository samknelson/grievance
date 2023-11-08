<?php

function sirius_hour_epayperiod_load($domain_nid, $employer_nid, $year, $month, $day, $create_active = FALSE) {
	// Search for an existing payperiod node
	$date = "$year-$month-$day 00:00:00";
	$date_db = gmdate('Y-m-d H:i:s', strtotime($date));



  $domain_nid = sirius_domain_nid();
  $sql_args = array();
  $sql = "select nid from node ";
  $sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
  $sql .= "left join field_data_field_sirius_worker on field_data_field_sirius_worker.entity_type = 'node' and field_data_field_sirius_worker.entity_id = node.nid ";
  $sql .= "left join field_data_field_grievance_shop on field_data_field_grievance_shop.entity_type = 'node' and field_data_field_grievance_shop.entity_id = node.nid ";
  $sql .= "left join field_data_field_sirius_datetime use index (field_sirius_datetime_value) on field_data_field_sirius_datetime.entity_type = 'node' and field_data_field_sirius_datetime.entity_id = node.nid ";
  $sql .= "left join field_data_field_sirius_datetime_completed on field_data_field_sirius_datetime_completed.entity_type = 'node' and field_data_field_sirius_datetime_completed.entity_id = node.nid ";
  $sql .= "where node.type = 'sirius_employer_payperiod' ";

  if ($domain_nid) {
    $sql .= "and field_sirius_domain_target_id = :domain_nid ";
    $sql_args[':domain_nid'] = $domain_nid;
  }

  $sql .= "and field_grievance_shop_target_id = :employer_nid ";
  $sql_args[':employer_nid'] = $employer_nid;

  if ($day == 1) {
    $sql .= "and field_sirius_datetime_value = :date_db ";
    $sql_args[':date_db'] = $date_db;
  } else {
    $sql .= "and field_sirius_datetime_value <= :date_db ";
    $sql .= "and field_sirius_datetime_completed_value > :date_db ";
    $sql_args[':date_db'] = $date_db;
  }
  $sql .= "order by nid ";

  $stmt = sirius_sql_query($sql, $sql_args);
  $hr = $stmt->fetchAssoc();
  if ($hr['nid']) { return node_load($hr['nid']); }

  // Didn't find one. 
  if (!$create_active) { return NULL; }

  $epayperiod_node = new stdClass();
  $epayperiod_node->type = 'sirius_employer_payperiod';
  $epayperiod_node->no_recurse = TRUE;
  node_object_prepare($epayperiod_node);
  $epayperiod_node->language = LANGUAGE_NONE;
  $epayperiod_node->uid = $user->uid;
  $epayperiod_node->status = 1;
  $epayperiod_node->promote = 0;
  $epayperiod_node->comment = 0;
	$start_date = "$year-$month-1 00:00:00";
	$start_date_db = gmdate('Y-m-d H:i:s', strtotime($start_date));
  $epayperiod_node->field_sirius_datetime['und'][0]['value'] = $start_date_db;
	$end_date = "$year-$month-15 23:59:00";
	$end_date_db = gmdate('Y-m-t 23:59:00', strtotime($end_date));
  $epayperiod_node->field_sirius_datetime_completed['und'][0]['value'] = $end_date_db;
  $epayperiod_node->field_sirius_active['und'][0]['value'] = $create_active;
  $epayperiod_node->field_grievance_shop['und'][0]['target_id'] = $employer_nid;
  if ($domain_nid) { $epayperiod_node->field_sirius_domain['und'][0]['target_id'] = $domain_nid; }
  $epayperiod_node = node_submit($epayperiod_node);
  node_object_prepare($epayperiod_node);
  node_save($epayperiod_node);
  return $epayperiod_node;
}