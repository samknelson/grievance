<?php

function grievance_uhmigrate_l665_workers_memberstatus($limit, $source_filename = 'Worker_List.csv') {
  // Give ourselves more memory
  ini_set('memory_limit', '4096M');
  
  $source_filename =  '/var/www/private/migrate/' . $source_filename;
  $source_handle = fopen($source_filename, "r");
  if (!$source_handle) {
    die("Unable to open $source_filename");
  }

  // Headers
  $headers = fgetcsv($source_handle, 10000);
  // $skiprow = fgetcsv($source_handle, 10000);

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
    // if (grievance_uhmigrate_skiprow()) { continue; }
    ++$processed_rowcount;
    if ($processed_rowcount > $limit) { break; }

    // Generate an associative array from the input
    $row = array("Row" => $total_rowcount - $rowcount);
    $col = 0;
    foreach ($headers as $header) {
      $row[$header] = $row_raw[$col];
      ++$col;
    }

    if (strtotime($row['LastUpdate']) < strtotime('2/1/2019')) { continue; }
    print "Last update " . $row['LastUpdate'] . "\n";

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

    $member_status_id = $row['ListID'];
    if (!$member_status_id) {
      grievance_uhmigrate_log('ERROR', $row, "No list ID (missing ListID)");
      continue;
    }
    $member_status_term = grievance_uhmigrate_find_term_by_sirius_id('sirius_member_status', $member_status_id);
    if (!$member_status_term) {
      grievance_uhmigrate_log('ERROR', $row, "No member status found with ID $member_status_id");
      continue;
    }

    $node->sirius_no_recurse = TRUE;
    sirius_worker_member_status_set($node, $member_status_term, TRUE, 'Import', NULL);

    // Save the node
    /*
    $nid = $node->nid;
    if (!$nid) { $nid = "[new]"; }
    $node->grievance_no_recurse = TRUE;
    node_object_prepare($node);
    node_submit($node);
    grievance_uhmigrate_node_save($node);
    grievance_uhmigrate_log('SUCCESS', $row, "External ID $external_id, Node ID $nid");
    */

    // grievance_uhmigrate_665cleanup($node, $row, 'memberstatus');
  }

  grievance_uhmigrate_log_summary();
}
