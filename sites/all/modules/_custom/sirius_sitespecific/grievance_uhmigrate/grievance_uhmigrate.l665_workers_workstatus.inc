<?php

function grievance_uhmigrate_l665_workers_workstatus($limit, $source_filename = 'WorkerStatusHistory.csv') {
  // Give ourselves more memory
  ini_set('memory_limit', '4096M');
  
  $source_filename =  '/var/www/private/migrate/' . $source_filename;
  $source_handle = fopen($source_filename, "r");
  if (!$source_handle) {
    die("Unable to open $source_filename");
  }

  // Headers
  $headers = fgetcsv($source_handle, 10000);
  $skiprow = fgetcsv($source_handle, 10000);

  // Load our rows one at a time
  $rows_raw = array();
  $count = 0;
  while ($row_raw = fgetcsv($source_handle, 10000, ",")) {
    $rows_raw[] = $row_raw;
  }
  fclose($source_handle);
  // $rows_raw = array_reverse($rows_raw);
  
  $total_rowcount = count($rows_raw);

  $rowcount = 0;
  foreach ($rows_raw as $row_raw) {
    // Clear the cache...
    entity_get_controller('node')->resetCache();
    
    ++$rowcount;
    if (grievance_uhmigrate_skiprow()) { continue; }
    ++$processed_rowcount;
    if ($processed_rowcount > $limit) { break; }

    // Generate an associative array from the input
    $row = array("Row" => $rowcount);
    $col = 0;
    foreach ($headers as $header) {
      $row[$header] = $row_raw[$col];
      ++$col;
    }

    // Find the node to create or update
    $external_id = $row['WorkerID'];
    if (!$external_id) {
      grievance_uhmigrate_log('ERROR', $row, "No external ID (missing WorkerID)");
      continue;
    }
    $worker_node = grievance_uhmigrate_find_by_sirius_id('sirius_worker', $external_id);
    if ($worker_node->nid) { 
      grievance_uhmigrate_log('DEBUG', $row, "Found worker with $external_id, NID = " . $worker_node->nid . ". It will be updated.");
    } else {
      grievance_uhmigrate_log('DEBUG', $row, "No node found with $external_id. Skipping.");
      continue;
    }


    $work_status_id = $row['WorkerStatusID'];
    if (!isset($work_status_id)) {
      grievance_uhmigrate_log('ERROR', $row, "No WorkerStatusID");
      continue;
    }
    $work_status_term = grievance_uhmigrate_find_term_by_sirius_id('sirius_work_status', $work_status_id);
    if (!$work_status_term) {
      grievance_uhmigrate_log('ERROR', $row, "No member status found with ID $work_status_id");
      continue;
    }

    // Create or update the log entry
    /*
    $category = 'worker:work_status';
    $type = 'import';
    $message = $worker_node->title . ' has been assigned the new work status ' . $work_status_term->name . " with an effective date of " . $row['EffectiveDate'];
    $summary = 'Import';
    $notes = print_r($row, 1);
    $attachments = array();
    $handler_nids = array($worker_node->nid);

    $sql = "select * from field_data_field_sirius_notes where entity_type = 'node' and bundle = 'sirius_log' and field_sirius_notes_value = :notes ";
    $stmt = db_query($sql, array(':notes' => $notes));
    $hr = $stmt->fetchAssoc();
    if ($hr) {
      $log_node = node_load($hr['entity_id']);
      $log_node->field_sirius_type['und'][0]['value'] = $type;
      $log_node->field_sirius_category['und'][0]['value'] = $category;
      $log_node->field_sirius_summary['und'][0]['value'] = $summary;
      $log_node->field_sirius_message['und'][0]['value'] = $message;
      node_save($log_node);
    } else {
      sirius_log($category, $type, $message, $summary, $notes, $handler_nids, $attachments);
    }
    */

    // Set the worker status
    $worker_node->field_sirius_work_status['und'][0]['tid'] = $work_status_term->tid;
    $worker_node->sirius_no_recurse = TRUE;
    node_save($worker_node);
  }

  grievance_uhmigrate_log_summary();
}
