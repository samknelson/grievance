<?php

function grievance_uhmigrate_l23_contacts_import_destructive($limit = 100) {
  // Give ourselves more memory
  ini_set('memory_limit', '4096M');

  // Source file
  $source_filename =  grievance_uhmigrate_base_dir() . '/uh_23/uh_23_contacts.csv';
  $source_handle = fopen($source_filename, "r");
  if (!$source_handle) {
    die("Unable to open $source_filename");
  }

  // delete everything (that's the destructive part)
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'grievance_shop_contact');
  $stmt = $query->execute();
  if ($stmt['node']) {
    $nids = array_keys($stmt['node']);
    node_delete_multiple($nids);
  }

  // Headers
  $headers = fgetcsv($source_handle, 1000);

  // Load our rows one at a time
  $rows_raw = array();
  while ($row_raw = fgetcsv($source_handle, 1000, ",")) {
    $rows_raw[] = $row_raw;
  }
  fclose($source_handle);
  
  $total_rowcount = count($rows_raw);

  $rowcount = 0;
  foreach ($rows_raw as $row_raw) {
    // Clear the cache...
    entity_get_controller('node')->resetCache();
    
    ++$rowcount;
    if ($rowcount > $limit) { break; }

    // Generate an associative array from the input
    $row = array("Row" => $rowcount);
    // if ($row['Row'] != 20494) { continue; }
    $col = 0;
    foreach ($headers as $header) {
      $row[$header] = $row_raw[$col];
      ++$col;
    }

    // Array
    // (
    //     [Row] => 71
    //     [Company] => Compass
    //     [Employer] => University of New Mexico
    //     [Name] =>  Ms. Michalle Thompson
    //     [Role] => Labor Relations Manager/Labor Counsel
    //     [Phone] => (714) 461-9816
    //     [Email] => Michalle.Thompson@compass-usa.com
    //     [Edit] => edit
    // )

    // Find the employer
    $employers = $row['Employer'];
    $employers = preg_replace('/HMS Host Atlanta, GA/', '!!!', $employers);
    $employers = preg_replace('/Gate Safe/', 'Gate Safe Houston, Gate Safe San Antonio', $employers);
    $employers = preg_replace('/British Airways$/', 'British Airways Dulles Lounge', $employers);
    $employers = preg_replace('/British Airways,/', 'British Airways Dulles Lounge,', $employers);
    $employers = preg_replace('/^African American History Museum/', 'Smithsonian African American History Museum', $employers);
    $employers = preg_replace('/, African American History Museum/', ', Smithsonian African American History Museum', $employers);
    $employers = explode(',', $employers);
    foreach ($employers as &$employer) {
      $employer = trim($employer);
      $employer = preg_replace('/!!!/', 'HMS Host Atlanta, GA', $employer);
   } 

    $shop_nids = array();
    foreach ($employers as $employer) {
      $query = new EntityFieldQuery();
      $query->entityCondition('entity_type', 'node');
      $query->entityCondition('bundle', 'grievance_shop');
      $query->propertyCondition('title', $employer);
      $stmt = $query->execute();
      if (!$stmt['node']) {
        grievance_uhmigrate_log('ERROR - EMPLOYER NOT FOUND', $row, "Unable to find employer: [$employer]");
        continue;
      }
      $tmp = array_keys($stmt['node']);
      $shop_nids[] = $tmp[0];
    }
		$contact_node = grievance_uhmigrate_find_by_external_id('grievance_shop_contact', 'external-id-not-defined-because-this-is-destructive-import');
    $contact_node->field_grievance_co_name['und'][0]['value'] = trim($row['Name']);
    $contact_node->field_grievance_co_role['und'][0]['value'] = trim($row['Role']);
    $contact_node->field_grievance_co_phone['und'][0]['value'] = trim($row['Phone']);
    $row['Email'] = preg_replace('/\s/', '', $row['Email']);

    $contact_node->field_grievance_co_email['und'][0]['email'] = trim($row['Email']);
    print "Here with [" . trim($row['Email']) . "]\n";
    $contact_node->field_grievance_shops['und'] = array();
    foreach ($shop_nids as $shop_nid) {
      $contact_node->field_grievance_shops['und'][] = array('target_id' => $shop_nid);
    }

		// Save the node
		node_save($contact_node);
    grievance_uhmigrate_log('SUCCESS', $row, "Saved node " . $log_node->nid);
  }

  grievance_uhmigrate_log_summary();
}




