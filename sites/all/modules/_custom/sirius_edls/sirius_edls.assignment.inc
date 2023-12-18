<?php

function sirius_edls_assignment_fetch_worker($date, $employer_nid, $worker_id) {
  $sql = "select * from sirius_edls_assignments ";
  $sql .= "where assignment_date = :date and worker_id = :worker_id and employer_nid = :employer_nid";
  $stmt = sirius_sql_query($sql, array(':date' => $date, ':worker_id' => $worker_id, ':employer_nid' => $employer_nid));
  return $stmt->fetchAssoc();
}

function sirius_edls_assignment_fetch_crew($sheet_node, $crew_uuid) {
  $assignments = sirius_edls_assignment_fetch_sheet($sheet_node);
  return $assignments[$crew_uuid];
}

function sirius_edls_assignment_fetch_sheet($sheet_node) {
  $date = $sheet_node->field_sirius_date_start['und'][0]['value'];
  $employer_node = node_load($sheet_node->field_grievance_shop['und'][0]['target_id']);
  $sql = "select * from sirius_edls_assignments ";
  $sql .= "where assignment_date = :date and sheet_nid = :sheet_nid ";
  $stmt = sirius_sql_query($sql, array(':date' => $date, ':sheet_nid' => $sheet_node->nid));

  $assignments = array();
  while ($hr = $stmt->fetchAssoc()) {
    $crew_uuid = $hr['crew_uuid'];
    $assignments[$crew_uuid][] = $hr;
  }

  $ms_list = sirius_edls_ms_list($employer_node, $type = 'name');
  $ms_weights = array();
  $weight = 1;
  foreach ($ms_list as $ms_key => $ignore) {
    $ms_weights[$ms_key] = $weight;
    ++$weight;
  }

  $classification_options = sirius_edls_worker_classification_options();
  $classification_weights = array();
  $weight = 1;
  foreach ($classification_options as $classification_key => $ignore) {
    $classification_weights[$classification_key] = $weight;
    ++$weight;
  }

  foreach ($assignments as $crew_uuid => $crew) {
    foreach ($crew as $delta => $assignment) {
      $assignments[$crew_uuid][$delta]['assignment_extra'] = json_decode($assignments[$crew_uuid][$delta]['assignment_extra'], TRUE);
      $assignments[$crew_uuid][$delta]['worker_name'] =  ucwords(strtolower($assignments[$crew_uuid][$delta]['worker_name']));
    }

    usort($assignments[$crew_uuid], function($a, $b) use ($ms_weights, $classification_weights) {
      // First sort by classification
      $a_weight = $classification_weights[$a['assignment_extra']['classification']];
      $b_weight = $classification_weights[$b['assignment_extra']['classification']];
      if (!$a_weight) { $a_weight = 1000; }
      if (!$b_weight) { $b_weight = 1000; }
      if ($a_weight < $b_weight) { return -1; }
      if ($a_weight > $b_weight) { return 1; }

      // Next sort by member status
      $a_weight = $ms_weights[$a['worker_ms']];
      $b_weight = $ms_weights[$b['worker_ms']];
      if (!$a_weight) { $a_weight = 1000; }
      if (!$b_weight) { $b_weight = 1000; }
      if ($a_weight < $b_weight) { return -1; }
      if ($a_weight > $b_weight) { return 1; }

      // Finally sort by worker name
      return strcasecmp($a['worker_name'], $b['worker_name']);
    });
  }

  return $assignments;
}

function sirius_edls_assignment_fetch_sheet_worker($date, $sheet_nid, $worker_id) {
  $sql = "select * from sirius_edls_assignments ";
  $sql .= "where assignment_date = :date and sheet_nid = :sheet_nid and worker_id = :worker_id ";
  $stmt = sirius_sql_query($sql, array(':date' => $date, ':sheet_nid' => $sheet_nid, ':worker_id' => $worker_id));
  $result = $stmt->fetchAssoc();
  if (!$result) { $result = array(); }
  return $result;
}

function sirius_edls_assignment_count_sheet($date, $sheet_nid) {
  $sql = "select count(*) as c from sirius_edls_assignments ";
  $sql .= "where assignment_date = :date and sheet_nid = :sheet_nid ";
  $stmt = sirius_sql_query($sql, array(':date' => $date, ':sheet_nid' => $sheet_nid));

  $hr = $stmt->fetchAssoc();
  return $hr['c'];
}

