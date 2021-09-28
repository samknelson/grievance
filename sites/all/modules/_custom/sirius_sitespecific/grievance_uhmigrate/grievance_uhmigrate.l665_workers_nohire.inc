<?php

function grievance_uhmigrate_l665_workers_nohire($limit, $source_filename = 'NoHire.csv') {
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
  while ($row_raw = fgetcsv($source_handle, 10000, ",")) {
    $rows_raw[] = $row_raw;
  }
  fclose($source_handle);
  // $rows_raw = array_reverse($rows_raw);
  
  $total_rowcount = count($rows_raw);

  $rowcount = 0;
  $processed_rowcount = 0;
  foreach ($rows_raw as $row_raw) {
    ++$rowcount;
    // if (grievance_uhmigrate_skiprow()) { continue; }
    ++$processed_rowcount;
    if ($processed_rowcount > $limit) { break; }

    // Clear the cache...
    entity_get_controller('node')->resetCache();

    // Generate an associative array from the input
    $row = array("Row" => $total_rowcount - $rowcount);
    $col = 0;
    foreach ($headers as $header) {
      $row[$header] = $row_raw[$col];
      ++$col;
    }

    if ($row['DateOffList']) { continue; }

    // Find the node to create or update
    $external_id = $row['WorkerID'];
    if (!$external_id) {
      grievance_uhmigrate_log('ERROR', $row, "No external ID (missing WorkerID)");
      continue;
    }
    $worker_node = grievance_uhmigrate_find_by_sirius_id('sirius_worker', $external_id);
    if ($worker_node->nid) { 
      grievance_uhmigrate_log('DEBUG', $row, "Found existing node with $external_id, NID = " . $worker_node->nid . ". It will be updated.");
    } else {
      grievance_uhmigrate_log('DEBUG', $row, "No node found with $external_id. Skipping.");
      continue;
    }

    $employer_id = $row['EmployerID'];
    if (!$employer_id) {
      grievance_uhmigrate_log('ERROR', $row, "No employer");
      continue;
    }
    $employer_node = grievance_uhmigrate_find_by_sirius_id('grievance_shop', $employer_id);
    if (!$employer_node->nid) { 
      grievance_uhmigrate_log('error', $row, "Employer not found.");
      continue;
    }

    $type = $row['NoHireTypeID'];
    if ($type == 1) {
      sirius_dispatch_dnc_set($worker_node, $employer_node, TRUE, $row['Note'], array());
    } else {
      sirius_dispatch_dnc2_set($worker_node, $employer_node, TRUE, $row['Note'], array());
    }
  }

  grievance_uhmigrate_log_summary();
}
