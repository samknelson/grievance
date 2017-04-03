<?php







function grievance_uhmigrate_l226_import_additional_violations($limit = 100) {
  // Give ourselves more memory
  ini_set('memory_limit', '4096M');
  
  // Source file
  // $source_filename = DRUPAL_ROOT . '/' . drupal_get_path('module', 'grievance_uhmigrate') . "/data/import.csv";
  $source_filename =  grievance_uhmigrate_base_dir() . '/uh_226/uh_226_additional_violations.csv';
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

    // Find the grievance_id
    $grievance_id = $row['TGrev#'];
    $row['Griev#'] = $row['TGrev#'];
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

    // Find the shop
    $shop_nid = $grievance_node->field_grievance_shop['und'][0]['target_id'];
    if (!$shop_nid) {
      grievance_uhmigrate_log('ERROR - NO SHOP', $row, "No shop in the node.");
      continue;
    }
    $shop_node = node_load($shop_nid);
    if (!$shop_node) {
      grievance_uhmigrate_log('ERROR - SHOP NOT FOUND', $row, "Shop node not found.");
      continue;
    }

    // Find the clauses
    $row['Violation'] = $row['TViolation'];
    $clause_nids = grievance_uhmigrate_find_violation_nid($row, $shop_node);
    $need_save = FALSE;
    foreach ($clause_nids as $clause_nid) {
      $found = FALSE;
      if ($grievance_node->field_grievance_clauseref['und']) {
        foreach ($grievance_node->field_grievance_clauseref['und'] as $tmp) {
          if ($tmp['target_id'] == $clause_nid) {
            $found = TRUE;
          }
        }
      }
      if (!$found) {
        $grievance_node->field_grievance_clauseref['und'][] = array('target_id' => $clause_nid);
        $tmp = print_r($row, 1);
        $tmp = preg_replace('/[\x00-\x08\x0B\x0C\x0E-\x1F\x7F-\x9F]/u', '', $tmp);
        $grievance_node->field_grievance_comments['und'][0]['value'] .= $tmp;
        $need_save = TRUE;
      }
    }
    if ($need_save) {
      grievance_uhmigrate_log('SUCCESS', $row, "Added clauses to " . $grievance_node->nid);
      node_save($grievance_node);
    } else {
      grievance_uhmigrate_log('SUCCESS', $row, "Clauses already set for " . $grievance_node->nid);
    }
  }

  grievance_uhmigrate_log_summary();
}















