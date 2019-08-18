<?php

function grievance_uhmigrate_l665_workers($limit, $source_filename = 'Worker.csv') {
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
    $row = array();
    foreach ($headers as $col => $header) {
      $row[$header] = $row_raw[$col];
    }
    if (strtotime($row['LastUpdate']) < strtotime('2/5/2019')) {
      print "Not modified recently: " . $row['LastUpdate'] . ". Skipping.\n";
      continue;
    }

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

    // SSN
    $ssn = $row['_SSN'];
    $node->field_sirius_ssn['und'][0]['value'] = $ssn;

    // Name
    $node->field_sirius_name['und'][0]['given'] = $row['FirstName'];
    $node->field_sirius_name['und'][0]['middle'] = $row['MiddleInit'];
    $node->field_sirius_name['und'][0]['family'] = $row['LastName'];

    // Address
    $node->field_sirius_address['und'][0]['country'] = 'US';
    $node->field_sirius_address['und'][0]['thoroughfare'] = $row['Address1'];
    $node->field_sirius_address['und'][0]['premise'] = $row['Address2'];
    $node->field_sirius_address['und'][0]['locality'] = $row['City'];
    $node->field_sirius_address['und'][0]['postal_code'] = $row['ZipCode'];
    $node->field_sirius_address['und'][0]['administrative_area'] = $row['State'];

    // Email
    if ($row['Email']) {
      $node->field_sirius_email['und'][0]['value'] = $row['Email'];
    }

    // Gender
    if ($row['Gender'] == 'M') {
      $node->field_sirius_gender['und'][0]['tid'] = 450;
    } else if ($row['Gender'] == 'F') {
      $node->field_sirius_gender['und'][0]['tid'] = 451;
    }

    // Titan ID
    if ($row['TITAN_MemberNo']) {
      $node->field_sirius_id2['und'][0]['value'] = $row['TITAN_MemberNo'];
    }

    // aat
    if ($row['Password']) {
      $node->field_sirius_aat['und'][0]['value'] = $row['Password'];
    }

    // Save the node
    $nid = $node->nid;
    if (!$nid) { $nid = "[new]"; }
    $node->grievance_no_recurse = TRUE;
    node_object_prepare($node);
    node_submit($node);
    grievance_uhmigrate_node_save($node);
    grievance_uhmigrate_log('SUCCESS', $row, "External ID $external_id, Node ID $nid");

    // grievance_uhmigrate_665cleanup($node, $row);
  }

  grievance_uhmigrate_log_summary();
}
