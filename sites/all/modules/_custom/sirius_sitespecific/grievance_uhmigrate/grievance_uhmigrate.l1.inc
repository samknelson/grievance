<?php

function grievance_uhmigrate_l1_import_grievances($limit, $source_filename, $coerce_contract_template) {
  // Give ourselves more memory
  ini_set('memory_limit', '4096M');
  
  // Source file
  // $source_filename = DRUPAL_ROOT . '/' . drupal_get_path('module', 'grievance_uhmigrate') . "/data/import.csv";
  // $source_filename =  grievance_uhmigrate_base_dir() . '/uh_1/uh_1_grievances.csv';
  $source_filename =  grievance_uhmigrate_base_dir() . '/uh_1/' . $source_filename;
  $source_handle = fopen($source_filename, "r");
  if (!$source_handle) {
    die("Unable to open $source_filename");
  }

  // Headers
  $headers = fgetcsv($source_handle, 1000);

  /*
  Array
  (
      [0] => RecId
      [1] => Griev#
      [2] => GrvLoc
      [3] => GrvCft
      [4] => Violation
      [5] => ViolType
      [6] => CntSubSec
      [7] => ViolDesc
      [8] => ViolCde
      [9] => DispDte
      [10] => DspAction
      [11] => ExpeditedARB
      [12] => Amount
      [13] => Initial
      [14] => In_TakeDte
      [15] => DeleteCde
      [16] => GrievTaker
      [17] => GreivAssign
      [18] => GrievShopSteward
      [19] => GrievAttny
      [20] => GrievSupervisor
      [21] => Union_Lead
      [22] => Occurence_Dte
      [23] => Occurence_Time
      [24] => Email_Addr
      [25] => Interpreter
      [26] => Language
      [27] => Loc_Wrk_Dept
      [28] => Coordinator
      [29] => Date1
      [30] => Date2
      [31] => Date3
      [32] => Date4
      [33] => Date5
      [34] => Date6
      [35] => Date7
      [36] => Date8
      [37] => Date9
      [38] => Date10
      [39] => Date11
      [40] => Date12
      [41] => Date13
      [42] => Date14
      [43] => Date15
      [44] => Date16
      [45] => Date17
      [46] => Date18
      [47] => Date19
      [48] => Date20
      [49] => Time1
      [50] => Text1
      [51] => Text2
      [52] => Text3
      [53] => Check1
      [54] => Check2
      [55] => GrievDOH
      [56] => GrievDayoff1
      [57] => GrievDayoff2
      [58] => GrievDayoff3
      [59] => Phone1
      [60] => Phone2
      [61] => GreivEmplyStat
      [62] => GreivShift
      [63] => Check3
      [64] => Check4
      [65] => Check5
      [66] => YN1
      [67] => Grievant
      [68] => GCustId
      [69] => Disp_Pro
      [70] => Hard_copy_Lost
  )
  */

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

    // Find the node to create or update
    $external_id = $row['RecId'];
    if (!$external_id) {
      grievance_uhmigrate_log('ERROR', $row, "No external ID (missing RecID)");
      continue;
    }
    $grievance_node = grievance_uhmigrate_find_by_external_id('grievance', $external_id);
    if ($grievance_node->nid) { 
      grievance_uhmigrate_log('DEBUG', $row, "Found existing grievance with $external_id. It will be updated.");
    } else {
      grievance_uhmigrate_log('DEBUG', $row, "No grievance found with $external_id. It will be created.");
    }

    // Clear everything that references this node now.  We'll rebuild.
    if ($grievance_node->nid) {
      $query = new EntityFieldQuery();
      $query->entityCondition('entity_type', 'node');
      $query->entityCondition('bundle', array('grievance_attachment', 'grievance_log', 'grievance_settlement'));;
      $query->fieldCondition('field_grievance', 'target_id', $grievance_node->nid);
      $stmt = $query->execute();
      if ($stmt['node']) {
        node_delete_multiple(array_keys($stmt['node']));
      }
    }

    // Find the employer.  Failure is an exception
    $shop_external_id = $row['GrvLoc'];
    if ($shop_map[$shop_external_id]) { $shop_external_id = $shop_map[$shop_external_id]; }

    $shop_node = grievance_uhmigrate_find_by_external_id('grievance_shop', $shop_external_id);
    if (!$shop_node->nid) {
      grievance_uhmigrate_log('ERROR', $row, "Unable to find employer with external id: $shop_external_id");
      continue;
    }
    $grievance_node->field_grievance_shop['und'][0]['target_id'] = $shop_node->nid;

    // Set the contract template
    if ($coerce_contract_template) {
      $contract_tplt_id = 30085;
    } else {
      $contract_tplt_id = $shop_node->field_grievance_contract_tplt['und'][0]['target_id'];
    }
    if ($contract_tplt_id) {
      $grievance_node->field_grievance_contract_tplt['und'][0]['target_id'] = $contract_tplt_id;
    }

    // Grievance ID
    $grievance_id = $row['Griev#'];
    $grievance_node->field_grievance_id['und'][0]['value'] = $grievance_id;

    // Dump everything into "Comments"
    $tmp = print_r($row, 1);
    // $tmp = preg_replace('/[[:cntrl:]]/', '', $tmp);
    $tmp = preg_replace('/[\x00-\x08\x0B\x0C\x0E-\x1F\x7F-\x9F]/u', '', $tmp);
    $grievance_node->field_grievance_comments['und'][0]['value'] = 
      t('Imported from CSV via migration script; last updated on ') . 
      date('r') . 
      "\n\nRaw data:\n" .
      $tmp;

    // Intake date
    $intake_date = NULL;
    if ($row['In_TakeDte'] && (strtotime($row['In_TakeDte']) > 1)) {
      $intake_date = date('Y-m-d 00:00:00', strtotime($row['In_TakeDte']));
      $grievance_node->field_grievance_status_date['und'][0] = array(
        'value' => format_date(strtotime($intake_date), 'custom', 'Y-m-d H:i:s', 'UTC'),
        'timezone' => 'UTC',  
        'timezone_db' => 'UTC',
      );
      $grievance_node->field_grievance_date_1['und'][0] = $grievance_node->field_grievance_status_date['und'][0];
    }
    /*
    if ($row['DispDte'] && strtotime($intake_date) > 0 && strtotime($intake_date) < strtotime('2014-1-1')) {
      grievance_uhmigrate_log('SKIP', $row, "Skipping row because the grievance is closed and the intake date $intake_date is prior to 2014-1-1");
      continue;
    }
    */

    // Incident date
    $incident_date = NULL;
    if ($row['Occurence_Dte'] && (strtotime($row['Occurence_Dte']) > 1)) {
      $incident_date = date('Y-m-d 00:00:00', strtotime($row['Occurence_Dte']));
      $grievance_node->field_grievance_date_1['und'][0] = array(
        'value' => format_date(strtotime($incident_date), 'custom', 'Y-m-d H:i:s', 'UTC'),
        'timezone' => 'UTC',  
        'timezone_db' => 'UTC',
      );
    }

    // 
    /*
    $discipline_date = NULL;
    if ($row['Date11'] && (strtotime($row['Date11']) > 1)) {
      $discipline_date = date('Y-m-d 00:00:00', strtotime($row['Date11']));
      $grievance_node->field_grievance_date_2['und'][0] = array(
        'value' => format_date(strtotime($discipline_date), 'custom', 'Y-m-d H:i:s', 'UTC'),
        'timezone' => 'UTC',  
        'timezone_db' => 'UTC',
      );
    }
    */

    /*
    // User name map -- some users have names entered differently on the two systems
    $user_name_map = array();

    // Intake staff
    $filed_by_name = $row['GrievTaker'];
    if ($user_name_map[$filed_by_name]) { $filed_by_name = $user_name_map[$filed_by_name]; }

    if ($filed_by_name) {
      $filed_by_user = user_load_by_name($filed_by_name);
      if (!$filed_by_user) {
        grievance_uhmigrate_log('WARNING - USER NOT FOUND', $row, "Grievance Specialist [$filed_by_name] not found.");
      } else {
        $grievance_node->field_grievance_rep_filed['und'][0]['target_id'] = $filed_by_user->uid;
      }
    }

    // Grievance Specialist
    $organizer_name = $row['GreivAssign'];
    if ($user_name_map[$organizer_name]) { $organizer_name = $user_name_map[$organizer_name]; }
    if ($organizer_name) {
      $organizer_user = user_load_by_name($organizer_name);
      if (!$organizer_user) {
        grievance_uhmigrate_log('WARNING - USER NOT FOUND', $row, "Intake staff [$organizer_name] not found.");
      } else {
        $grievance_node->field_grievance_rep_organizer['und'][0]['target_id'] = $organizer_user->uid;
      }
    }
    */

    // Member's name
    if ($row['Grievant']) {
      list ($last_name, $first_name, $middle_initial) = explode(';', $row['Grievant']);
      $grievance_node->field_grievance_first_name['und'][0]['value'] = $first_name;
      $grievance_node->field_grievance_last_name['und'][0]['value'] = $last_name;
    }

    // Member's TIMSS ID
    if ($row['GCustId']) {
      $grievance_node->field_grievance_min['und'][0]['value'] = $row['GCustId'];
    }

    // Member's email address
    if ($row['Email_Addr'] && valid_email_address($row['Email_Addr'])) {
      $grievance_node->field_grievance_email['und'][0]['email'] = $row['Email_Addr'];
    }

    // Member's phone
    if ($row['Phone1']) {
      $grievance_node->field_grievance_phone['und'][0]['value'] = $row['Phone1'];
    }

    /*
    // Member's Department
    $department_tid = NULL;
    if ($row['Loc_Wrk_Dept']) {
      $department_terms = taxonomy_get_term_by_name($row['Loc_Wrk_Dept'], 'grievance_department');
      if (!$department_terms) {
        // grievance_uhmigrate_log('WARNING - DEPARTMENT NOT FOUND', $row, "Department " . $row['Loc_Wrk_Dept'] . " not found.");
      } else {
        $tmp = array_keys($department_terms);
        $department_tid = $tmp[0];
        $grievance_node->field_grievance_department_tid['und'][0]['tid'] = $department_tid;
      }
    }

    // Work Status
    if ($row['GreivEmplyStat']) {
      $map = array(
        'PartTime' => 'Part-Time',
        'Stx' => 'Steady Extra',
        'Reg' => 'Regular/Full-Time',
        'Loff' => 'Layoff',
        'Full-Time Flex' => 'Full-Time Flex',
      );
      $work_status_name = $map[$row['GreivEmplyStat']];
      if ($work_status_name) {
        $work_status_terms = taxonomy_get_term_by_name($work_status_name, 'grievance_work_status');
        if (!$work_status_terms) {
          grievance_uhmigrate_log('WARNING - WORK STATUS NOT FOUND', $row, "Work status $work_status_name not found.");
        } else {
          $tmp = array_keys($work_status_terms);
          $work_status_tid = $tmp[0];
          $grievance_node->field_grievance_work_status['und'][0]['tid'] = $work_status_tid;
        }
      }
    }
    */

    // Violation
    $grievance_node->field_grievance_violation['und'][0]['value'] = preg_replace('/[^[:print:]]/', '', $row['Violation']);

    // Shop steward
    // $grievance_node->field_grievance_supervisor_name['und'][0]['value'] = $row['GrievSupervisor'];

    // Save the grievance node
    $nid = $grievance_node->nid;
    if (!$nid) { $nid = "[new]"; }
    $grievance_node->grievance_no_recurse = TRUE;
    grievance_uhmigrate_node_save($grievance_node);
    grievance_uhmigrate_log('SUCCESS', $row, "External ID $external_id, Grievance ID $grievance_id, Node ID $nid, intake $intake_date");

    // Create Settlements
    // grievance_uhmigrate_create_settlements($row, $grievance_node);

    // Create the log
    $final_status_tid = grievance_uhmigrate_l1_create_log($row, $grievance_node);
    if ($final_status_tid) {
      $grievance_node->field_grievance_status['und'][0]['tid'] = $final_status_tid;
      grievance_uhmigrate_node_save($grievance_node);
    }

    // Set the creation date
    $intake_ts = strtotime($intake_date);
    $sql = "update node set created = :intake_ts where nid = :nid";
    $stmt = db_query($sql, array(':intake_ts' => $intake_ts, ':nid' => $grievance_node->nid));
    // print "$sql $intake_ts " . $grievance_node->nid . "\n";

    /*
    if ($row['GrievDayoff1']) {
      print_r($row);
      exit();
    }
    */
  }

  grievance_uhmigrate_log_summary();
}

