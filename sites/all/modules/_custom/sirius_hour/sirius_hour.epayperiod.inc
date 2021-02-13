<?php

function sirius_hour_epayperiod_load($domain_nid, $employer_nid, $year, $month, $day, $create_active = FALSE) {
	// Search for an existing payperiod node
	$date = "$year-$month-$day 00:00:00";
	$date_db = gmdate('Y-m-d H:i:s', strtotime($date));
	$query = new EntityFieldQuery;
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'sirius_employer_payperiod');
  $query->fieldCondition('field_grievance_shop', 'target_id', $employer_nid);
  $query->fieldCondition('field_sirius_datetime', 'value', $date_db, '<=');
  $query->fieldCondition('field_sirius_datetime_completed', 'value', $date_db, '>');
  if ($domain_nid) { $query->fieldCondition('field_sirius_domain', 'target_id', $domain_nid); }
  $query->propertyOrderBy('nid');
  $stmt = $query->execute();
  if ($stmt['node']) { return node_load(array_keys($stmt['node'])[0]); }

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