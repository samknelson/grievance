<?php

function grievance_uhmigrate_l226_import_grievances() {
  // Source file
  // $source_filename = DRUPAL_ROOT . '/' . drupal_get_path('module', 'grievance_uhmigrate') . "/data/import.csv";
  $source_filename = '/var/www/vhosts/grievances.clevernamehere.com/private/migrate/import.csv';
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
  $rowcount = 0;
  while ($row_raw = fgetcsv($source_handle, 1000, ",")) {
    ++$rowcount;
    if ($rowcount > 1000) { break; }

    // Generate an associative array from the input
    $row = array("Row" => $rowcount);
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

    // Find the employer.  Failure is an exception
    $shop_external_id = $row['GrvLoc'];
    $shop_node = grievance_uhmigrate_find_by_external_id('grievance_shop', $shop_external_id);
    if (!$shop_node->nid) {
      grievance_uhmigrate_log('ERROR', $row, "Unable to find employer with external id: $shop_external_id");
      continue;
    }
    $grievance_node->field_grievance_shop['und'][0]['target_id'] = $shop_node->nid;

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

    // Intake staff
    $filed_by_name = $row['GrievTaker'];
    if ($filed_by_name) {
      $filed_by_user = user_load_by_name($filed_by_name);
      if (!$filed_by_user) {
        grievance_uhmigrate_log('WARNING - USER NOT FOUND', $row, "Grievance Spcialist [$filed_by_name] not found.");
      } else {
        $grievance_node->field_grievance_rep_filed['und'][0]['target_id'] = $filed_by_user->uid;
      }
    }

    // Grievance Specialist
    $organizer_name = $row['GreivAssign'];
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
        grievance_uhmigrate_log('WARNING - DEPARTMENT NOT FOUND', $row, "Department " . $row['Loc_Wrk_Dept'] . " not found.");
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
          $grievance_node->field_grievance_shift['und'][0]['tid'] = $shift_tid;
        }
      }
    }

    // Violation
    $clause_nid = grievance_uhmigrate_find_violation_tid($row, $shop_node);
    if ($clause_nid) {
      $grievance_node->field_grievance_clauseref['und'][0]['target_id'] = $clause_nid;
    }

    // Shop steward
    $grievance_node->field_grievance_supervisor_name['und'][0]['value'] = $row['GrievSupervisor'];

    // Save the grievance node
    $nid = $grievance_node->nid;
    if (!$nid) { $nid = "[new]"; }
    grievance_uhmigrate_node_save($grievance_node);
    grievance_uhmigrate_log('SUCCESS', $row, "External ID $external_id, Grievance ID $grievance_id, Node ID $nid");

    // Create Settlements
    grievance_uhmigrate_create_settlements($row, $grievance_node);

    // Create the log
    grievance_uhmigrate_create_log($row, $grievance_node);

    if ($row['GrievDayoff1']) {
      print_r($row);
      exit();
    }
  }
  fclose($source_handle);

  grievance_uhmigrate_log_summary();
}

function grievance_uhmigrate_create_log($row, $grievance_node) {
  // Disposition date
  $disp_date = NULL;
  if ($row['DispDte'] && (strtotime($row['DispDte']) > 1)) {
    $disp_date = date('Y-m-d 00:00:00', strtotime($row['DispDte']));
  }

  // Date1: BOA Letter Date
  if ($row['Date1'] && (strtotime($row['Date1']) > 1)) {
    $status = 'BOA Held';
    $date = date('Y-m-d 00:00:00', strtotime($row['Date1']));
    $terms = taxonomy_get_term_by_name($status, 'grievance_status');
    if (!$terms) {
      grievance_uhmigrate_log('ERROR - STATUS NOT FOUND', $row, "Status [$status] not found.");
      return NULL;
    }
    $tmp = array_keys($terms);
    $status_tid = $tmp[0];
    grievance_uhmigrate_create_log_entry(
      $row, 
      $grievance_node, 
      'Grievance - Updated', 
      '',
      $date,
      $status_tid
      );
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

function grievance_uhmigrate_find_violation_tid($row, $shop_node) {
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

  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'grievance_contract_clause');
  $query->fieldCondition('field_grievance_contract_tplt', 'target_id', $contract_template_nid);
  $query->fieldCondition('field_grievance_label', 'value', $clause);
  $query->propertyOrderBy('nid');
  $stmt = $query->execute();
  if (!$stmt['node']) {
    grievance_uhmigrate_log('WARNING - VIOLATION NOT FOUND', $row, "Violation [$clause] was not found in the contract template.");
    return NULL;
  }

  $tmp = array_keys($stmt['node']);
  return $tmp[0];
}