function grievance_uhmigrate_l1_create_log($row, $grievance_node) {
  // 
  // Log the insert event
  //

  grievance_uhmigrate_create_log_entry(
    $row, 
    $grievance_node, 
    'Grievance - Inserted', 
    '',
    $grievance_node->field_grievance_status_date['und'][0]['value'],
    NULL
  );

  //
  // Log the event date, which starts the timeline.  But only if we don't already have one.
  //

  $event_date = $grievance_node->field_grievance_date_2['und'][0]['value'];
  if (!$event_date) {
    $event_date = $grievance_node->field_grievance_date_1['und'][0]['value'];
  }
  if (!$event_date) {
    $event_date = date('Y-m-d 00:00:00', strtotime('-15 days', strtotime($grievance_node->field_grievance_status_date['und'][0]['value'])));
  }

  grievance_uhmigrate_create_log_entry(
    $row, 
    $grievance_node, 
    'Grievance - Event', 
    '',
    $event_date,
    variable_get('grievance_log_incident_status', '')
  );

  // Load our status TIDs
  $status_vocabulary = taxonomy_vocabulary_machine_name_load('grievance_status');
  $status_terms = taxonomy_get_tree($status_vocabulary->vid);
  $status_lookup = array();
  foreach ($status_terms as $status_term) {
    $status_lookup[$status_term->tid] = $status_term->name;
    $status_lookup[$status_term->name] = $status_term->tid;
  }


  // Dates into status updates
  /*
  $mapping = array(
    'Date1' => 'Grievance letter filed',
    'Date2' => 'Human Resources Meeting',
    'Date3' => 'Human Resources Response',
    'Date4' => 'General Manager Notification',
    'Date5' => 'Step 2 Notification',
    'Date6' => 'Arbitration Notification',
    'Date7' => 'Step 3 Notification',
    'Date8' => 'Step 2 Notification',
    'Date9' => 'Mediation Notification',
    'Date12' => 'Joint Grievance Committee Notification',
    'Date13' => 'Step 2 Notification',
    'Date14' => 'District Manager Step',
    'Date15' => 'Step 3 Notification',
    'Date16' => 'Regional H.R. Director Notification',
    'Date18' => 'Board of Adjustment Notification',
    'Date19' => 'Step 3 Notification',
    'Date20' => 'Step 2 Notification',
  );
  */

  /*
  $final_ts = 0;
  $final_status_tid = NULL;
  foreach ($mapping as $field_name => $status_name) { 
    if ($row[$field_name] && (strtotime($row[$field_name]) > 1)) {
      $date = date('Y-m-d 00:00:00', strtotime($row[$field_name]));
      $status_tid = $status_lookup[$status_name];
      if (!$status_tid) {
        grievance_uhmigrate_log('ERROR - STATUS NOT FOUND', $row, "Status [$status_name] not found.");
        return NULL;
      }
      grievance_uhmigrate_create_log_entry(
        $row, 
        $grievance_node, 
        'Grievance - Updated - With Status Change', 
        '',
        $date,
        $status_tid
      );

      if (strtotime($date) > $final_ts) {
        $final_ts = strtotime($date);
        $final_status_tid = $status_tid;
      }
    }
  }
  */

  /*
  if (!$final_status_tid) {
    // 
    // Create a default "Step 1" status
    //

    $status_name = 'Intake';
    $status_tid = $final_status_tid = $status_lookup[$status_name];

    grievance_uhmigrate_create_log_entry(
      $row, 
      $grievance_node, 
      'Grievance - Updated - With Status Change', 
      '',
      $grievance_node->field_grievance_status_date['und'][0]['value'],
      $status_tid
    );
  }
  */

  /*
For the attached file called "others", please import every greivance.

Based on the "DspAction", 

Everything else will be here:

Based on the "GreivAssign" field, if the value is CH999997 the import with the status "Arbitration Scheduling"; if there is any other value in that field, import with the status "Arbitration Notification" using the "In_TakeDte" plus 30 days for the date.

Please let me know if you have any questions!  
  */

  // Disposition date
  $date = NULL;
  $status_tid = NULL;

  if ($row['DspAction']) {
    // If there's a DspAction, "anything with a value in it should be imported with the status "Case Closed" with a date from the "DispDte" field."
    $date = date('Y-m-d 00:00:00', strtotime($row['DispDte']));
    $status_name = 'Case Closed';
  } else {
    // Based on the "GreivAssign" field, if the value is CH999997 the import with the status "Arbitration Scheduling"; if there is any other value in that field, import with the status "Arbitration Notification" using the "In_TakeDte" plus 30 days for the date.
    $date = date('Y-m-d 00:00:00', strtotime('+30 days', strtotime($row['In_TakeDte'])));
    if ($row['GreivAssign'] == 'CH999997') {
      $status_name = 'Arbitration Scheduling';
    } else {
      $status_name = 'Arbitration Notification';
    }
  }

  $status_tid = $status_lookup[$status_name];
  if (!$status_tid) {
    grievance_uhmigrate_log('ERROR - STATUS NOT FOUND', $row, "Status [$status_name] not found.");
    return NULL;
  }

  grievance_uhmigrate_create_log_entry(
    $row, 
    $grievance_node, 
    'Grievance - Updated - With Status Change', 
    '',
    $date,
    $status_tid
  );

  print "Final status: $status_name\n";
  return $status_tid;
}
