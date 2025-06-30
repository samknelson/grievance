<?php

function sirius_smf_emphist($denorm_item) {
	$args = json_decode($denorm_item['denorm_trigger_args'], TRUE);
	$worker_nid = $denorm_item['denorm_target_nid'];

	// @todo: Ignoring mode (always live)
	sirius_smf_emphist_from_hours($worker_nid);
}

function sirius_smf_emphist_from_hours($worker_nid) {
  // Make sure we're in the correct domain
  $worker_node = node_load($worker_nid);
  sirius_domain($worker_node->field_sirius_domain['und'][0]['target_id']);

  // Get the list of all employers ever
  $sql = "select distinct employer_nid ";
  $sql .= "from sirius_hours_cache ";
  $sql .= "where worker_nid = :worker_nid ";
  $stmt = sirius_sql_query($sql, array(':worker_nid' => $worker_nid));
  $employer_nids = array();
  while ($hr = $stmt->fetchAssoc()) { $employer_nids[] = $hr['employer_nid']; }

  foreach ($employer_nids as $employer_nid) {
    sirius_minilog("Computing employment history: " . sirius_node_title($employer_nid));
    sirius_minilog_indent();
    sirius_smf_emphist_from_hours_employer($worker_nid, $employer_nid);
    sirius_minilog_outdent();
  }

  $lookups =  &drupal_static(__FUNCTION__);

  // Get the member status TID for each special member status
  if (!isset($lookups['ms_map'])) {
    $ms_map = array();
    foreach (array('H60', 'P100', 'E60', 'E80', 'E100') as $id) {
      $ms_map[$id] = sirius_taxonomy_lookup_by_anything('sirius_member_status', $id);
      if (!$ms_map[$id]) {
        sirius_minilog("No member status found with ID $id.", 'error');
        return;
      }
    }
    $lookups['ms_map'] = $ms_map;
  }
  $ms_map = $lookups['ms_map'];


  if (!isset($lookups['industry_map'])) {
    foreach (array('H', 'P', 'E') as $id) {
      $industry_map[$id] = sirius_taxonomy_lookup_by_anything('sirius_industry', $id);
      if (!$industry_map[$id]) {
        sirius_minilog("No industry found with ID $id.", 'error');
        return;
      }
    }
    $lookups['industry_map'] = $industry_map;
  }
  $industry_map = $lookups['industry_map'];

  // Get the date of first hours for each employer
  $first_hours_by_industry = array();
  foreach ($employer_nids as $employer_nid) {
    $sql = "select * from sirius_hours_cache ";
    $sql .= "where employer_nid = :employer_nid ";
    $sql .= "and worker_nid = :worker_nid ";
    $sql .= "and hours > 0 ";
    $sql .= "order by year, month, day limit 1 ";
    $stmt = sirius_sql_query($sql, array(':worker_nid' => $worker_nid, ':employer_nid' => $employer_nid));
    $hr = $stmt->fetchAssoc();
    if (!$hr) { continue; }

    $employer_node = node_load($employer_nid);
    $industry_tid = $employer_node->field_sirius_industry['und'][0]['tid'];
    if (!$industry_tid) { continue; }

    $date = date('Y-m-d', strtotime("$hr[year]-$hr[month]-$hr[day]"));

    if ($first_hours_by_industry[$industry_tid] && ($first_hours_by_industry[$industry_tid] <= $date)) { continue; }
    $first_hours_by_industry[$industry_tid] = $date;
  }

  // Figure out the correct member status
  sirius_minilog("Computing member status");
  sirius_minilog_indent();
  foreach ($industry_map as $industry_id => $industry_tid) {
    if (!$first_hours_by_industry[$industry_tid]) {
      sirius_minilog("For industry $industry_id, no hours found. Skipping.");
      continue;
    }

    if ($industry_id == 'H') {
      $ms_id = 'H60';
    } else if ($industry_id == 'P') {
      $ms_id = 'P100';
    } else if ($industry_id == 'E') {
      if (in_array($ms_map['E60'], sirius_fieldvals($worker_node, 'field_sirius_member_status', 'tid'))) {
        sirius_minilog("For industry $industry_id, the member already has member status E60. Skipping.");
        continue; 
      }

      if (time() - strtotime($first_hours_by_industry[$industry_tid]) > 60*60*24*365*5) {
        $ms_id = 'E80';
      } else {
        $ms_id = 'E100';
      }
    } else {
      continue; 
    }

    $memo = "For industry $industry_id, Setting member status $ms_id because first hours was $first_hours_by_industry[$industry_tid]";
    sirius_minilog($memo);
    $result = sirius_worker_member_status_set($worker_node, taxonomy_term_load($ms_map[$ms_id]), TRUE, $memo);
    if ($result['success']) {
      sirius_minilog("... Result: " . $result['msg']);
    } else {
      sirius_minilog("... Result: " . $result['msg'], 'warning');
    }
  }
  sirius_minilog_outdent();
  sirius_minilog("Work status history and member status computations complete.");
}

