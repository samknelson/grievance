<?php

function grievance_uhmigrate_l665_workers_image($limit, $source_filename = 'Worker.csv') {
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
  foreach ($rows_raw as $row_raw) {
    // Clear the cache...
    entity_get_controller('node')->resetCache();
    
    ++$rowcount;
    if ($rowcount > $limit) { break; }

    // Generate an associative array from the input
    $row = array("Row" => $total_rowcount - $rowcount);
    foreach ($headers as $col => $header) {
      $row[$header] = $row_raw[$col];
    }
    if (!$row['ImageFile']) { continue; }
    $x = $total_rowcount - $rowcount;
    if ($x > 2920) { continue; }

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
      grievance_uhmigrate_log('DEBUG', $row, "No node found with $external_id. It will be created.");
    }

    $source = "/var/www/private/migrate/Photos/" . $row['ImageFile'];
    if (!file_exists($source)) {
      grievance_uhmigrate_log('DEBUG', $row, "Can't find file at $source. Skipping.");
      continue;
    }

    $ext = pathinfo($source, PATHINFO_EXTENSION);
    $file_temp = file_save_data(file_get_contents($source), 'private://incoming_' . $node->nid . ".$ext", FILE_EXISTS_REPLACE);
    $node->field_sirius_headshot['und'][0] = (array) $file_temp;
    node_object_prepare($node);
    node_submit($node);
    node_save($node);

    print "Saved: $source\n";
  }
}