function grievance_uhmigrate_l226_import_grievances($limit = 100) {
  // Give ourselves more memory
  ini_set('memory_limit', '4096M');
  
  // Source file
  // $source_filename = DRUPAL_ROOT . '/' . drupal_get_path('module', 'grievance_uhmigrate') . "/data/import.csv";
  $source_filename =  grievance_uhmigrate_base_dir() . '/uh_226/uh_226_grievances.csv';
  $source_handle = fopen($source_filename, "r");
  if (!$source_handle) {
    die("Unable to open $source_filename");
  }

  // Headers
  $headers = fgetcsv($source_handle, 1000);

  /*
  Array
  (
      [0] => RecId                // External ID
      [1] => Griev#               // Grievance Number, aka "Grievance ID"
      [2] => GrvLoc               // External ID for the shop
      [3] => GrvCft               // DON'T IMPORT
      [4] => Violation            // Match to the CBA as best as possible
      [5] => ViolType             // DON'T IMPORT
      [6] => CntSubSec            // DON'T IMPORT
      [7] => ViolDesc             // DON'T IMPORT
      [8] => ViolCde              // DON'T IMPORT
      [9] => DispDte              // @TODO Date of final disposition
      [10] => DspAction           // @TODO Action of final disposition.  Need to combine with the various date fields to figure out the statuses
      [11] => ExpeditedARB        // @TODO TRUE or FALSE
      [12] => Amount              // Settlement Amount
      [13] => Initial             // Initials of the person who scanned the closure documents.  Create a log entry "Closed by: [initials]"
      [14] => In_TakeDte          // Intake Date
      [15] => DeleteCde           // Marked for deletion.  Ignore.
      [16] => GrievTaker          // Intake Staff, only import the users who exist
      [17] => GreivAssign         // Grievance Specialist, only import the users who exist
      [18] => GrievShopSteward    // Don't import
      [19] => GrievAttny          // Don't import
      [20] => GrievSupervisor     // Import into "Supervisor" field
      [21] => Union_Lead          // Don't import
      [22] => Occurence_Dte       // Import as incident date
      [23] => Occurence_Time      // Don't import
      [24] => Email_Addr          // Member's email address
      [25] => Interpreter         // Don't import
      [26] => Language            // Don't import
      [27] => Loc_Wrk_Dept        // Department
      [28] => Coordinator         // Don't import
      [29] => Date1               // BOA Letter Date
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
      [44] => Time1
      [45] => Text1
      [46] => Text2
      [47] => Text3
      [48] => Check1
      [49] => Check2
      [50] => GrievDOH            // Date of Hire
      [51] => GrievDayoff1        // Days Off
      [52] => GrievDayoff2        // Days Off
      [53] => GrievDayoff3        // Days Off
      [54] => Phone1              // Phone number
      [55] => Phone2              // DONT'T IMPORT
      [56] => GreivEmplyStat      // Work Status
      [57] => GreivShift          // Shift
      [58] => Check3
      [59] => Check4
      [60] => Check5
      [61] => YN1
      [62] => Grievant            // Grievant name
      [63] => GCustId             // TIMSS ID
      [64] => Disp_Pro            // DON'T IMPORT
      [65] => Hard_copy_Lost      // DON'T IMPORT
      [66] => Purge               // DON'T IMPORT
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

    /*
    grievance_uhmigrate_log('DEBUG', $row, 'DispDte = ' . $row['DispDte']);
    grievance_uhmigrate_log('DEBUG', $row, 'DspAction = ' . $row['DspAction']);
    grievance_uhmigrate_log('DEBUG', $row, 'DispDte = ' . $row['DispDte']);
    */

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

    // Some shops are mapped to different shops.
    $shop_map['LVH00146'] = 'LVH00009';
    $shop_map['LVH00308'] = 'LVH00001';
    $shop_map['LVH00317'] = 'LVH00007';
    $shop_map['LVH00322'] = 'LVH00001';
    $shop_map['LVH00351'] = 'LVH00001';
    $shop_map['LVH00355'] = 'LVH00001';
    $shop_map['LVH00563'] = 'LVH00568';
    $shop_map['LVH00612'] = 'LVH00549';
    $shop_map['LVH00772'] = 'LVH00575';
    $shop_map['LVH00773'] = 'LVH00549';
    $shop_map['LVH00783'] = 'LVH00591';
    $shop_map['LVH00784'] = 'LVH00784';
    $shop_map['LVH00788'] = 'LVH00550';
    $shop_map['LVH00792'] = 'LVH00549';
    $shop_map['LVH00816'] = 'LVH00816';
    $shop_map['LVH87002'] = 'LVH00508';

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

    // Skip any with an ID that begins with 9
    if (preg_match('/^9/', $grievance_id)) {
      // grievance_uhmigrate_log('SKIP', $row, "Skipping row the grievance ID begins with a 9.");
      continue;
    }

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
    if ($row['DispDte'] && strtotime($intake_date) > 0 && strtotime($intake_date) < strtotime('2014-1-1')) {
      // grievance_uhmigrate_log('SKIP', $row, "Skipping row because the grievance is closed and the intake date $intake_date is prior to 2014-1-1");
      continue;
    }

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

    // Date of Hire
    $doh = NULL;
    if ($row['GrievDOH'] && (strtotime($row['GrievDOH']) > 1)) {
      $doh = date('Y-m-d 00:00:00', strtotime($row['GrievDOH']));
      $grievance_node->field_grievance_hire_date['und'][0] = array(
        'value' => format_date(strtotime($doh), 'custom', 'Y-m-d H:i:s', 'UTC'),
        'timezone' => 'UTC',  
        'timezone_db' => 'UTC',
      );
    }

    // Days Off
    if ($row['GrievDayoff1']) {
      $map = array(
        'Mon' => 'Monday',
        'Tue' => 'Tuesday',
        'Wed' => 'Wednesday',
        'Thu' => 'Thursday',
        'Fri' => 'Friday',
        'Sat' => 'Saturday',
        'Sun' => 'Sunday',
      );
      $grievance_node->field_grievance_daysoff['und'] = array();
      for ($i=1; $i<=3; ++$i) {
        if ($row['GrievDayoff' . $i]) {
          $d = $map[$row['GrievDayoff' . $i]];
          if ($d) {
            $grievance_node->field_grievance_daysoff['und'][] = array('value' => $d);
          }
        }
      }
    }

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

    // Shift
    if ($row['GreivShift']) {
      $map = array(
        'Extra' => 'Extra',
        'Swing' => 'Swing',
        'Day' => 'Day',
        'Grave' => 'Graveyard',
        'Relief' => 'Relief',
        'On-Call' => 'On Call',
      );
      $shift_name = $map[$row['GreivShift']];
      if ($shift_name) {
        $shift_terms = taxonomy_get_term_by_name($shift_name, 'grievance_shift');
        if (!$work_status_terms) {
          grievance_uhmigrate_log('WARNING - SHIFT NOT FOUND', $row, "Shift $shift_name not found.");
        } else {
          $tmp = array_keys($shift_terms);
          $shift_tid = $tmp[0];
          if ($shift_tid) {
            $grievance_node->field_grievance_shift['und'][0]['tid'] = $shift_tid;
          }
        }
      }
    }

    // Violation
    $clause_nids = grievance_uhmigrate_find_violation_nid($row, $shop_node);
    if ($clause_nids) {
      $delta = 0;
      foreach ($clause_nids as $clause_nid) {
        $grievance_node->field_grievance_clauseref['und'][$delta]['target_id'] = $clause_nid;
        ++$delta;
      }
    }

    // Shop steward
    $grievance_node->field_grievance_supervisor_name['und'][0]['value'] = $row['GrievSupervisor'];

    // Save the grievance node
    $nid = $grievance_node->nid;
    if (!$nid) { $nid = "[new]"; }
    $grievance_node->grievance_no_recurse = TRUE;
    grievance_uhmigrate_node_save($grievance_node);
    grievance_uhmigrate_log('SUCCESS', $row, "External ID $external_id, Grievance ID $grievance_id, Node ID $nid, intake $intake_date");

    // Create Settlements
    grievance_uhmigrate_create_settlements($row, $grievance_node);

    // Create the log
    $final_status_tid = grievance_uhmigrate_create_log($row, $grievance_node);
    if ($final_status_tid) {
      $grievance_node->field_grievance_status['und'][0]['tid'] = $final_status_tid;
      grievance_uhmigrate_node_save($grievance_node);
    }

    /*
    if ($row['GrievDayoff1']) {
      print_r($row);
      exit();
    }
    */
  }

  grievance_uhmigrate_log_summary();
}

