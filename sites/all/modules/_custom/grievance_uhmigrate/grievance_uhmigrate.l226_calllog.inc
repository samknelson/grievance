<?php

function grievance_uhmigrate_l226_import_calllog($limit = 100) {
  // Give ourselves more memory
  ini_set('memory_limit', '4096M');

  // User name map -- some users have names entered differently on the two systems
  $user_name_map = array();
  $user_name_map['Anna Rapovy'] = 'Diane Woolman';
  $user_name_map['Bea  Duran'] = 'Bea Duran';
  $user_name_map['Brandon L. Smith'] = 'Brandon Smith';
  $user_name_map['Diane E Woolman'] = 'Diane Woolman';
  $user_name_map['Elizabeth Gutierrez'] = 'Elizabeth Gutierrez';
  $user_name_map['John Dicillo'] = 'John Di Cillo';
  $user_name_map['Nicole A. Quesnell'] = 'Diane Woolman';
  $user_name_map['Raymond J Saldana'] = 'Raymond Saldana';
  $user_name_map['Tony Martin'] = 'Tony Martin';
  $user_name_map['Victor Chicas'] = 'Bea Duran';
  $user_name_map['Brandon L. Smith'] = 'Brandon Smith';
  $user_name_map['John DiCillo'] = 'John Di Cillo';
  $user_name_map['John Dicillo'] = 'John Di Cillo';
  $user_name_map['Raymond J Saldana'] = 'Raymond Saldana';
  $user_name_map['Tony Martin'] = 'Tony Martin';

  // Get the term
	$log_terms = taxonomy_get_term_by_name('Communciation - Grievant - Call to', 'grievance_log_types');  
	if (!$log_terms) {
		die("Unable to find log terms.");
	}
	$log_tids = array_keys($log_terms);
	$log_tid = $log_tids[0];

  // Source file
  $source_filename =  grievance_uhmigrate_base_dir() . '/uh_226/uh_226_calllog.csv';
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
    // if ($row['Row'] != 20494) { continue; }
    $col = 0;
    foreach ($headers as $header) {
      $row[$header] = $row_raw[$col];
      ++$col;
    }

    // Array
    // (
    //     [Row] => 65433
    //     [Recid] => 72695
    //     [Griev#] => 20151853
    //     [Grv_Call_Date] => 5/22/2015
    //     [Grv_Call_Time] => 2:49 PM
    //     [Grv_Call] => Called to let him know his BOA is on 06/09/15 at 9:15 in Flamingo HR.
    //     [Grv_Call_Prsn] => Brandon L Smith
    //     [Grv_Call_Msg] => FALSE
    //     [Grv_Call_SS] =>
    //     [Grv_Last_Upt] => 5/22/2015
    //     [Grv_User] => dwoolman
    //     [Purge] => FALSE
    // )

    // Find the grievance_id
    $grievance_id = $row['Griev#'];
    if (!$grievance_id) {
      grievance_uhmigrate_log('ERROR - NO GRIEVANCE ID', $row, "No grievance_id");
      continue;
    }

    // Find the grievance
    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', 'node');
    $query->entityCondition('bundle', 'grievance');
    $query->fieldCondition('field_grievance_id', 'value', $grievance_id);
    $stmt = $query->execute();
    if (!$stmt['node']) {
      grievance_uhmigrate_log('ERROR - NO GRIEVANCE ID', $row, "Unable to find grievance with id [$grievance_id]");
      continue;
    }
    $grievance_nids = array_keys($stmt['node']);
    $grievance_nid = $grievance_nids[0];
    $grievance_node = node_load($grievance_nid);
    if (!$grievance_node) {
      grievance_uhmigrate_log('ERROR - GRIEVANCE ID MISSING', $row, "Unable to find grievance with id [$grievance_id]");
      continue;
    }

    // Find the external ID
    $external_id = $row['Recid'];
    if (!$external_id) {
      grievance_uhmigrate_log('ERROR - NO RECID', $row, "Unable to find external ID");
      continue;
    }

		$log_node = grievance_uhmigrate_find_by_external_id('grievance_log', $external_id);
		$log_node->field_grievance['und'][0]['target_id'] = $grievance_nid;

		// Set the date, time, and body
		$log_node->body['und'][0]['value'] = $row['Grv_Call'];
		$log_node->field_grievance_status_date['und'][0]['value'] = date('Y-m-d', strtotime($row['Grv_Call_Date']));
		$log_node->field_grievance_time['und'][0]['value'] = timefield_time_to_integer($row['Grv_Call_Time']);
		$log_node->field_grievance_log_type['und'][0]['tid'] = $log_tid;

		// User
	  $user_name = $row['Grv_Call_Prsn'];
	  if ($user_name_map[$user_name]) { $user_name = $user_name_map[$user_name]; }

	  $log_user = NULL;
	  if ($user_name) {
	    $log_user = user_load_by_name($user_name);
	    if (!$log_user) {
	      grievance_uhmigrate_log('WARNING - USER NOT FOUND', $row, "User [$user_name] not found.");
	    } else {
	      $log_node->uid = $log_user->uid;
	    }
	  }


		// Save the node
		node_save($log_node);
    grievance_uhmigrate_log('SUCCESS', $row, "Saved node " . $log_node->nid);

    // Set created
    $ts = strtotime($row['Grv_Call_Date'] . ' ' . $row['Grv_Call_Time']);;
    $sql = "update node set created = :ts where nid = :nid";
    $stmt = db_query($sql, array(':ts' => $ts, ':nid' => $log_node->nid));
  }

  grievance_uhmigrate_log_summary();
}




