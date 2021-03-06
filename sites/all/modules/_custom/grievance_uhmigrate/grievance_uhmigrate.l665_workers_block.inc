<?php

function grievance_uhmigrate_l665_workers_block($limit, $source_filename = 'NoSignIn.csv') {
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

  $xyz = variable_get('grievance_uhmigrate_l665_workers_block', 0);
  $rowcount = 0;
  $woohoo = 0;
  foreach ($rows_raw as $row_raw) {
    // Clear the cache...
    entity_get_controller('node')->resetCache();
    
    ++$rowcount;
    // if ($rowcount < $xyz) { continue; }
    ++$processed_rowcount;
    if ($processed_rowcount > $limit) { break; }

    // Generate an associative array from the input
    $row = array("Row" => $rowcount, 'Total' => $total_rowcount);
    $col = 0;
    foreach ($headers as $header) {
      $row[$header] = $row_raw[$col];
      ++$col;
    }

    if ($row['DateOffList']) {
      print "Off list\n";
      continue;
    }

    // Find the node to create or update
    $external_id = $row['WorkerID'];
    if (!$external_id) {
      grievance_uhmigrate_log('ERROR', $row, "No external ID (missing RequestID)");
      continue;
    }
    $node = grievance_uhmigrate_find_by_sirius_id('sirius_worker', $external_id);
    if ($node->nid) { 
      grievance_uhmigrate_log('DEBUG', $row, "Found existing node with $external_id, NID = " . $node->nid . ". It will be updated.");
    } else {
      grievance_uhmigrate_log('DEBUG', $row, "No node found with $external_id. Skipping.");
      continue;
    }

    if ($node->field_sirius_dispatch_sib['und'][0]['value'] == 'Yes') { 
      print "Already blocked.\n"; 
      continue;
    }

    $node->field_sirius_dispatch_sib['und'][0]['value'] = 'Yes';
    $node->grievance_no_recurse = TRUE;
    node_object_prepare($node);
    node_submit($node);
    grievance_uhmigrate_node_save($node);
    grievance_uhmigrate_log('SUCCESS', $row, "External ID $external_id, Node ID " . $node->nid);
    ++$woohoo;

    // grievance_uhmigrate_665cleanup($node, $row, 'phone');
  }

  $xyz = variable_set('grievance_uhmigrate_l665_workers_block', $rowcount);

  print "Woo Hoo: $woohoo\n";
}
