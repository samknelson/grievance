<?php

function sirius_feed_hours_info() {
  $items = array();
  $items['feed_hours'] = array(
    'name' => 'feed_hours',
    'title' => 'Hours',
    'description' => 'Upload a new list of hours.',
    'steps' => array(
      'upload'    => array('label' => 'Upload',     'callback' => 'sirius_feed_wizard_upload'),
      'sheet'     => array('label' => 'Worksheet',  'callback' => 'sirius_feed_wizard_sheet'),
      'map'       => array('label' => 'Mapping',    'callback' => 'sirius_feed_wizard_map'),
      'verify'    => array('label' => 'Verify',     'callback' => 'sirius_feed_wizard_verify'),
      'process'   => array('label' => 'Process',    'callback' => 'sirius_feed_wizard_process'),
      'review'    => array('label' => 'Review',     'callback' => 'sirius_feed_wizard_review'),
    ),
    'feed' => array(
      'pks' => array(
        'nid' => 'Worker NID',
        'ssn' => 'SSN',
        'id' => 'Badge ID',
        'ein' => 'Employee ID',
      ),
      'fields' => array(
        'nid' => array('title' => 'Worker NID'),
        'ssn' => array('title' => 'SSN'),
        'id' => array('title' => 'Badge ID'),
        'ein' => array('title' => 'EIN'),
        'hour_type' => array('title' => 'Hour Type'),
        'employer' => array('title' => 'Employer'),
        'date' => array('title' => 'Date'),
        'hours' => array('title' => 'Hours'),
        'department' => array('title' => 'Department'),
      ),
      'batch_size' => 500,
      'callback_row_verify' => 'sirius_feed_hours_row_verify',
      'callback_row_process' =>'sirius_feed_hours_row_process',
      'employer_access' => TRUE,
    ),
    'callback_launch' => 'sirius_feed_wizard_launch',
    'callback_launch_args' => array('feed_hours'),
  );

  $employer = sirius_employer_get();
  if ($employer) { unset($items['feed_hours']['feed']['fields']['employer']); }
  return $items;
}

function sirius_feed_hours_row_verify($feed_node, $info, $data, &$row, $precompute) {
  $pk = $data['pk'];
  if (!$pk) { $pk = 'id'; }

  static $rowcount;
  $rowcount++;
  // if ($rowcount > 1000) { return array('success' => FALSE, 'msg' => 'Discounting subsequent rows.'); }

	static $employer_map;
	if (!$employer_map) {
		$sql = "select title, nid from node where type = 'grievance_shop'";
		$stmt = db_query($sql, array());
		while ($hr = $stmt->fetchAssoc()) {
			$employer_map[$hr['title']] = $hr['nid'];
		}
	}

	// Trim all
	foreach ($row as $key => $value) {
    if (!is_array($value)) { $row[$key] = trim($value); }
	}

  if ($row['ssn']) { $row['ssn'] = sirius_ssn_format($row['ssn']); }

  if ($pk == 'id') {
   if (!preg_match('/^\d+$/', $row['id'])) { return array('success' => FALSE, 'msg' => 'Invalid Member ID'); }
  } else if ($pk == 'id3') {
   if (!preg_match('/^\d+$/', $row['id3'])) { return array('success' => FALSE, 'msg' => 'Invalid ID3'); }
  } else if ($pk == 'ssn') {
    if (!sirius_ssn_validate($row['ssn'])) { return array('success' => FALSE, 'msg' => 'Invalid SSN'); }
  } else if ($pk == 'ein') {
    if (!$row['ein']) { return array('success' => FALSE, 'msg' => 'No EIN'); }
  } else if ($pk) {
    return array('success' => FALSE, 'msg' => "Don't know how to look up the worker; unknown pk $pk.");
  } else {
    return array('success' => FALSE, 'msg' => "Don't know how to look up the worker; no pk has been set for this upload.");
  }

  if (!$row['employer'] && !$feed_node->field_grievance_shop['und'][0]['target_id']) {
    return array('success' => FALSE, 'msg' => "No employer specified.");
  }
  $employer_nid = $feed_node->field_grievance_shop['und'][0]['target_id'];
  if (!$employer_nid) {
    $employer_nid = $employer_map[$row['employer']];
  }
  if (!$employer_nid) { 
    return array('success' => FALSE, 'msg' => 'Employer not found.'); 
  }

  if (!$row['department']) {
    return array('success' => FALSE, 'msg' => "No department specified.");
  }
  $department_tid = sirius_taxonomy_lookup_by_anything('grievance_department', $row['department'], $employer_nid);
  if (!$department_tid) {
    return array('success' => FALSE, 'msg' => 'Department not found');
  }

  if (!$row['hour_type']) {
    return array('success' => FALSE, 'msg' => "No hour type specified.");
  }
  $hour_type_tid = sirius_taxonomy_lookup_by_anything('sirius_hour_type', $row['hour_type'], $employer_nid);
  if (!$hour_type_tid) {
    return array('success' => FALSE, 'msg' => 'Hour type not found');
  }

  $date = $row['date'];
  if (!$date) {
    return array('success' => FALSE, 'msg' => 'No date.');
  }
  if (preg_match('/^\d+\/\d+$/', $date)) {
    list ($month, $year) = explode('/', $row['date']);
    $day = 1;
    $ts = strtotime("$year-$month-1 00:00:00");
  } else if (preg_match('/^\s*(\d\d\d\d)(\d\d)(\d\d)\s*$/', $date, $matches)) {
    $year = $matches[1];
    $month = $matches[2];
    $day = $matches[3];
  } else {
    // @todo: This code is untested.
    $ts = strtotime($date);
    list ($year, $month, $day) = explode('-', gmdate('Y-m-d', $ts));
  }

  if (!$year || !$month || !$day) {
    return array('success' => FALSE, 'msg' => 'Unable to parse the date');
  }

  // Worker
  if ($precompute) {

    $row['employer_nid'] = $employer_nid;
    $row['department_tid'] = $department_tid;
    $row['hour_type_tid'] = $hour_type_tid;

    $row['year'] = $year;
    $row['month'] = $month;
    $row['day'] = $day;

    if ($pk == 'ein') {
      $worker_nid = sirius_worker_ein_lookup($employer_nid, $row['ein']);
      if (!$worker_nid) { return array('success' => FALSE, 'msg' => 'Worker not found with this EIN.'); }
    } else {
      if ($pk == 'ssn') { 
        $table = 'field_data_field_sirius_id';
        $column = 'field_sirius_id_value';
        $value = $row['id'];
      } else if ($pk == 'id3') { 
        $table = 'field_data_field_sirius_id3';
        $column = 'field_sirius_id3_value';
        $value = $row['id3'];
      } else if ($pk == 'id3') { 
        $table = 'field_data_field_sirius_ssn';
        $column = 'field_sirius_ssn_value';
        $value = $row['ssn'];
      } 

      $sql = "select max(entity_id) as a, min(entity_id) as b from $table ";
      $sql .= "where entity_type = 'node' ";
      $sql .= "and bundle = 'sirius_worker' ";
      $sql .= "and $column = :value ";
      $stmt = db_query($sql, array(':value' => $value));
      $hr = $stmt->fetchAssoc();
      $worker_nid = $hr['a'];
      $other_nid = $hr['b'];
      if (!$worker_nid) { return array('success' => FALSE, 'msg' => "Worker not found with this ID ($table, $column)."); }
      if ($worker_nid != $other_nid) { return array('success' => FALSE, 'msg' => "Multiple workers found with this ID ($table, $column)."); }
    }

    $row['worker_nid'] = $worker_nid;
  }

  return array('success' => TRUE, 'msg' => 'Looks good.');
}

