<?php

function sirius_edls_assignment_fetch_worker($date, $employer_nid, $worker_id) {
  $sql = "select * from sirius_edls_assignments ";
  $sql .= "where assignment_date = :date and worker_id = :worker_id and employer_nid = :employer_nid";
  $stmt = sirius_sql_query($sql, array(':date' => $date, ':worker_id' => $worker_id, ':employer_nid' => $employer_nid));
  return $stmt->fetchAssoc();
}

function sirius_edls_assignment_fetch_crew($date, $sheet_nid, $crew_uuid) {
  $sql = "select * from sirius_edls_assignments ";
  $sql .= "where assignment_date = :date and sheet_nid = :sheet_nid and crew_uuid = :crew_uuid ";
  $stmt = sirius_sql_query($sql, array(':date' => $date, ':sheet_nid' => $sheet_nid, ':crew_uuid' => $crew_uuid));

  while ($hr = $stmt->fetchAssoc()) {
    $crew[$hr['assignment_position']] = $hr;
  }

  return $crew;
}

function sirius_edls_assignment_fetch_sheet($date, $sheet_nid) {
  $sql = "select * from sirius_edls_assignments ";
  $sql .= "where assignment_date = :date and sheet_nid = :sheet_nid ";
  $stmt = sirius_sql_query($sql, array(':date' => $date, ':sheet_nid' => $sheet_nid));

  $assignments = array();
  while ($hr = $stmt->fetchAssoc()) {
    $assignments[$hr['crew_uuid']][$hr['assignment_position']] = $hr;
  }

  return $assignments;
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

  $crew = sirius_edls_crew($sheet_node, $crew_uuid);
  if (!$crew) { return array('success' => FALSE, 'msg' => 'Crew not found.'); }

  $assignments = sirius_edls_assignment_fetch_crew($date, $sheet_nid, $crew_uuid);

  $position = -1;
  for ($i=0; $i<$crew['count']; ++$i) {
    if (!$assignments[$i]) {
      $position = $i;
      break;
    }
  }

  if ($position < 0) {
    return array('success' => FALSE, 'msg' => 'The crew is full.');
  }

  $worker = sirius_edls_worker_fetch($employer_nid, $worker_id);
  if (!$worker) { return array('success' => FALSE, 'msg' => "Worker ID $worker_id not found."); }

  $sql_args = array();
  $sql_args[':date'] = $date;
  $sql_args[':worker_id'] = $worker_id;
  $sql_args[':sheet_nid'] = $sheet_nid;
  $sql_args[':crew_uuid'] = $crew_uuid;
  $sql_args[':assignment_extra'] = json_encode($extra);
  $sql_args[':assignment_position'] = $position;
  $sql_args[':worker_name'] = $worker['worker_name'];
  $sql_args[':employer_nid'] = $employer_nid;
  $sql = "insert into sirius_edls_assignments (assignment_date, worker_id, worker_name, employer_nid, sheet_nid, crew_uuid, assignment_position, assignment_extra) ";
  $sql .= "values (:date, :worker_id, :worker_name, :employer_nid, :sheet_nid, :crew_uuid, :assignment_position, :assignment_extra)";
  $stmt = db_query($sql, $sql_args);

  return array('success' => TRUE, 'position' => $position);
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

  return array('success' => TRUE, 'extra' => $extra);
}

function sirius_edls_unassign($date, $worker_id, $sheet_nid) {
  $sql = "delete from sirius_edls_assignments where assignment_date = :date and worker_id = :worker_id and sheet_nid = :sheet_nid";
  $stmt = db_query($sql, array(':date' => $date, ':worker_id' => $worker_id, ':sheet_nid' => $sheet_nid));
  return array('success' => TRUE);
}
