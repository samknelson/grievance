<?php

function grievance_uhmigrate_l665_workers_cleanup($limit) {
  // Give ourselves more memory
  ini_set('memory_limit', '4096M');
  global $sirius_suppress_notifications;
  $sirius_suppress_notifications = TRUE;

  $employed_not_available = sirius_get_special_tid('sirius_worker_dispatch_status|employed_not_available');
  $not_employed_available = sirius_get_special_tid('sirius_worker_dispatch_status|not_employed_available');

  $sql = "select entity_id as nid from field_data_field_sirius_phone_alt where bundle = 'sirius_worker' order by entity_id ";;
  $stmt = db_query($sql, array());
  $rows_raw = array();
  while ($hr = $stmt->fetchAssoc()) {
    $rows_raw[] = $hr;
  }

  $headers = array('nid');
  
  $total_rowcount = count($rows_raw);

  $rowcount = 0;
  $xyz = variable_get('grievance_uhmigrate_l665_workers_cleanup_xyz', 0);
  foreach ($rows_raw as $row) {
    
    ++$rowcount;
    // if (grievance_uhmigrate_skiprow()) { continue; }
    if ($rowcount < $xyz) { continue; }
    ++$processed_rowcount;
    if ($processed_rowcount > $limit) { break; }

    $row['Row'] = $rowcount;
    // Clear the cache...
    entity_get_controller('node')->resetCache();

    print "Checking: $rowcount / $total_rowcount - $row[nid]\n";
    $worker_node = node_load($row['nid']);
    $need_save = FALSE;
    // $worker_node->field_sirius_dispatch_medium['und'][0]['value'] = 'voice';
    if ($worker_node->field_sirius_phone_alt['und'][0]['value'] && $worker_node->field_sirius_dispatch_medium['und'][1]['value'] != 'voice_alt') {
      $worker_node->field_sirius_dispatch_medium['und'][1]['value'] = 'voice_alt';
      $need_save = TRUE;
    } 
    if ($need_save) {
      print "... Saving: $row[nid]\n";
      node_save($worker_node);
    }

  }
  print "-----------------------------------------\n";

  // variable_set('grievance_uhmigrate_l665_workers_cleanup_xyz', $rowcount - 1);

  exit();
}