function grievance_uhmigrate_create_log($row, $grievance_node) {
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


  // Disposition date
  $disp_date = NULL;
  if ($row['DispDte'] && (strtotime($row['DispDte']) > 1)) {
    $disp_date = date('Y-m-d 00:00:00', strtotime($row['DispDte']));
  }

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
  10:  First Step Letter Date = Step 1 Letter Processed
  9:    First Step Date = Step 1 Held (or can we just use “Step 1 Resolved” and “Step 1 Unresolved”?)
  1:    BOA Letter Date = BOA Letter Processed
  2:    BOA Date = BOA Held
  13:  ADR Sub Date = ADR Referred
  14:  ADR Date = ADR Held
  8:    Exp. Sub Arb Date = Expedited Arbitration Referred
  7:    Expedited Arb Date = Expedited Arbitration Held
  5:    FMCS Sub Date = FMCS Referred
  12:  FMCS Date = FMCS Held
  6:    Mediation Sub Date = Mediation Referred
  15:  Mediation Date = Mediation Held
  3:    Reg Sub Arb Date = Formal Arbitration Referred
  4:    Regular Arb Date = Formal Arbitration Held
  */

  $mapping = array(
    'Date10' => 'Step 1 Letter Processed',
    'Date9' => 'Step 1 Held',
    'Date1' => 'BOA Letter Processed',
    'Date2' => 'BOA Held',
    'Date13' => 'ADR Referred',
    'Date14' => 'ADR Held',
    'Date8' => 'Expedited Arbitration Referred',
    'Date7' => 'Expedited Arbitration Held',
    'Date5' => 'FMCS Referred',
    'Date12' => 'FMCS Held',
    'Date6' => 'Mediation Referred',
    'Date15' => 'Mediation Held',
    'Date3' => 'Formal Arbitration Referred',
    'Date4' => 'Formal Arbitration Held',
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

  // Close the grievance
  if ($row['DispDte'] && 
      (strtotime($row['DispDte']) > 1)) {

    $final_status_name = $status_lookup[$final_status_tid];
    $status_name = NULL;
    $status_tid = NULL;

    if ($final_status_name == 'BOA Letter Processed' && $row['DspAction'] == 'NFA')     { $status_name = 'BOA NFA'; } 
    else if ($final_status_name == 'Step 1 Held' && $row['DspAction'] == 'NFA')         { $status_name = 'Step 1 Resolved'; } 
    else if ($final_status_name == 'ADR Held' && $row['DspAction'] == 'LOST') { $status_name = 'ADR Lost'; }
    else if ($final_status_name == 'ADR Held' && $row['DspAction'] == 'NFA') { $status_name = 'ADR NFA'; }
    else if ($final_status_name == 'ADR Held' && $row['DspAction'] == 'RSLVD') { $status_name = 'ADR Resolved'; }
    else if ($final_status_name == 'ADR Held' && $row['DspAction'] == 'WON') { $status_name = 'ADR Won'; }
    else if ($final_status_name == 'ADR Referred' && $row['DspAction'] == 'LOST') { $status_name = 'ADR Lost'; }
    else if ($final_status_name == 'ADR Referred' && $row['DspAction'] == 'NFA') { $status_name = 'ADR NFA'; }
    else if ($final_status_name == 'ADR Referred' && $row['DspAction'] == 'RSLVD') { $status_name = 'ADR Resolved'; }
    else if ($final_status_name == 'ADR Referred' && $row['DspAction'] == 'WON') { $status_name = 'ADR Won'; }
    else if ($final_status_name == 'Expedited Arbitration Referred' && $row['DspAction'] == 'RSLVD') { $status_name = 'Expedited Arbitration Resolved'; }
    else if ($final_status_name == 'FMCS Held' && $row['DspAction'] == 'NFA') { $status_name = 'FMCS NFA'; }
    else if ($final_status_name == 'FMCS Held' && $row['DspAction'] == 'RSLVD') { $status_name = 'FMCS Resolved'; }
    else if ($final_status_name == 'FMCS Referred' && $row['DspAction'] == 'NFA') { $status_name = 'FMCS NFA'; }
    else if ($final_status_name == 'FMCS Referred' && $row['DspAction'] == 'RSLVD') { $status_name = 'FMCS Resolved'; }
    else if ($final_status_name == 'Formal Arbitration Held' && $row['DspAction'] == 'LOST') { $status_name = 'Formal Arbitration Lost'; }
    else if ($final_status_name == 'Formal Arbitration Held' && $row['DspAction'] == 'NFA') { $status_name = 'Formal Arbitration NFA'; }
    else if ($final_status_name == 'Formal Arbitration Held' && $row['DspAction'] == 'RSLVD') { $status_name = 'Formal Arbitration Resolved'; }
    else if ($final_status_name == 'Formal Arbitration Held' && $row['DspAction'] == 'WON') { $status_name = 'Formal Arbitration Won'; }
    else if ($final_status_name == 'Formal Arbitration Referred' && $row['DspAction'] == 'LOST') { $status_name = 'Formal Arbitration Lost'; }
    else if ($final_status_name == 'Formal Arbitration Referred' && $row['DspAction'] == 'NFA') { $status_name = 'Formal Arbitration NFA'; }
    else if ($final_status_name == 'Formal Arbitration Referred' && $row['DspAction'] == 'RSLVD') { $status_name = 'Formal Arbitration Resolved'; }
    else if ($final_status_name == 'Formal Arbitration Referred' && $row['DspAction'] == 'WON') { $status_name = 'Formal Arbitration Won'; }
    else if ($final_status_name == 'Mediation Held' && $row['DspAction'] == 'RSLVD') { $status_name = 'Mediation Resolved'; }
    else if ($final_status_name == 'Mediation Referred' && $row['DspAction'] == 'NFA') { $status_name = 'Mediation NFA'; }
    else if ($final_status_name == 'Mediation Referred' && $row['DspAction'] == 'RSLVD') { $status_name = 'Mediation Resolved'; }
    else if ($final_status_name == 'Step 1 Held' && $row['DspAction'] == 'RSLVD') { $status_name = 'Step 1 Resolved'; }
    else if ($final_status_name == 'Step 1 Letter Processed' && $row['DspAction'] == 'NFA') { $status_name = 'Step 1 NFA'; }
    else if ($final_status_name == 'Step 1 Letter Processed' && $row['DspAction'] == 'RSLVD') { $status_name = 'Step 1 Resolved'; }
    else if ($final_status_name == 'BOA Held' && $row['DspAction'] == 'NFA') { $status_name = 'BOA NFA'; }
    else if ($final_status_name == 'BOA Held' && $row['DspAction'] == 'RSLVD') { $status_name = 'BOA Resolved'; }
    else if ($final_status_name == 'BOA Held' && $row['DspAction'] == 'LOST') { $status_name = 'BOA NFA'; }
    else if ($final_status_name == 'BOA Letter Processed' && $row['DspAction'] == 'LOST') { $status_name = 'Formal Arbitration Lost'; }
    else if ($final_status_name == 'BOA Letter Processed') { $status_name = 'BOA Resolved'; }
    else if ($final_status_name == 'BOA Held' && $row['DspAction'] == 'WON') { $status_name = 'ADR Resolved'; }
    else if ($final_status_name == 'Mediation Held' && $row['DspAction'] == 'LOST') { $status_name = 'Mediation NFA'; }
    else if ($row['DspAction'] == 'NFA') { $status_name = 'Pre-Processed NFA'; }
    else if ($row['DspAction'] == 'RSLVD') { $status_name = 'Pre-Processed Resolved'; }

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
      $row['DispDte'],
      $status_tid
    );

    $final_ts = strtotime($row['DispDte']);
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

function grievance_uhmigrate_create_log_entry($row, $grievance_node, $log_type, $msg, $log_date, $status_tid = NULL) {
  $terms = taxonomy_get_term_by_name($log_type, 'grievance_log_types');
  if (!$terms) {
    grievance_uhmigrate_log('ERROR', $row, "Unable to load log type $log_type");
    return NULL;
  }

  $tmp = array_keys($terms);
  $log_type_tid = $tmp[0];

  global $user;
  $node = new stdClass();
  $node->type = "grievance_log";
  node_object_prepare($node); // Sets some defaults. Invokes hook_prepare() and hook_node_prepare().
  $node->language = LANGUAGE_NONE; // Or e.g. 'en' if locale is enabled
  $node->uid = $user->uid; 
  $node->status = 1;
  $node->promote = 0;
  $node->comment = 0;
  $node->field_grievance['und'][0]['target_id'] = $grievance_node->nid;
  $node->field_grievance_log_type['und'][0]['tid'] = $log_type_tid;
  $node->body['und'][0]['value'] = $msg;
  if ($log_date) {
    $node->field_grievance_status_date['und'][0] = array(
      'value' => format_date(strtotime($log_date), 'custom', 'Y-m-d H:i:s', 'UTC'),
      'timezone' => 'UTC',  
      'timezone_db' => 'UTC',
    );
  }
  if ($status_tid) {
    $node->field_grievance_status['und'][0]['tid'] = $status_tid;
  }
  grievance_uhmigrate_node_save($node);
}

function grievance_uhmigrate_create_settlements($row, $grievance_node) {
  if ($row['Amount']) {
    global $user;
    $node = new stdClass();
    $node->type = "grievance_settlement";
    node_object_prepare($node); // Sets some defaults. Invokes hook_prepare() and hook_node_prepare().
    $node->language = LANGUAGE_NONE; // Or e.g. 'en' if locale is enabled
    $node->uid = $user->uid; 
    $node->status = 1;
    $node->promote = 0;
    $node->comment = 0;
    $node->field_grievance['und'][0]['target_id'] = $grievance_node->nid;
    $node->field_grievance_amt['und'][0]['value'] = $row['Amount'];
    grievance_uhmigrate_node_save($node);
  }
}

function grievance_uhmigrate_find_violation_nid($row, $shop_node) {
  $contract_template_nid = $shop_node->field_grievance_contract_tplt['und'][0]['target_id'];
  if (!$contract_template_nid) {
    grievance_uhmigrate_log('WARNING - VIOLATION NOT FOUND', $row, "The employer does not have a contract template configured.");
    return NULL;
  }

  $clause = trim($row['Violation']);
  if (!$clause) {
    grievance_uhmigrate_log('WARNING - VIOLATION NOT FOUND', $row, "There was no 'Violation' entered for this grievance.");
    return NULL;
  
}
  $clauses = array();

  global $clause_map;
  if (!$clause_map) {
    $clause_map = grievance_uhmigrate_l226_clausmap();
  }
  $other_key = $shop_node->title . '|' . $clause;
  $other_key = preg_replace('/[^a-zA-Z0-9_\.\ \(\)\-]/', '', $other_key);
  $other_key = preg_replace('/[ ]/', '', $other_key);

  if ($clause_map[$other_key]) {
    $clauses = $clause_map[$other_key];
  } else {
    $clauses = array($clause);
  }

  $nids = array();
  foreach ($clauses as $clause) {
    if (!$clause) { continue; }

    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', 'node');
    $query->entityCondition('bundle', 'grievance_contract_clause');
    $query->fieldCondition('field_grievance_contract_tplt', 'target_id', $contract_template_nid);
    $query->fieldCondition('field_grievance_label', 'value', $clause);
    $query->propertyOrderBy('nid');
    $stmt = $query->execute();
    if (!$stmt['node']) {
      grievance_uhmigrate_log('WARNING - VIOLATION NOT FOUND', $row, "Violation [$clause] was not found in the contract template ($contract_template_nid).");
      // print_r($row);
      // print "$other_key\n\n";

      global $violations_not_found;
      $key = $shop_node->title . "\t" . $clause;
      if (!$violations_not_found[$key]) { $violations_not_found[$key] = $row['Row']; }
      // print "Logging as $key\n";
    } else {
      $tmp = array_keys($stmt['node']);
      $nids[] = $tmp[0];
    }
  }

  return $nids;
}