function sirius_feed_hours_row_process($feed_node, $info, $data, $row) {
	$result = sirius_feed_hours_row_verify($feed_node, $info, $data, $row, TRUE);
	if (!$result['success']) { return $result; }
  if (!$row['worker_nid']) { return array('success' => FALSE, 'msg' => 'No worker.'); }
  $worker_nid = $row['worker_nid'];
  $ts = $row['ts'];
  $employer_nid = $row['employer_nid'];
  $department_tid = $row['department_tid'];
  $hours = $row['hours'];
  $year = $row['year'];
  $month = $row['month'];
  $day = (int) $row['day'];
  $hour_type_tid = $row['hour_type_tid'];

  $domain_nid = $feed_node->field_sirius_domain['und'][0]['target_id'];
  $payperiod_node = sirius_hour_payperiod_load($domain_nid, $worker_nid, $employer_nid, $year, $month, $day, 'Yes');
  $json = sirius_jsonfield_util_node_json_get($payperiod_node);

  // Sort through the entries in this payperiod, removing any that match our incoming department, type, and start ts. 
  // (This allows us to run the feed multiple times without creating multiple entries.)
  $source_key = $feed_node->nid;

  $process_ts = $data['process_ts'];
  $process_date = date('Y-m-d H:i:s', $process_ts);
  $source_name = "Employer upload: $process_date";

  if ($source_name != $json['entries'][$source_key]['source_name']) {
    // drupal_set_message("Resetting.");
    unset($json['entries'][$source_key]);
  }

  $json['entries'][$source_key]['source_name'] = $source_name;
  $json['entries'][$source_key]['status'] = 'live';

  // Add our new legitimate entry
  $entry = array();
  $entry['hour_type'] = $hour_type_tid;
  $entry['department'] = $department_tid;
  $entry['day'] = $day;
  $entry['qty'] = $hours;
  $entry['comment'] = print_r($row['xxxrawxxx'], 1);
  $json['entries'][$source_key]['rows'][] = $entry;

  sirius_hour_payperiod_compute($json);
  sirius_jsonfield_util_node_json_set($payperiod_node, $json);
  node_save($payperiod_node);

  return array('success' => TRUE, 'msg' => 'Updated an existing worker.');
}
