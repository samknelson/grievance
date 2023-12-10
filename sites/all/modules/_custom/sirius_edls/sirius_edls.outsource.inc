<?php

function sirius_edls_outsource_encode($workers) {
  $lines = array();
  foreach ($workers as $worker_record) {
    $worker_name = $worker_record['worker_name'];
    $worker_ms = $worker_record['worker_ms'];
    $worker_id = $worker_record['worker_id'];
    $worker_id = preg_replace('/^O/', '', $worker_id);

    $lines[] = "$worker_id|$worker_ms|$worker_name";
  }
  return join("\n", $lines);
}

function sirius_edls_outsource_decode($workers_encode) {
  $lines = explode("\n", $workers_encode);
  $workers = array();
  foreach ($lines as $line) {
    list ($worker_id, $worker_ms, $worker_name) = explode('|', $line);

    $worker_name = trim($worker_name);
    $worker_name = preg_replace('/[^a-zA-Z0-9_, ]/', '', $worker_name);
    $worker_name = preg_replace('/\s+/', ' ', $worker_name);

    $worker_id = trim($worker_id);
    $worker_id = preg_replace('/[^a-zA-Z0-9_]/', '', $worker_id);
    $worker_id = preg_replace('/\s+/', ' ', $worker_id);

    $worker_ms = trim($worker_ms);
    $worker_ms = preg_replace('/[^a-zA-Z0-9_]/', '', $worker_ms);
    $worker_ms = preg_replace('/\s+/', ' ', $worker_ms);

    if (!$worker_id && !$worker_ms && !$worker_name) { continue; }
    if (!$worker_id || !$worker_ms || !$worker_name) { return array('success' => FALSE, 'msg' => "Please make sure that each row has a worker ID, member status, and name ($worker_id, $worker_ms, $worker_name)"); }

    $worker_id = 'O' . $worker_id;

    $workers[] = array('worker_name' => $worker_name, 'worker_ms' => $worker_ms, 'worker_id' => $worker_id);
  }

  return array('success' => TRUE, 'workers' => $workers);
}

function sirius_edls_outsource_list($employer_nid, $date = NULL, $filters = array()) {
  // Why do we want the date?
  //
  // Ideally, this function would fetch a list of workers, and then we'd look up each worker's assignment.
  // But that's expensive. This function is called very frequently, and we're using raw SQL for speed.
  // So we join against the assignment list. If we have no date, pick a random one.
  if (!$date) { $date = '1970-01-01'; }

  $employer_node = node_load($employer_nid);
  $ms_list = sirius_edls_outsource_ms_list($employer_node, 'full');

  $sql_args = array();
  $sql = "select ";
  $sql .= "( ";
  $sql .= "  select sheet_nid from sirius_edls_assignments ";
  $sql .= "  where sirius_edls_assignments.worker_id = sirius_edls_outsource.worker_id ";
  $sql .= "  and sirius_edls_assignments.employer_nid = sirius_edls_outsource.employer_nid ";
  $sql .= "  and sirius_edls_assignments.assignment_date = :date ";
  $sql .= ") as curr_sheet_nid, ";
  $sql .= "( ";
  $sql .= "  select sheet_nid from sirius_edls_assignments ";
  $sql .= "  where sirius_edls_assignments.worker_id = sirius_edls_outsource.worker_id ";
  $sql .= "  and sirius_edls_assignments.employer_nid = sirius_edls_outsource.employer_nid ";
  $sql .= "  and sirius_edls_assignments.assignment_date < :date ";
  $sql .= "  order by assignment_date desc limit 1 ";
  $sql .= ") as prev_sheet_nid, ";
  $sql .= "( ";
  $sql .= "  select sheet_nid from sirius_edls_assignments ";
  $sql .= "  where sirius_edls_assignments.worker_id = sirius_edls_outsource.worker_id ";
  $sql .= "  and sirius_edls_assignments.employer_nid = sirius_edls_outsource.employer_nid ";
  $sql .= "  and sirius_edls_assignments.assignment_date > :date ";
  $sql .= "  order by assignment_date limit 1 ";
  $sql .= ") as next_sheet_nid, ";
  $sql .= "sirius_edls_outsource.* ";
  $sql .= "from sirius_edls_outsource ";
  $sql_args['date'] = $date;
  $sql .= "where sirius_edls_outsource.employer_nid = :employer_nid ";
  $sql_args[':employer_nid'] = $employer_nid;

  if ($filters['nameid']) { 
    $sql .= "and (sirius_edls_outsource.worker_name like :match or sirius_edls_outsource.worker_id like :match) ";
    $sql_args[':match'] = '%' . $filters['nameid'] . '%';
  }

  if ($filters['ms']) {
    $sql .= "and sirius_edls_outsource.worker_ms = :ms ";
    $sql_args[':ms'] = $filters['ms'];
  }

  // $sql .= "limit 20";
  $stmt = sirius_sql_query($sql, $sql_args);

  $workers = array();
  while ($hr = $stmt->fetchAssoc()) {
    $workers[] = $hr;
  }

  usort($workers, function($a, $b) use ($ms_list) {
    $ms_weight_a = $ms_list[$a['worker_ms']]['weight'];
    $ms_weight_b = $ms_list[$b['worker_ms']]['weight'];
    if ($ms_weight_a < $ms_weight_b) { return -1; }
    if ($ms_weight_a > $ms_weight_b) { return 1; }
    return strcmp($a['worker_name'], $b['worker_name']);
  });

  return $workers;
}

