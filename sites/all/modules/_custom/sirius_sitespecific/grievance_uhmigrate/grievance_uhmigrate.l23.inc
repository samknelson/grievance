<?php

function grievance_uhmigrate_l23_import_grievances($limit = 100) {
  // Give ourselves more memory
  ini_set('memory_limit', '4096M');
  
  // Source file
  // $source_filename = DRUPAL_ROOT . '/' . drupal_get_path('module', 'grievance_uhmigrate') . "/data/import.csv";
  $source_filename =  grievance_uhmigrate_base_dir() . '/uh_23/uh_23_grievances.csv';
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
      [6] => Discipline Date
      [7] => DspAction
      [8] => Amount
      [9] => Initial
      [10] => In_TakeDte
      [11] => DeleteCde
      [12] => GrievTaker
      [13] => GreivAssign
      [14] => GrievShopSteward
      [15] => Occurence_Dte
      [16] => Occurence_Time
      [17] => Email_Addr
      [18] => Loc_Wrk_Dept
      [19] => 1st Step Response
      [20] => 2nd Step letter sent
      [21] => Mediation Letter Sent
      [22] => FMCS Panel
      [23] => 3rd Step Response
      [24] => Information Requested
      [25] => 3rd Step Meeting
      [26] => 3rd Step Letter Sent
      [27] => 1st Step Meeting
      [28] => 1st Step Letter sent
      [29] => Date11
      [30] => 2nd Step Meeting
      [31] => 2nd Step Meeting
      [32] => 2nd Step Response
      [33] => Mediation Meeting
      [34] => Phone1
      [35] => Grievant
      [36] => GCustId
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
    $grievance_node = grievance_uhmigrate_find_by_external_id('grievance', $external_id);

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

    // Some shops are other shops.
    // Standard Parking
    $shop_map['AP031875'] = 'BBH00027';
    $shop_map['BBH00027'] = 'BBH00027';
    $shop_map['APH31914'] = 'BBH00027';
    $shop_map['APH31948'] = 'BBH00027';
    $shop_map['APH31950'] = 'BBH00027';
    $shop_map['APH31956'] = 'BBH00027';
    $shop_map['APH31939'] = 'BBH00027';
    $shop_map['APH31924'] = 'BBH00027';
    $shop_map['APH31860'] = 'BBH00027';
    $shop_map['APH31940'] = 'BBH00027';
    $shop_map['APH31849'] = 'BBH00027';
    $shop_map['APH31949'] = 'BBH00027';
    $shop_map['APH31954'] = 'BBH00027';
    $shop_map['APH31952'] = 'BBH00027';
    $shop_map['APH31850'] = 'BBH00027';
    $shop_map['APH31857'] = 'BBH00027';
    $shop_map['APH31951'] = 'BBH00027';
    $shop_map['APH31847'] = 'BBH00027';
    $shop_map['APH31859'] = 'BBH00027';
    $shop_map['APH31938'] = 'BBH00027';
    $shop_map['APH31936'] = 'BBH00027';
    $shop_map['APH31957'] = 'BBH00027';
    $shop_map['APH31955'] = 'BBH00027';
    $shop_map['APH31919'] = 'BBH00027';
    $shop_map['APH31903'] = 'BBH00027';
    $shop_map['APH31863'] = 'BBH00027';
    $shop_map['APH31937'] = 'BBH00027';
    $shop_map['APH31941'] = 'BBH00027';
    $shop_map['APH31926'] = 'BBH00027';
    $shop_map['APH31854'] = 'BBH00027';
    $shop_map['APH31855'] = 'BBH00027';
    $shop_map['APH31890'] = 'BBH00027';
    $shop_map['APH31902'] = 'BBH00027';
    $shop_map['APH31856'] = 'BBH00027';
    $shop_map['APH31892'] = 'BBH00027';
    $shop_map['APH31945'] = 'BBH00027';
    $shop_map['APH31874'] = 'BBH00027';
    $shop_map['APH31871'] = 'BBH00027';
    $shop_map['APH31953'] = 'BBH00027';
    $shop_map['APH31923'] = 'BBH00027';
    $shop_map['APH31858'] = 'BBH00027';
    $shop_map['APH31862'] = 'BBH00027';
    $shop_map['APH31946'] = 'BBH00027';
    $shop_map['APH31935'] = 'BBH00027';
    $shop_map['APH31891'] = 'BBH00027';
    $shop_map['APH31958'] = 'BBH00027';
    $shop_map['APH31943'] = 'BBH00027';
    $shop_map['APH31944'] = 'BBH00027';
    $shop_map['APH31888'] = 'BBH00027';
    $shop_map['APH31895'] = 'BBH00027';
    $shop_map['APH31886'] = 'BBH00027';
    $shop_map['APH31887'] = 'BBH00027';
    $shop_map['APH31911'] = 'BBH00027';
    $shop_map['APH31912'] = 'BBH00027';
    $shop_map['APH31913'] = 'BBH00027';
    $shop_map['APH31920'] = 'BBH00027';
    $shop_map['APH31918'] = 'BBH00027';
    $shop_map['APH31838'] = 'BBH00027';
    $shop_map['APH31839'] = 'BBH00027';
    $shop_map['APH31845'] = 'BBH00027';
    $shop_map['APH31840'] = 'BBH00027';
    $shop_map['APH31846'] = 'BBH00027';
    $shop_map['APH31841'] = 'BBH00027';
    $shop_map['APH31842'] = 'BBH00027';
    $shop_map['APH31907'] = 'BBH00027';
    $shop_map['APH31927'] = 'BBH00027';
    $shop_map['APH31915'] = 'BBH00027';
    $shop_map['APH31932'] = 'BBH00027';
    $shop_map['APH31916'] = 'BBH00027';
    $shop_map['APH31928'] = 'BBH00027';
    $shop_map['APH31880'] = 'BBH00027';
    $shop_map['APH31881'] = 'BBH00027';
    $shop_map['APH31876'] = 'BBH00027';
    $shop_map['APH31877'] = 'BBH00027';
    $shop_map['APH31882'] = 'BBH00027';
    $shop_map['APH31878'] = 'BBH00027';
    $shop_map['APH31883'] = 'BBH00027';
    $shop_map['APH31884'] = 'BBH00027';
    $shop_map['APH31879'] = 'BBH00027';
    $shop_map['APH31908'] = 'BBH00027';
    $shop_map['APH31931'] = 'BBH00027';
    $shop_map['APH31864'] = 'BBH00027';
    $shop_map['APH31868'] = 'BBH00027';
    $shop_map['APH31869'] = 'BBH00027';
    $shop_map['APH31870'] = 'BBH00027';
    $shop_map['APH31899'] = 'BBH00027';
    $shop_map['APH31900'] = 'BBH00027';
    $shop_map['APH31889'] = 'BBH00027';
    $shop_map['APH31909'] = 'BBH00027';
    $shop_map['APH31851'] = 'BBH00027';
    $shop_map['APH31853'] = 'BBH00027';
    $shop_map['APH31852'] = 'BBH00027';
    $shop_map['APH31894'] = 'BBH00027';
    $shop_map['APH31933'] = 'BBH00027';
    $shop_map['APH31934'] = 'BBH00027';
    $shop_map['APH31960'] = 'BBH00027';
    $shop_map['APH31901'] = 'BBH00027';
    $shop_map['APH31917'] = 'BBH00027';
    $shop_map['APH31897'] = 'BBH00027';
    $shop_map['APH31898'] = 'BBH00027';
    $shop_map['APH31861'] = 'BBH00027';
    $shop_map['APH31904'] = 'BBH00027';
    $shop_map['APH31905'] = 'BBH00027';
    $shop_map['APH31929'] = 'BBH00027';
    $shop_map['APH31959'] = 'BBH00027';
    $shop_map['APH31848'] = 'BBH00027';
    $shop_map['APH31922'] = 'BBH00027';
    $shop_map['APH31893'] = 'BBH00027';
    $shop_map['APH31837'] = 'BBH00027';
    $shop_map['APH31910'] = 'BBH00027';
    $shop_map['APH31921'] = 'BBH00027';
    $shop_map['APH31873'] = 'BBH00027';
    $shop_map['APH31872'] = 'BBH00027';
    $shop_map['APH31947'] = 'BBH00027';
    $shop_map['APH31906'] = 'BBH00027';
    $shop_map['APH31930'] = 'BBH00027';

    // Delaware North Austin
    $shop_map['APH00001'] = 'HSH01261';

    // Westin Denver
    $shop_map['APH00074'] = 'APH00074';

    // Standard Parking
    $shop_map['BBH02701'] = 'BBH00027';

    // Aramark Phillip Morris
    $shop_map['WAH0144B'] = 'WAH00144';

    // Closed
    $shop_map['HSH00100'] = 'ZZZCLOSED';
    $shop_map['WAH00057'] = 'ZZZCLOSED';
    $shop_map['WAH00197'] = 'ZZZCLOSED';
    $shop_map['WAH00497'] = 'ZZZCLOSED';
    $shop_map['APH36111'] = 'ZZZCLOSED';

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
    $contract_tplt_id = $shop_node->field_grievance_contract_tplt['und'][0]['target_id'];
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

    // Discipline date
    $discipline_date = NULL;
    if ($row['Date11'] && (strtotime($row['Date11']) > 1)) {
      $discipline_date = date('Y-m-d 00:00:00', strtotime($row['Date11']));
      $grievance_node->field_grievance_date_2['und'][0] = array(
        'value' => format_date(strtotime($discipline_date), 'custom', 'Y-m-d H:i:s', 'UTC'),
        'timezone' => 'UTC',  
        'timezone_db' => 'UTC',
      );
    }

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
    grievance_uhmigrate_create_settlements($row, $grievance_node);

    // Create the log
    $final_status_tid = grievance_uhmigrate_l23_create_log($row, $grievance_node);
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

function grievance_uhmigrate_l23_create_log($row, $grievance_node) {
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
  $mapping = array(
    '1st Step Response' => 'Step 1 Responded',
    '2nd Step letter sent' => 'Step 2 Filed',
    'Mediation Letter Sent' => 'Mediation Filed',
    '3rd Step Response' => 'Step 3 Responded',
    '3rd Step Meeting' => 'Step 3 Meeting',
    '3rd Step Letter Sent' => 'Step 3 Filed',
    '1st Step Meeting' => 'Step 1 Meeting',
    '1st Step Letter sent' => 'Step 1 Filed',
    '2nd Step Meeting' => 'Step 2 Meeting',
    '2nd Step Meeting 2' => 'Step 2 Meeting',
    '2nd Step Response' => 'Step 2 Responded',
    'Mediation Meeting' => 'Mediation Responded',
  );

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

  if (!$final_status_tid) {
    // 
    // Create a default "Step 1" status
    //

    $status_name = 'Step 1 Filed';
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

  // Disposition date
  $disp_date = NULL;
  if ($row['Discipline Date']) {
    $disp_date = date('Y-m-d 00:00:00', strtotime($row['Discipline Date']));
  }

  // Close the grievance
  if ($disp_date) {

    $final_status_name = $status_lookup[$final_status_tid];
    $status_name = NULL;
    $status_tid = NULL;

    if (preg_match('/Step 1/i', $final_status_name) && $row['DspAction'])                { $status_name = 'Step 1 Resolved'; } 
    else if (preg_match('/Step 2/i', $final_status_name) && $row['DspAction'])           { $status_name = 'Step 2 Resolved'; } 
    else if (preg_match('/Step 3/i', $final_status_name) && $row['DspAction'])           { $status_name = 'Step 3 Resolved'; } 
    else if (preg_match('/Arbitration/i', $final_status_name) && $row['DspAction'])      { $status_name = 'Arbitration Resolved'; } 
    else if (preg_match('/Mediation/i', $final_status_name) && $row['DspAction'])        { $status_name = 'Mediation Resolved'; } 
    else                                                                                 { $status_name = 'Step 1 Resolved'; } 


    if (!$status_name) {
      grievance_uhmigrate_log('ERROR - CANT CLOSE', $row, "Don't know how to close [$final_status_name] with disposition [" . $row['DspAction'] . "].");
      return NULL;
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
      $disp_date,
      $status_tid
    );

    $final_ts = strtotime($disp_date);
    $final_status_tid = $status_tid;
  }

  // Create "Closed By" log entry
  if ($row['Initial']) {
    grievance_uhmigrate_create_log_entry(
      $row, 
      $grievance_node, 
      'Internal - Comment', 
      'Grievance closed by ' . $row['Initial'],
      $disp_date
    );
  }

  return $final_status_tid;
}
