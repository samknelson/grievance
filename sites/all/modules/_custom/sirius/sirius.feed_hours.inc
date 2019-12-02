<?php

function sirius_feed_hours_row_verify($feed_node, $info, $data, &$row, $precompute) {
  $pk = $data['pk'];
  if (!$pk) { $pk = 'id'; }

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
		$row[$key] = trim($value);
	}

  $row['ssn'] = sirius_ssn_format($row['ssn']);
  if ($pk == 'id') {
   if (!preg_match('/^\d+$/', $row['id'])) { return array('success' => FALSE, 'msg' => 'Invalid Member ID'); }
  } else if ($pk == 'id3') {
   if (!preg_match('/^\d+$/', $row['id3'])) { return array('success' => FALSE, 'msg' => 'Invalid ID3'); }
  } else if ($pk == 'ssn') {
    if (!sirius_ssn_validate($row['ssn'])) { return array('success' => FALSE, 'msg' => 'Invalid SSN'); }
  } else if ($pk) {
    return array('success' => FALSE, 'msg' => "Don\'t know how to look up the worker; unknown pk $pk.");
  } else {
    return array('success' => FALSE, 'msg' => "Don\'t know how to look up the worker; no pk has been set for this upload.");
  }

  if (!$row['employer']) {
    return array('success' => FALSE, 'msg' => "No employer specified.");
  }

  if (!$row['department']) {
    return array('success' => FALSE, 'msg' => "No department specified.");
  }

  if (!$row['hour_type']) {
    return array('success' => FALSE, 'msg' => "No hour type specified.");
  }

  $date = $row['date'];
  if (preg_match('/^\d+\/\d+$/', $date)) {
    list ($month, $year) = explode('/', $row['date']);
    $day = 1;
    $ts = strtotime("$year-$month-1 00:00:00");
  } else {
    // @todo: This code is untested.
    $ts = strtotime($date);
    list ($year, $month, $day) = explode('-', gmdate('Y-m-d', $ts));
  }
  $row['year'] = $year;
  $row['month'] = $month;
  $row['day'] = $day;

  // Worker
  if ($precompute) {
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
    $nid = $hr['a'];
    $other_nid = $hr['b'];
    if (!$nid) { return array('success' => FALSE, 'msg' => "Worker not found with this ID ($table, $column)."); }
    if ($nid != $other_nid) { return array('success' => FALSE, 'msg' => "Multiple workers found with this ID ($table, $column)."); }
    $row['worker_nid'] = $nid;

    $sql = "select max(nid) as a, min(nid) as b from node where type = 'grievance_shop' and title = :title";
    $stmt = db_query($sql, array(':title' => $row['employer']));
    $hr = $stmt->fetchAssoc();
    $nid = $hr['a'];
    $other_nid = $hr['b'];
    if (!$nid) { return array('success' => FALSE, 'msg' => "Employer not found."); }
    if ($nid != $other_nid) { return array('success' => FALSE, 'msg' => "Multiple employers found."); }
    $row['employer_nid'] = $nid;

    $sql = "select max(tid) as a, min(tid) as b from taxonomy_term_data "; 
    $sql .= "join taxonomy_vocabulary on taxonomy_term_data.vid = taxonomy_vocabulary.vid and taxonomy_vocabulary.machine_name = 'grievance_department' ";
    $sql .= "left join field_data_field_sirius_id on field_data_field_sirius_id.entity_type = 'taxonomy_term' and field_data_field_sirius_id.entity_id = tid ";
    $sql .= "where taxonomy_term_data.name = :title ";
    $sql .= "or field_sirius_id_value = :title ";
    $stmt = db_query($sql, array(':title' => $row['department']));
    $hr = $stmt->fetchAssoc();
    $tid = $hr['a'];
    $other_tid = $hr['b'];
    if (!$tid) { return array('success' => FALSE, 'msg' => "Department not found."); }
    if ($tid != $other_tid) { return array('success' => FALSE, 'msg' => "Multiple departments found."); }
    $row['department_tid'] = $tid;

    $sql = "select max(tid) as a, min(tid) as b from taxonomy_term_data "; 
    $sql .= "join taxonomy_vocabulary on taxonomy_term_data.vid = taxonomy_vocabulary.vid and taxonomy_vocabulary.machine_name = 'sirius_hour_type' ";
    $sql .= "left join field_data_field_sirius_id on field_data_field_sirius_id.entity_type = 'taxonomy_term' and field_data_field_sirius_id.entity_id = tid ";
    $sql .= "where taxonomy_term_data.name = :title ";
    $sql .= "or field_sirius_id_value = :title ";
    $stmt = db_query($sql, array(':title' => $row['hour_type']));
    $hr = $stmt->fetchAssoc();
    $tid = $hr['a'];
    $other_tid = $hr['b'];
    if (!$tid) { return array('success' => FALSE, 'msg' => "Hour Type not found."); }
    if ($tid != $other_tid) { return array('success' => FALSE, 'msg' => "Multiple hour types found."); }
    $row['hour_type_tid'] = $tid;
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
  $day = $row['day'];
  $hour_type_tid = $row['hour_type_tid'];

  $start_ts = strtotime("$year-$month-$day 00:00:00");
  $end_ts = $start_ts + ($hours*60*60);

  $comment = '';
  for ($i=1; $i<=3; ++$i) {
    if (!$row["notes$i"]) { continue; }
    if ($comment) { $comment .= " / "; }
    $comment .= $row["$notes$i"];
  }

  $payperiod_node = sirius_hour_payperiod_load($worker_nid, $employer_nid, $year, $month, $day, 'Yes');
  $json = json_decode($payperiod_node->field_sirius_json['und'][0]['value'], TRUE);

  // Sort through the entries in this payperiod, removing any that match our incoming department, type, and start ts. 
  // (This allows us to run the feed multiple times without creating multiple entries.)
  $entries = $json['entries'];
  $new_entries = array();
  foreach ($entries as $entry) {
    if ($entry['department_tid'] != $department_tid || 
      $entry['hour_type_tid'] != $hour_type_tid ||
      $entry['start_ts'] != $start_ts) {
      $new_entries[] = $entry;
    }
  }

  // Add our new legitimate entry
  $new_entries[] = array(
    'hour_type_tid' => $hour_type_tid,
    'start_ts' => $start_ts,
    'end_ts' => $end_ts,
    'department_tid' => $department_tid,
    'qty' => $hours,
    'comment' => $comment,
  );
  $json['entries'] = $new_entries;

  sirius_hour_payperiod_compute($json);
  $payperiod_node->field_sirius_json['und'][0]['value'] = json_encode($json);
  node_save($payperiod_node);

  return array('success' => TRUE, 'msg' => 'Updated an existing worker.');
}
