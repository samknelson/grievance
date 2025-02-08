<?php

function sirius_edls_assignment_fetch_worker($date, $employer_nid, $worker_id) {
  $sql = "select * from sirius_edls_assignments ";
  $sql .= "where assignment_date = :date and worker_id = :worker_id and employer_nid = :employer_nid";
  $stmt = sirius_sql_query($sql, array(':date' => $date, ':worker_id' => $worker_id, ':employer_nid' => $employer_nid));
  return $stmt->fetchAssoc();
}

function sirius_edls_assignment_fetch_by_assignment_id($assignment_id) {
  $sql = "select * from sirius_edls_assignments ";
  $sql .= "where assignment_id = :assignment_id ";
  $stmt = sirius_sql_query($sql, array(':assignment_id' => $assignment_id));
  return $stmt->fetchAssoc();
}

function sirius_edls_assignment_fetch_crew($sheet_node, $crew_uuid) {
  $assignments = sirius_edls_assignment_fetch_sheet($sheet_node);
  $result = $assignments[$crew_uuid];
  if (!$result) { $result = array(); }
  return $result;
}

function sirius_edls_assignment_fetch_sheet($sheet_node) {
  $date = $sheet_node->field_sirius_date_start['und'][0]['value'];
  $employer_node = node_load($sheet_node->field_grievance_shop['und'][0]['target_id']);
  $sql = "select * from sirius_edls_assignments ";
  $sql .= "where assignment_date = :date and sirius_edls_assignments.sheet_nid = :sheet_nid ";
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
      if ($assignments[$crew_uuid][$delta]['assignment_extra']['time']) {
        $assignments[$crew_uuid][$delta]['assignment_extra']['time'] = date('H:i', strtotime($assignments[$crew_uuid][$delta]['assignment_extra']['time']));
      }

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


function sirius_edls_assignment_search($params) {
  $domain_nid = sirius_domain_nid();
  $sql = "select * from sirius_edls_assignments ";
  $sql .= "left join node on sheet_nid = node.nid ";
  $sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
  $sql .= "left join field_data_field_sirius_date_start on field_data_field_sirius_date_start.entity_type = 'node' and field_data_field_sirius_date_start.entity_id = node.nid ";
  $sql .= "left join field_data_field_grievance_shop on field_data_field_grievance_shop.entity_type = 'node' and field_data_field_grievance_shop.entity_id = node.nid ";
  $sql .= "left join field_data_field_sirius_job_number on field_data_field_sirius_job_number.entity_type = 'node' and field_data_field_sirius_job_number.entity_id = node.nid ";
  $sql .= "left join field_data_field_grievance_rep_assignee on field_data_field_grievance_rep_assignee.entity_type = 'node' and field_data_field_grievance_rep_assignee.entity_id = node.nid ";
  $sql .= "left join field_data_field_sirius_edls_sheet_status on field_data_field_sirius_edls_sheet_status.entity_type = 'node' and field_data_field_sirius_edls_sheet_status.entity_id = node.nid ";
  $sql .= "left join field_data_field_grievance_department_tid on field_data_field_grievance_department_tid.entity_type = 'node' and field_data_field_grievance_department_tid.entity_id = node.nid ";
  $sql .= "where node.type = 'sirius_edls_sheet' ";

  if ($domain_nid) {
    $sql .= "and field_sirius_domain_target_id = :domain_nid ";
    $sql_args[':domain_nid'] = $domain_nid;
  }

  if ($params['grievance_shop']) {
    $sql .= "and field_grievance_shop_target_id = :grievance_shop ";
    $sql_args[':grievance_shop'] = $params['grievance_shop'];
  }

  if ($params['date']) {
    $sql .= "and field_sirius_date_start_value = :date ";
    $sql_args[':date'] = date('Y-m-d', strtotime($params['date']));
  }

  if ($params['date_min']) {
    $sql .= "and field_sirius_date_start_value >= :date_min ";
    $sql_args[':date_min'] = date('Y-m-d', strtotime($params['date_min']));
  }

  if ($params['date_max']) {
    $sql .= "and field_sirius_date_start_value <= :date_max ";
    $sql_args[':date_max'] = date('Y-m-d', strtotime($params['date_max']));
  }

  if ($params['job_number']) {
    $sql .= "and field_sirius_job_number_value = :job_number ";
    $sql_args[':job_number'] = date('Y-m-d', strtotime($params['job_number']));
  }

  if ($params['facility']) {
    $sql .= "and field_sirius_dispatch_facility_target_id = :facility ";
    $sql_args[':facility'] = $params['facility'];
  }

  if ($params['job_number']) {
    $sql .= "and field_sirius_job_number_value = :job_number ";
    $sql_args[':job_number'] = $params['job_number'];
  }

  if ($params['supervisor']) {
    $sql .= "and field_grievance_rep_assignee_target_id = :supervisor ";
    $sql_args[':supervisor'] = $params['supervisor'];
  }

  if ($params['status']) {
    $sql .= "and field_sirius_edls_sheet_status_value = :status ";
    $sql_args[':status'] = $params['status'];
  }

  if ($params['statuses']) {
    $sql .= "and field_sirius_edls_sheet_status_value in (:statuses) ";
    $sql_args[':statuses'] = $params['statuses'];
  }

  if ($params['department']) {
    $sql .= "and field_grievance_department_tid_tid = :department ";
    $sql_args[':department'] = $params['department'];
  }

  if ($params['worker_id']) {
    $sql .= "and worker_id = :worker_id ";
    $sql_args[':worker_id'] = $params['worker_id'];
  }

  if ($params['mss']) {
    $sql .= "and worker_ms in (:mss) ";
    $sql_args[':mss'] = $params['mss'];
  }

  if (!$params['include_trash']) {
    $sql .= "and field_sirius_edls_sheet_status_value != 'trash' ";
  }

  if ($params['order']) {
    $sql .= 'order by ' . $params['order'] . ' ';
  } else {
    $sql .= "order by field_sirius_date_start_value ";
  }

  $limit = $params['limit'];
  if (!$limit) { $limit = 100; }
  $sql .= "limit $limit";

  $stmt = sirius_sql_query($sql, $sql_args);
  $assignments = array();
  while ($hr = $stmt->fetchAssoc()) {
    if ($hr['assignment_extra']) { 
      $hr['assignment_extra'] = json_decode($hr['assignment_extra'], TRUE);
    } else {
      $hr['assignment_extra'] = array();
    }
    $assignments[$hr['assignment_id']] = $hr;
  }

  return $assignments;
}

function sirius_edls_assignment_fetch_sheet_worker($date, $sheet_nid, $worker_id) {
  $sql = "select * from sirius_edls_assignments ";
  $sql .= "where assignment_date = :date and sirius_edls_assignments.sheet_nid = :sheet_nid and worker_id = :worker_id ";
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
  if (!$worker || !$worker['worker_name']) { return array('success' => FALSE, 'msg' => "Worker ID $worker_id not found."); }

  $crew = sirius_edls_crew($sheet_node, $crew_uuid);
  if (!$crew) { return array('success' => FALSE, 'msg' => 'Crew not found.'); }

  $assignments = sirius_edls_assignment_fetch_crew($sheet_node, $crew_uuid);
  if (!$assignments) { $assignments = array(); }
  if (count($assignments) >= $crew['count']) { return array('success' => FALSE, 'msg' => 'The crew is full.'); }

  if (!sirius_edls_sheet_reserved($sheet_node)) {
    if (preg_match('/^I/', $worker_id)) { 
      $worker_id_external = preg_replace('/^I/', '', $worker_id);
      $worker_nid =  sirius_worker_get_by_id($worker_id_external, TRUE);
      if ($worker_nid) {
        $sched = sirius_edls_sched_check_worker_day($employer_nid, $worker_nid, $date);
        if ($sched) {
          return array('success' => FALSE, 'msg' => "Schedule conflict: " . $sched['json']['display']['type']);
        }
      }
    }
  }

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

  sirius_edls_set_changed_ts($sheet_nid);

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

  // We implement this as an unassign / reassign, to keep the number of primitive operations small,
  // and because it's nice if an "assignment" never changes.
  
  $result = sirius_edls_unassign($date, $worker_id, $sheet_nid);
  if (!$result['success']) { return $result; }

  $result = sirius_edls_assign($date, $worker_id, $sheet_nid, $assignment['crew_uuid'], $extra);
  if (!$result['success']) { return $result; }

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

  sirius_edls_set_changed_ts($sheet_nid);

  return array(
    'success' => TRUE,
    'msg' => "Unassigned $assignment[worker_name] ($worker_id)",
  );
}

function sirius_edls_assignment_cleanup($mode) {
  // Clean up assignments  where the sheet no longer exists
  $sql = "select assignment_id from sirius_edls_assignments ";
  $sql .= "left join node on node.nid = sheet_nid ";
  $sql .= "where nid is null ";
  $sql .= "order by assignment_id limit 5000 ";
  $stmt = db_query($sql, array());

  $count = 0;
  while ($hr = $stmt->fetchAssoc()) {
    ++$count;
    $assignment_id = $hr['assignment_id'];
    if ($mode == 'live') {
      sirius_minilog("Purging assignment $assignment_id");
      $sql2 = "delete from sirius_edls_assignments where assignment_id = :assignment_id ";
      $stmt2 = db_query($sql2, array(':assignment_id' => $assignment_id));
    } else {
      sirius_minilog("Purging assignment $assignment_id (skipping purge because mode = $mode)");
    }
  }

  return array('success' => TRUE, 'count' => $count);
}

function sirius_edls_assignment_render_extra($assignment, $render_options = array()) {
  if (!$assignment['assignment_id']) { return ''; }
  $extra = $assignment['assignment_extra'];
  if (!$extra) { $extra = array(); }
  if (!is_array($extra)) { $extra = json_decode($extra, TRUE); }

  $lookups = &drupal_static(__FUNCTION__);
  if (!isset($lookups)) {
    $lookups = array();
    $lookups['classification_options'] = sirius_edls_worker_classification_options();
  }

  $parts = array();
  if ($extra['classification']) {
    $parts[] = $lookups['classification_options'][$extra['classification']];
  }
  if ($extra['time']) {
    $parts[] = date('H:i', strtotime($extra['time']));
  }
  //if ($extra['truck']) {
  //$parts[] = 'Trk #' . $extra['truck'];
  //}
  if ($extra['note']) {
    $parts[] = $extra['note'];
  }

  $html = join(' ', array_filter($parts));
  if (!$render_options['text_only']) {
    if ($html) { $html = "<span class=\"sirius_edls_assignment_extra\">($html)</span>"; }
  }

  return $html;
}


function sirius_edls_assignment_list_render($assignments, $render_options) {
  if (!$assignments) { return '<p>' . t('No assignments found.') . '</p>'; }

  // Can I view *all* notifications
  $can_view_notifications = sirius_access_instance('edls')->access('notification_view_delivery');

  $rows = array();
  foreach ($assignments as $assignment_id => $assignment) {
    $sheet_node = node_load($assignment['sheet_nid']);
    $notification = sirius_edls_notify_fetch_by_assignment_id($assignment_id);
    


    $tools = l('Sheet', 'node/' . $assignment['sheet_nid'] . '/sirius_edls_sheet_view', array('attributes' => array('class' => array('tool-button'))));
    if ($can_view_notifications && $notification) {
      $tools .= l('Notification', 'edls-notify/' . $notification['notification_uuid'], array('attributes' => array('class' => array('tool-button'))));
    }

    $row = array();
    $row[] = $assignment['assignment_date'];
    $row[] = $sheet_node->field_sirius_job_number['und'][0]['value'];
    $row[] = sirius_term_title($sheet_node->field_grievance_department_tid['und'][0]['tid']);
    $row[] = sirius_edls_assignment_render_extra($assignment, array('text_only' => TRUE));
    $row[] = $notification['notification_status'];
    $row[] = $tools;
    $rows[] = $row;
  }

  $header = array();
  $header[] = t('Date');
  $header[] = t('Job');
  $header[] = t('Dept');
  $header[] = t('Details');
  $header[] = t('Notification Status');
  $header[] = t('Tools');
  return sirius_table_render($header, $rows);
}
