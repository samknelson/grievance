<?php

function grievance_uhmigrate_l665_jobs($limit, $source_filename = 'Request.csv') {
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
  while ($row_raw = fgetcsv($source_handle, 10000, ",")) {
    $rows_raw[] = $row_raw;
  }
  fclose($source_handle);
  // $rows_raw = array_reverse($rows_raw);
  
  $total_rowcount = count($rows_raw);

  $rowcount = 0;
  foreach ($rows_raw as $row_raw) {
    ++$rowcount;
    if (grievance_uhmigrate_skiprow()) { continue; }
    ++$processed_rowcount;
    if ($processed_rowcount > $limit) { break; }

    // Clear the cache...
    entity_get_controller('node')->resetCache();
    

    // Generate an associative array from the input
    $row = array();
    $col = 0;
    foreach ($headers as $header) {
      $row[$header] = $row_raw[$col];
      ++$col;
    }
    $row['Row'] = $rowcount;
    $row['Total'] = $total_rowcount;

    $ts = strtotime($row['RequestReceived']); 
    if ($ts < strtotime('2/1/2019')) { 
      // print "Skipping because " . $row['RequestReceived'] . "\n";
      // continue;
    }
    
    // Find the node to create or update
    $external_id = $row['RequestID'];
    // if ($external_id != '34053') { continue; }
    if (!$external_id) {
      grievance_uhmigrate_log('ERROR', $row, "No external ID (missing RequestID)");
      print_r($row);
      break;
      continue;
    }
    $node = grievance_uhmigrate_find_by_sirius_id('sirius_dispatch_job', $external_id, 'field_sirius_id2');
    if ($node->nid) { 
      grievance_uhmigrate_log('DEBUG', $row, "Found existing node with $external_id, NID = " . $node->nid . ". It will not be updated.");
      continue;
    } else {
      grievance_uhmigrate_log('DEBUG', $row, "No node found with $external_id. It will be created.");
    }

    // Find the employer
    $employer_id = $row['EmployerID'];
    if (!$employer_id) {
      grievance_uhmigrate_log('ERROR', $row, "No Employer ID");
      continue;
    }
    $employer_node = grievance_uhmigrate_find_by_sirius_id('grievance_shop', $employer_id);
    if (!$employer_node->nid) { 
      grievance_uhmigrate_log('ERROR', $row, "Employer not found for $employer_id");
      continue;
    }
    $node->field_grievance_shop['und'][0]['target_id'] = $employer_node->nid;

    // Find the facility
    $facility_id = $row['LocationID'];
    if ($facility_id) {
      $facility_node = grievance_uhmigrate_find_by_sirius_id('sirius_dispatch_facility', $facility_id);
      if ($facility_node->nid) { 
        $node->field_sirius_dispatch_facility['und'][0]['target_id'] = $facility_node->nid; 
      }
    }

    // Find the hall
    $hall_id = $row['HallID'];
    if ($hall_id) {
      $hall_node = grievance_uhmigrate_find_by_sirius_id('sirius_dispatch_facility_hall', $hall_id);
      if ($hall_node->nid) { 
        $node->field_sirius_dispatch_hall['und'][0]['target_id'] = $hall_node->nid; 
      }
    }

    // Find the request type TID
    if ($row['BullPen'] == 'TRUE') {
      $tid = sirius_get_special_tid('sirius_dispatch_job_type|BULL');
    } else if ($row['RequestTypeID'] == 1) {
      $tid = sirius_get_special_tid('sirius_dispatch_job_type|OPEN');
    } else if ($row['RequestTypeID'] == 2) {
      $tid = sirius_get_special_tid('sirius_dispatch_job_type|CBN');
    } else if ($row['RequestTypeID'] == 3) {
      $tid = sirius_get_special_tid('sirius_dispatch_job_type|FAN');
    } else {
      // grievance_uhmigrate_log('ERROR', $row, "Job type not set: " . $row['RequestTypeID']);
      // continue;
      $tid = sirius_get_special_tid('sirius_dispatch_job_type|OPEN');
    }
    $node->field_sirius_dispatch_job_type['und'][0]['tid'] = $tid;

    // Address notes
    if ($row['Booth']) {
      $node->field_sirius_address_notes['und'][0]['value'] = 'Booth #' . $row['Booth'];
    }

    // Created
    if ($row['RequestReceived']) {
      $node->field_sirius_datetime_created['und'][0]['value'] = date('Y-m-d H:i:s', strtotime($row['RequestReceived']));
    }

    // Report At
    if ($row['ReportAtTime']) {
      $node->field_sirius_datetime['und'][0]['value'] = date('Y-m-d H:i:s', strtotime($row['ReportAtTime']));
    }

    // Call
    if ($row['CallNo']) {
      $node->field_sirius_id['und'][0]['value'] = $row['CallNo'];
    }

    // Count
    if ($row['WorkersRequested'] > 0) {
      $node->field_sirius_count['und'][0]['value'] = $row['WorkersRequested'];
    }

    // Status
    $status = 'pending';
    if ($row['Closed'] == 'TRUE') {
      $status = 'Archived';
    } else if ($row['ApprovalStatusID'] == 1) {
      $status = 'Pending'; 
    } else if ($row['ApprovalStatusID'] == 2) {
      $status = 'Unfilled'; 
    } else if ($row['ApprovalStatusID'] == 3) {
      $status = 'Unfilled';
    }
    $node->field_sirius_dispatch_job_status['und'][0]['value'] = $status;

    // Notes
    $notes_keys = array(
      'ReportToName',
      'ReportToPlace',
      'JobDestination',
      'JobDescription',
      'DispatchNote',
    );
    $notes = '';
    foreach ($notes_keys as $key) {
      if (strlen($row[$key]) > 2) {
        if ($notes) { $notes .= "\n\n"; }
        $notes .= $row[$key];
      }
    }
    $node->field_sirius_notes['und'][0]['value'] = $notes;

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

  // grievance_uhmigrate_log_summary();
}
