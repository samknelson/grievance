<?php

function grievance_uhmigrate_l1_import_contacts($limit = 0) {
  // Source file
  // $source_filename = DRUPAL_ROOT . '/' . drupal_get_path('module', 'grievance_uhmigrate') . "/data/import.csv";
  $source_filename = grievance_uhmigrate_base_dir() . '/uh_1/uh_1_contacts.csv';

  $source_handle = fopen($source_filename, "r");
  if (!$source_handle) {
    die("Unable to open $source_filename");
  }

  // Headers
  $headers = fgetcsv($source_handle, 1000);
  /*
  Array
  (
    [0] => Employer
    [1] => Shop
    [2] => House Number
    [3] => Contact
    [4] => Title
    [5] => Phone
    [6] => Fax
    [7] => Email
    [8] =>
    [9] =>
    [10] =>
  )
  */

  // Load our rows one at a time
  $rowcount = 0;
  $seen_before = array();
  while ($row_raw = fgetcsv($source_handle, 1000, ",")) {
    ++$rowcount;
    if ($limit && ($rowcount > $limit)) { break; }

    // Generate an associative array from the input
    $row = array("Row" => $rowcount);
    $col = 0;
    foreach ($headers as $header) {
      $row[$header] = $row_raw[$col];
      ++$col;
    }

    // Find the node to create or update
    $external_id = $row['House Number'];
    $shop_node = grievance_uhmigrate_find_by_external_id('grievance_shop', $external_id);

    // Clear everything that references this node now.  We'll rebuild.
    if ($shop_node->nid && !$seen_before[$shop_node->nid]) {
    	$seen_before[$shop_node->nid] = TRUE;
      $query = new EntityFieldQuery();
      $query->entityCondition('entity_type', 'node');
      $query->entityCondition('bundle', array('grievance_shop_contact'));
      $query->fieldCondition('field_grievance_shops', 'target_id', $shop_node->nid);
      $stmt = $query->execute();
      if ($stmt['node']) {
        node_delete_multiple(array_keys($stmt['node']));
      }
    }

    // Shop node title
    $shop_node->title = $row['Shop'];

    // Company
    $company_name = $row['Employer'];
    if ($company_name) {
    	$company_node = grievance_uhmigrate_find_by_title('grievance_company', $company_name);
    	if (!$company_node->nid) {
		    grievance_uhmigrate_node_save($company_node);
		    grievance_uhmigrate_log('SUCCESS', $row, "Created company name $company_name");
    	}
    	$shop_node->field_grievance_company['und'][0]['target_id'] = $company_node->nid;
    }

    // Save the shop node
    $nid = $shop_node->nid;
    if (!$nid) { $nid = "[new]"; }
    grievance_uhmigrate_node_save($shop_node);
    grievance_uhmigrate_log('SUCCESS', $row, "Created shop external ID $external_id, Shop NID $nid");

    // Save the contact
	  global $user;
	  $contact_node = new stdClass();
	  $contact_node->type = "grievance_shop_contact";
	  node_object_prepare($contact_node);
	  $contact_node->language = LANGUAGE_NONE;
	  $contact_node->uid = $user->uid; 
	  $contact_node->status = 1;
	  $contact_node->promote = 0;
	  $contact_node->comment = 0;
	  $contact_node->title = $title;

	  $contact_node->field_grievance_co_name['und'][0]['value'] = $row['Contact'];
	  $contact_node->field_grievance_co_role['und'][0]['value'] = $row['Title'];
	  $contact_node->field_grievance_co_phone['und'][0]['value'] = $row['Phone'];
	  $contact_node->field_grievance_co_fax['und'][0]['value'] = $row['Fax'];
	  $contact_node->field_grievance_co_email['und'][0]['email'] = $row['Email'];
	  $contact_node->field_grievance_shops['und'][0]['target_id'] = $shop_node->nid;

    grievance_uhmigrate_node_save($contact_node);
    grievance_uhmigrate_log('SUCCESS', $row, "Created contact " . $row['Contact'] . ' NID ' . $contact_node->nid);
  }
  fclose($source_handle);
}