function sirius_edls_assign($date, $worker_id, $sheet_nid, $crew_uuid, $extra = array()) {
  $sheet_node = node_load($sheet_nid);

  $employer_nid = $sheet_node->field_grievance_shop['und'][0]['target_id'];
  if (!$employer_nid) { return array('success' => FALSE, 'msg' => 'There is no employer associated with this sheet.'); }

  $assignment = sirius_edls_assignment_fetch_worker($date, $employer_nid, $worker_id);
  if ($assignment) {
    if (($assignment['sheet_nid'] != $sheet_nid)) {
      return array('success' => FALSE, 'msg' => 'The worker is already assigned to a different sheet.', 'assignment' => $assignment);
    }
    return array('success' => FALSE, 'msg' => 'The worker is already assigned to this sheet.', 'assignment' => $assignment);
  }

  $worker = sirius_edls_worker_fetch($employer_nid, $worker_id);
  if (!$worker) { return array('success' => FALSE, 'msg' => "Worker ID $worker_id not found."); }

  $crew = sirius_edls_crew($sheet_node, $crew_uuid);
  if (!$crew) { return array('success' => FALSE, 'msg' => 'Crew not found.'); }

  $assignments = sirius_edls_assignment_fetch_crew($sheet_node, $crew_uuid);
  if (!$assignments) { $assignments = array(); }
  if (count($assignments) >= $crew['count']) { return array('success' => FALSE, 'msg' => 'The crew is full.'); }

  $sql_args = array();
  $sql_args[':date'] = $date;
  $sql_args[':worker_id'] = $worker_id;
  $sql_args[':sheet_nid'] = $sheet_nid;
  $sql_args[':crew_uuid'] = $crew_uuid;
  $sql_args[':assignment_extra'] = json_encode($extra);
  $sql_args[':worker_name'] = $worker['worker_name'];
  $sql_args[':employer_nid'] = $employer_nid;
  $sql_args[':worker_ms'] = $worker['worker_ms'];
  $sql = "insert into sirius_edls_assignments (assignment_date, worker_id, worker_name, worker_ms, employer_nid, sheet_nid, crew_uuid, assignment_extra) ";
  $sql .= "values (:date, :worker_id, :worker_name, :worker_ms, :employer_nid, :sheet_nid, :crew_uuid, :assignment_extra)";
  $stmt = db_query($sql, $sql_args);

  $assignment = sirius_edls_assignment_fetch_sheet_worker($date, $sheet_nid, $worker_id);
  sirius_edls_log_instance()->assignment_log('assign', $assignment);

  return array(
    'success' => TRUE, 
    'values' => $sql_args, 
    'msg' => "Assigned $worker[worker_name] ($worker_id)",
  );
}

function sirius_edls_assignment_set_extra($date, $worker_id, $sheet_nid, $extra = array()) {
  $sheet_node = node_load($sheet_nid);

  $employer_nid = $sheet_node->field_grievance_shop['und'][0]['target_id'];
  if (!$employer_nid) { return array('success' => FALSE, 'msg' => 'There is no employer associated with this sheet.'); }

  $assignment = sirius_edls_assignment_fetch_worker($date, $employer_nid, $worker_id);
  if (!$assignment || ($assignment['sheet_nid'] != $sheet_nid)) {
    return array('success' => FALSE, 'msg' => 'The worker is not assigned to this sheet.');
  }

  if ($extra['time']) {
    $ts = strtotime($extra['time']);
    if ($ts === false) {
      unset($extra['time']);
    } else {
      $extra['time'] = date('g:i a', $ts);
    }
  }

  $sql = "update sirius_edls_assignments set assignment_extra = :extra where assignment_id = :assignment_id";
  $stmt = db_query($sql, array(':assignment_id' => $assignment['assignment_id'], ':extra' => json_encode($extra)));

  $assignment = sirius_edls_assignment_fetch_sheet_worker($date, $sheet_nid, $worker_id);
  sirius_edls_log_instance()->assignment_log('update', $assignment);

  return array(
    'success' => TRUE, 
    'extra' => $extra,
    'msg' => "Updated $assignment[worker_name] ($worker_id)",
  );
}

function sirius_edls_unassign($date, $worker_id, $sheet_nid) {
  $assignment = sirius_edls_assignment_fetch_sheet_worker($date, $sheet_nid, $worker_id);
  sirius_edls_log_instance()->assignment_log('unassign', $assignment);

  $sql = "delete from sirius_edls_assignments where assignment_date = :date and worker_id = :worker_id and sheet_nid = :sheet_nid";
  $stmt = db_query($sql, array(':date' => $date, ':worker_id' => $worker_id, ':sheet_nid' => $sheet_nid));

  return array(
    'success' => TRUE,
    'msg' => "Unassigned $assignment[worker_name] ($worker_id)",
  );
}
