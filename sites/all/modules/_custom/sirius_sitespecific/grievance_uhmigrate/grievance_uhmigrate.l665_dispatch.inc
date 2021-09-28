<?php

function grievance_uhmigrate_l665_dispatch($limit, $source_filename = 'Dispatch.csv') {
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

  $old_ts = strtotime('2019-01-01');
  $rowcount = 0;

  $xyz = variable_get('grievance_uhmigrate_l665_dispatch_xyz', 200000);

  foreach ($rows_raw as $row_raw) {
    ++$rowcount;
    if ($rowcount < $xyz) { continue; }
    // if (grievance_uhmigrate_skiprow()) { continue; }
    ++$processed_rowcount;
    if ($processed_rowcount > $limit) { break; }


    // Generate an associative array from the input
    $row = array("Row" => $rowcount, 'Total' => $total_rowcount);
    $col = 0;
    foreach ($headers as $header) {
      $row[$header] = $row_raw[$col];
      ++$col;
    }

    $ts = strtotime($row['DateAdd']);

    $is_old = ($ts < $old_ts);
    if ($is_old) {
      // print "$rowcount / $total_rowcount: Skipping old dispatch for now.\n";
      // continue;
    }

    // Find the node to create or update
    $external_id = $row['DispatchID'];
    if (!$external_id) {
      grievance_uhmigrate_log('ERROR', $row, "No external ID (missing DispatchID)");
      continue;
    }
    $node = grievance_uhmigrate_find_nid_by_sirius_id('sirius_dispatch', $external_id);
    if (is_numeric($node)) { 
      grievance_uhmigrate_log('DEBUG', $row, "Found existing node with $external_id, NID = $node. It will not be updated.");
      continue;
    } else {
      grievance_uhmigrate_log('DEBUG', $row, "No node found with $external_id. It will be created.");
    }

    // Clear the cache...
    entity_get_controller('node')->resetCache();
    
    // Payrate
    $node->field_sirius_payrate['und'][0]['value'] = $row['PayRate'];

    // Find the job
    $job_id = $row['RequestID2'];
    if (!$job_id) {
      grievance_uhmigrate_log('ERROR', $row, "No Job ID");
      continue;
    }
    $job_node = grievance_uhmigrate_find_by_sirius_id('sirius_dispatch_job', $job_id, 'field_sirius_id2');
    if (!$job_node->nid) { 
      grievance_uhmigrate_log('ERROR', $row, "Job not found for $job_id");
      continue;
    }
    $node->field_sirius_dispatch_job['und'][0]['target_id'] = $job_node->nid;

    // Find the worker
    $worker_id = $row['WorkerID2'];
    if (!$worker_id) {
      grievance_uhmigrate_log('ERROR', $row, "No Worker ID");
      continue;
    }
    $worker_node = grievance_uhmigrate_find_by_sirius_id('sirius_worker', $worker_id);
    if (!$worker_node->nid) { 
      grievance_uhmigrate_log('ERROR', $row, "Worker not found for $worker_id");
      continue;
    }
    $node->field_sirius_worker['und'][0]['target_id'] = $worker_node->nid;

    if ($row['DispatchedAt']) {
      $node->field_sirius_datetime_completed['und'][0]['value'] = date('Y-m-d', strtotime($row['DispatchedAt']));
    }

    $status = 'expired';
    if ($row['DispatchedAt']) { 
      $status = 'terminated';
    }
    if ($is_old && $status == 'accepted') {
      $status = 'terminated';
    }
    if ($row['Cancel'] == 'TRUE') {
      $status = 'declined';
    }
    if ($row['Terminated'] == 'TRUE') {
      $status = 'terminated';
    }
    if ($job_node->field_sirius_dispatch_job_type['und'][0]['value'] == sirius_get_special_tid('sirius_dispatch_job_type|BULL')) {
      $status = 'bullpen';
    }
    $node->field_sirius_dispatch_status['und'][0]['value'] = $status;

    // Save the node
    $nid = $node->nid;
    if (!$nid) { $nid = "[new]"; }
    $node->grievance_no_recurse = TRUE;
    $node->sirius_nohook = TRUE;
    node_object_prepare($node);
    node_submit($node);
    grievance_uhmigrate_node_save($node);
    grievance_uhmigrate_log('SUCCESS', $row, "External ID $external_id, Node ID $nid");

    // grievance_uhmigrate_665cleanup($node, $row);
  }

  variable_set('grievance_uhmigrate_l665_dispatch_xyz', $rowcount - 1);
}
