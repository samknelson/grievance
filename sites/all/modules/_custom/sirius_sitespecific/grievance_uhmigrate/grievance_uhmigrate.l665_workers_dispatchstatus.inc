<?php

function grievance_uhmigrate_l665_workers_dispatchstatus($limit, $source_filename = 'worker_memberstatus.csv') {
  // Give ourselves more memory
  ini_set('memory_limit', '4096M');
  global $sirius_suppress_notifications;
  $sirius_suppress_notifications = TRUE;

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
    if ($count > 1000) { break; }
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
    $row = array("Row" => $total_rowcount - $rowcount);
    $col = 0;
    foreach ($headers as $header) {
      $row[$header] = $row_raw[$col];
      ++$col;
    }

    if (!$row['SignedInAt']) { continue; }

    // Find the node to create or update
    $external_id = $row['WorkerID'];
    if (!$external_id) {
      grievance_uhmigrate_log('ERROR', $row, "No external ID (missing WorkerID)");
      continue;
    }
    $node = grievance_uhmigrate_find_by_sirius_id('sirius_worker', $external_id);
    if ($node->nid) { 
      grievance_uhmigrate_log('DEBUG', $row, "Found existing node with $external_id, NID = " . $node->nid . ". It will be updated.");
    } else {
      grievance_uhmigrate_log('DEBUG', $row, "No node found with $external_id. Skipping.");
      continue;
    }

    $node->field_sirius_worker_dispstatus['und'][0]['tid'] = sirius_get_special_tid('sirius_worker_dispatch_status|not_employed_available');
    $node->field_sirius_dispatch_availdate['und'][0]['value'] = gmdate('Y-m-d H:i:s', strtotime($row['SignedInAt']));
    node_save($node);
  }

  grievance_uhmigrate_log_summary();
}
