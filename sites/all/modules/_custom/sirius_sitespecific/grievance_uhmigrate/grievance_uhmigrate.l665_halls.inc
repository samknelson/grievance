<?php

function grievance_uhmigrate_l665_halls($limit, $source_filename = 'halls.csv') {
  // Give ourselves more memory
  ini_set('memory_limit', '4096M');
  
  $source_filename =  '/var/www/private/migrate/' . $source_filename;
  $source_handle = fopen($source_filename, "r");
  if (!$source_handle) {
    die("Unable to open $source_filename");
  }

  // Headers
  $headers = fgetcsv($source_handle, 1000);

  // Load our rows one at a time
  $rows_raw = array();
  while ($row_raw = fgetcsv($source_handle, 1000, ",")) {
    $rows_raw[] = $row_raw;
  }
  fclose($source_handle);
  $rows_raw = array_reverse($rows_raw);
  
  $total_rowcount = count($rows_raw);

  $rowcount = 0;
  foreach ($rows_raw as $row_raw) {
    // Clear the cache...
    entity_get_controller('node')->resetCache();
    
    ++$rowcount;
    if ($rowcount > $limit) { break; }

    // Generate an associative array from the input
    $row = array("Row" => $total_rowcount - $rowcount);
    $col = 0;
    foreach ($headers as $header) {
      $row[$header] = $row_raw[$col];
      ++$col;
    }

    // Find the node to create or update
    $external_id = $row['HallID'];
    if (!$external_id) {
      grievance_uhmigrate_log('ERROR', $row, "No external ID (missing RecID)");
      continue;
    }
    $node = grievance_uhmigrate_find_by_sirius_id('sirius_dispatch_facility_hall', $external_id);
    if ($node->nid) { 
      grievance_uhmigrate_log('DEBUG', $row, "Found existing node with $external_id, NID = " . $node->nid . ". It will be updated.");
    } else {
      grievance_uhmigrate_log('DEBUG', $row, "No node found with $external_id. It will be created.");
    }

    // Facility
    $facility_id = $row['LocationID'];
    if (!$facility_id) {
      grievance_uhmigrate_log('DEBUG', $row, "No facility ID found; skipping.");
      continue;
    }
    $facility_node = grievance_uhmigrate_find_by_sirius_id('sirius_dispatch_facility', $facility_id);
    if (!$facility_node || !$facility_node->nid) {
      grievance_uhmigrate_log('DEBUG', $row, "No facility node found for $facility_id; skipping.");
      continue;
    }
    $node->field_sirius_dispatch_facility['und'][0]['target_id'] = $facility_node->nid;

    // Title
    $node->title = $row['LongName'];
    if ($row['Abbrv']) {
      $node->field_sirius_name_short['und'][0]['value'] = $row['Abbrv'];
    }

    // Save the grievance node
    $nid = $node->nid;
    if (!$nid) { $nid = "[new]"; }
    $node->grievance_no_recurse = TRUE;
    grievance_uhmigrate_node_save($node);
    grievance_uhmigrate_log('SUCCESS', $row, "External ID $external_id, Node ID $nid, intake $intake_date");

    grievance_uhmigrate_665cleanup($node, $row);
  }

  grievance_uhmigrate_log_summary();
}