function sirius_edls_outsource_set_all($employer_node, $new_workers) {
  // Check member statuses
  $ms_options = sirius_edls_outsource_ms_list($employer_node);
  if (!$ms_options) {
    return array('success' => FALSE, 'msg' => 'Please configure at least one member status.');
  }
  foreach ($new_workers as $worker_record) {
    $ms = $worker_record['worker_ms'];
    if (!$ms_options[$ms]) {
      return array('success' => FALSE, 'msg' => "Please make sure that every member status is configured [$ms].");
    }
  }

  // Delete the old workers
  $employer_nid = $employer_node->nid;
  $old_workers = sirius_edls_outsource_list($employer_nid);
  foreach ($old_workers as $worker_record) {
    $worker_id = $worker_record['worker_id'];
    if ($new_workers[$worker_id]) { continue; }
    $sql = "delete from sirius_edls_outsource where employer_nid = :employer_nid and worker_id = :worker_id";
    $stmt = db_query($sql, array(':employer_nid' => $employer_nid, ':worker_id' => $worker_id));
  }

  // Create or update the new workers
  foreach ($new_workers as $worker_record) {
    $worker_id = $worker_record['worker_id'];
    $sql = "insert into sirius_edls_outsource ";
    $sql .= "(employer_nid, worker_id, worker_ms, worker_name) ";
    $sql .= "values ";
    $sql .= "(:employer_nid, :worker_id, :worker_ms, :worker_name) ";
    $sql .= "on duplicate key update worker_name = :worker_name ";
    $sql_args = array();
    $sql_args[':employer_nid'] = $employer_nid;
    $sql_args[':worker_id'] = $worker_id;
    $sql_args[':worker_name'] = $worker_record['worker_name'];
    $sql_args[':worker_ms'] = $worker_record['worker_ms'];
    $stmt = db_query($sql, $sql_args);
  }

  return array('success' => TRUE);
}

function sirius_edls_outsource_worker_fetch($employer_nid, $worker_id) {
  $sql = "select * from sirius_edls_outsource where employer_nid = :employer_nid and worker_id = :worker_id ";
  $stmt = db_query($sql, array(':employer_nid' => $employer_nid, ':worker_id' => $worker_id));
  return $stmt->fetchAssoc();
}

function sirius_edls_outsource_ms_set_all($employer_node, $options) {
  $employer_json = sirius_json_get($employer_node);
  $employer_json['edls']['outsource']['ms']['options'] = $options;
  sirius_json_set($employer_node, $employer_json);
  node_save($employer_node);
}

function sirius_edls_outsource_ms_list($employer_node, $type = 'name') {
  $employer_json = sirius_json_get($employer_node);
  $items = $employer_json['edls']['outsource']['ms']['options'];
  if (!$items) { $items = array(); }

  uksort($items, function($a, $b) use ($items) { return $items[$a]['weight'] <=> $items[$b]['weight']; });

  if ($type == 'full') {
    return $items;
  }

  $result = array();
  foreach ($items as $key => $record) {
    $result[$key] = $record['ms_name'];
  }

  return $result;
}

function sirius_edls_outsource_ms_encode($options) {
  $lines = array();
  foreach ($options as $ms_key => $ms_value) {
    $line = "$ms_key|$ms_value";
    $lines[] = $line;
  }
  return join("\n", $lines);
}

function sirius_edls_outsource_ms_decode($options_encode) {
  $lines = explode("\n", $options_encode);
  $options = array();
  $weight = 0;
  foreach ($lines as $line) {
    list ($ms_key, $ms_name) = explode('|', $line);

    $ms_key = trim($ms_key);
    $ms_key = preg_replace('/[^a-zA-Z0-9_]/', '', $ms_key);

    $ms_name = trim($ms_name);
    $ms_name = preg_replace('/[^a-zA-Z0-9_ ]/', '', $ms_name);
    $ms_name = preg_replace('/\s+/', ' ', $ms_name);

    if (!$ms_name && !$ms_key) { continue; }
    if (!$ms_name || !$ms_key) { return array('success' => FALSE, 'msg' => "Please make sure that each row has a key and name ($ms_key, $ms_name)"); }

    $options[$ms_key] = array('ms_key' => $ms_key, 'ms_name' => $ms_name, 'weight' => $weight);
    ++$weight;
  }

  return array('success' => TRUE, 'options' => $options);
}
