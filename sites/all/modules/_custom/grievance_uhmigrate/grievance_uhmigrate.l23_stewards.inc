<?php

function grievance_uhmigrate_l23_stewards_import($limit = 100) {
  // Give ourselves more memory
  ini_set('memory_limit', '4096M');

  // Source file
  $source_filename =  grievance_uhmigrate_base_dir() . '/uh_23/uh_23_stewards.csv';
  $source_handle = fopen($source_filename, "r");
  if (!$source_handle) {
    die("Unable to open $source_filename");
  }

  // Headers
  $headers = fgetcsv($source_handle, 1000);
  print_r($headers);

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
    //     [0] => name
    //     [1] => timss_contact
    //     [2] => chapter
    //     [3] => timss_shop
    //     [4] => shop_name
    //     [5] => dept
    //     [6] => email
    //     [7] => phone
    //     [8] => mobile
    //     [9] => address
    //     [10] => Address_2
    //     [11] => city
    //     [12] => state
    //     [13] => zip
    // )

    // Find the shops
    $timss_shops = explode(';', $row['timss_shop']);
    if (!$timss_shops) {
      grievance_uhmigrate_log('ERROR - NO TIMSS_SHOP', $row, "The timss_shop column is empty.");
      continue;
    }
    $shop_nodes = array();
    foreach ($timss_shops as $timss_shop) {
      $timss_shop = trim($timss_shop);
      $shop_node = grievance_uhmigrate_find_by_external_id('grievance_shop', $timss_shop);
      if (!$shop_node->nid) {
        grievance_uhmigrate_log('ERROR - SHOP NOT FOUND', $row, "Unable to find the shop with TIMSS ID [$timss_shop]");
        // continue;
      } else {
        $shop_nodes[] = $shop_node;
      }
    }

    // Find the contact
    $timss_contact = $row['timss_contact'];
		$contact_node = grievance_uhmigrate_find_by_external_id('grievance_shop_contact', $timss_contact);

    // Set the variables
    $contact_node->field_grievance_co_name['und'][0]['value'] = trim($row['name']);
    $contact_node->field_grievance_co_address['und'][0]['value'] = trim($row['address']);
    $contact_node->field_grievance_co_address_2['und'][0]['value'] = trim($row['Address_2']);
    $contact_node->field_grievance_co_city['und'][0]['value'] = trim($row['city']);
    $contact_node->field_grievance_co_state['und'][0]['value'] = trim($row['state']);
    $contact_node->field_grievance_co_zip['und'][0]['value'] = trim($row['zip']);
    $row['email'] = preg_replace('/\s/', '', $row['email']);
    $contact_node->field_grievance_co_email['und'][0]['email'] = trim($row['email']);

    $contact_node->field_grievance_co_phone['und'][0]['value'] = trim($row['phone']);
    $contact_node->field_grievance_co_phone_2['und'][0]['value'] = trim($row['mobile']);

    $contact_node->field_grievance_min['und'][0]['value'] = $row['timss_contact'];

    $contact_node->field_grievance_shops['und'] = array();
    foreach ($shop_nodes as $shop_node) {
      $contact_node->field_grievance_shops['und'][] = array('target_id' => $shop_node->nid);
    }
    $contact_node->field_grievance_contact_types = array(
      'und' => array(
        array('tid' => 282)
      )
    );

		// Save the node
		node_save($contact_node);
    grievance_uhmigrate_log('SUCCESS', $row, "Saved node " . $contact_node->nid);
  }

  grievance_uhmigrate_log_summary();
}