function sirius_smf_emphist_from_hours_employer($worker_nid, $employer_nid) {
  // Don't calculate past "now".
  $now_year = date('Y');
  $now_month = date('m');
  $now_ym = sirius_smf_emphist_from_hours_calc_ym($now_year, $now_month);
  $last_ym = $now_ym;

  // Get the "term" hour type
  $lookups =  &drupal_static(__FUNCTION__);
  if (!$lookups['term_type_tid']) {
    $lookups['term_type_tid'] = sirius_taxonomy_lookup_by_anything('sirius_hour_type', 'terminated');
    if (!$lookups['term_type_tid']) {
      sirius_minilog("No hour type has the ID 'terminated'.", 'error');
      return;
    }
  }
  $term_type_tid = $lookups['term_type_tid'];

  // Get the stub job
  $employer_node = node_load($employer_nid);
  $industry_tid = $employer_node->field_sirius_industry['und'][0]['tid'];
  if (!$industry_tid) {
    sirius_minilog("Missing industry for employer " . $employer_node->title . ' (' . $employer_nid . ')', 'error');
    return;
  }
  $stub_nid = sirius_dispatch_job_stub_fetch($employer_nid, $industry_tid, TRUE);
  if (!$stub_nid) {
    sirius_minilog("Missing stub job for employer " . $employer_node->title . ' (' . $employer_nid . ') with industry ' . sirius_term_title($industry_tid) . ' (' . $industry_tid . ')', 'error');
    return;
  }

  // Fetch the worker's hours by month and type
  $sql = "select year, month, sum(hours) as h, hours_type_tid ";
  $sql .= "from sirius_hours_cache ";
  $sql .= "where worker_nid = :worker_nid ";
  $sql .= "and employer_nid = :employer_nid ";
  $sql .= "group by year, month, hours_type_tid ";
  $sql .= "order by year, month, hours_type_tid ";
  $stmt = sirius_sql_query($sql, array(':worker_nid' => $worker_nid, ':employer_nid' => $employer_nid));
  $data = array();
  $first_ym = NULL;
  while ($hr = $stmt->fetchAssoc()) {
    $year = $hr['year'];
    $month = $hr['month'];
    $ym = sirius_smf_emphist_from_hours_calc_ym($year, $month);
    if (!$first_ym) { $first_ym = $ym; }
    $data[$ym]['total'] += $hr['h'];
    $data[$ym][$hr['hours_type_tid']] += $hr['h'];
  }
  sirius_minilog("Computing from $first_ym through $last_ym");

  // Get a list of months
  $ym = $first_ym;
  list ($year, $month) = explode('-', $ym);
  $avoid_infinite_loops = 0;
  $yms = array();
  while ($ym < $last_ym) {
    $yms[] = $ym;

    ++$month; 
    if ($month > 12) { $month = 1; ++$year; }
    $ym = sirius_smf_emphist_from_hours_calc_ym($year, $month);
    ++$avoid_infinite_loops; if ($avoid_infinite_loops > 10000) { break; }
  }

  // Find out if the worker is working in each of these months.
  $status_by_ym = array();
  $status = 'inactive';
  foreach ($yms as $ym) {
    if (isset($data[$ym][$term_type_tid])) {
      // We were terminated. Set term status
      // - If we had any active status previously, OR
      // - If we have nonzero hours (i.e. terminated and started in the same month)
      if ( (in_array($status, array('start', 'active'))) || ($data[$ym]['total'] > 0) ) {
        $status = 'term';
      } else {
        $status = 'inactive';
      }
    } else if ($data[$ym]['total'] > 0) {
      // We have nonzero hours.
      // - Set "start" if we haven't started yet
      // - Otherwise set "active"
      if (!in_array($status, array('start', 'active'))) { $status = 'start'; }
      else { $status = 'active'; }
    } else {
      // We have zero hours.
      // - Set "active" if we just started
      // - Set "inactive" if we just termed
      // - Otherwise do nothing
      if ($status == 'start') { $status = 'active'; }
      else if ($status == 'term') { $status = 'inactive'; }
      else { }
    }

    $status_by_ym[$ym] = $status;
  }

  // Fetch all the dispatches
  $existing_dispatches = sirius_dispatch_emphist_search($worker_nid, array('Primary', 'Secondary'), array('employer_nid' => $employer_nid));

  // Walk the yms
  $wh_records = array();
  foreach ($status_by_ym as $ym => $status) {
    // Start new records
    if ($status == 'start' || ($status == 'term' && !$wh_record)) {
      $wh_record = array();
      $wh_record['employer_nid'] = $employer_nid;
      $wh_record['start'] = "$ym-01";
      $wh_record['type'] = 'Secondary';
      $wh_record['job_nid'] = $stub_nid;
      $wh_record['worker_nid'] = $worker_nid;
      $wh_record['ym'] = $ym;

      $wh_records[$ym] = $wh_record;
    }
    
    if ($status == 'term') {
      $t = date('t', strtotime("$ym-15"));
      $wh_records[$wh_record['ym']]['end'] = "$ym-$t";
    }

    if ($status == 'term' || $status == 'inactive') {
      unset($wh_record);
    }
  }

  // Check our existing dispatches and see if they're correct
  $nodupes = array();
  foreach ($existing_dispatches as $dispatch_nid => $dispatch_node) {
    $is_bad = FALSE;

    // If we don't have a start date, we're bad.
    $ts = sirius_fieldval($dispatch_node, 'field_sirius_date_start', 'ts');
    if (!$ts) { $is_bad = "No start date"; }

    // If we are a primary dispatch, we're bad.
    if ($dispatch_node->field_sirius_dispatch_status['und'][0]['value'] == 'accepted') { $is_bad = "Accepted primary"; }

    if ($dispatch_node->field_sirius_dispatch_type['und'][0]['value'] != 'Secondary') { $is_bad = "Primary-type dispatch"; }

    // get our wh_record
    $wh_record = sirius_dispatch_emphist_record_from_node($dispatch_node);
    $ym = date('Y-m', strtotime($wh_record['start']));

    // Avoid dupes
    if ($nodupes[$wh_record['start']]) { $is_bad = "Two records with the same start date"; }
    $nodupes[$ym] = $wh_record;

    // Find out if the records match
    if (!$wh_records[$ym]) {
      $is_bad = "Start date isn't correct.";
    } else {
      $wh_records[$ym]['dispatch_nid'] = $wh_record['dispatch_nid'];
      if ($wh_records[$ym]['end'] != $wh_record['end']) { $wh_records[$ym]['needs_save'] = TRUE; }
    }

    if ($is_bad) { 
      sirius_minilog("Deleting existing dispatch " . $dispatch_node->title . " ($dispatch_nid) because: $is_bad");
      node_delete($dispatch_nid);
      unset($existing_dispatches[$dispatch_nid]);
    }
  }

  // Save
  foreach ($wh_records as $wh_record) {
    sirius_minilog("Setting employment history record: " . print_r($record, TRUE) . " ...");
    if ($wh_record['dispatch_nid'] && !$wh_record['needs_save']) {
      sirius_minilog("... No changes. Skipping save.");
      continue;
    }
    $result = sirius_dispatch_emphist_save($wh_record, $wh_record['dispatch_nid']);
    if ($result['success']) {
      if ($result['msg']) { sirius_minilog($result['msg']); }
    } else {
      sirius_minilog($result['msg'], 'error');
    }
  }
}

function sirius_smf_emphist_from_hours_calc_ym($y, $m) {
  if (strlen($m) < 2) { $m = "0$m"; }
  return "$y-$m";
}
