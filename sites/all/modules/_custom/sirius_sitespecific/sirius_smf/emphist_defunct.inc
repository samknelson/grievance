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

  // Don't calculate past "now".
  $now_year = date('Y');
  $now_month = date('m');
  $now_ym = sirius_smf_emphist_from_hours_calc_ym($now_year, $now_month);
  sirius_minilog("Starting SMF calculations: Worker $worker_nid as of $now_ym");

  // Start year and month
  $first_year = date('Y');
  $first_month = date('m');
  $first_ym = sirius_smf_emphist_from_hours_calc_ym($first_year, $first_month);

  // Load all the worker's hours.
  // Break out by month, and include payperiod data like eligible hours by employer
  $sql = "select year, month, employer_nid, sum(hours) as h ";
  $sql .= "from sirius_hours_cache ";
  $sql .= "where worker_nid = :worker_nid ";
  $sql .= "group by year, month, employer_nid ";
  $sql .= "order by year, month ";
  $stmt = sirius_sql_query($sql, array(':worker_nid' => $worker_nid));
  $data = array();
  $first_data_ym = NULL;
  while ($hr = $stmt->fetchAssoc()) {
    $year = $hr['year'];
    $month = $hr['month'];
    $employer_nid = $hr['employer_nid'];
    $hrs = $hr['h'];
    if ($hrs <= 0) { continue; }
    $ym = sirius_smf_emphist_from_hours_calc_ym($year, $month);
    $last_ym = $ym;
    if (!$first_data_ym) { $first_data_ym = $ym; }

    // Look up the eligible hours
    $payperiod_node = sirius_hour_payperiod_load(sirius_domain_nid(), $worker_nid, $employer_nid, $year, $month, 1);
    $payperiod_json = sirius_json_get($payperiod_node);
    $elig_hrs = $payperiod_json['smf']['elig_hrs'];
    $elig_comment = '';
    if (!$elig_hrs) {
      $elig_hrs = 80;
      $elig_comment = "(elig hours not found, defaulting to $elig_hrs)";
    }

    // Invert hrs, so that all keys are sorted in ascending order (i.e. -100 before -50)
    $hrs *= -1;

    $data[$ym][$elig_hrs][$hrs][] = $employer_nid;
  }

  // Start date should be whenever we start our data
  if ($first_data_ym) {
    $first_ym = $first_data_ym;
  }

  // End date is six months after the last data, but no later than today.
  list ($last_year, $last_month) = explode('-', $last_ym);
  $last_month += 6;
  if ($last_month > 12) { $last_month -= 12; $last_year++; }
  $last_ym = sirius_smf_emphist_from_hours_calc_ym($last_year, $last_month);
  if ($last_ym > $now_ym) { $last_ym = $now_ym; }
  sirius_minilog("Calculating work history from $first_ym through $last_ym");

  // There might be gaps in our data. Pad with blank entries, so that we can confidently iterate over data.
  if ($first_ym && $last_ym) {
    $ym = $first_ym;
    list ($year, $month) = explode('-', $ym);
    $avoid_infinite_loops = 0;
    while ($ym < $last_ym) {
      ++$month; 
      if ($month > 12) { $month = 1; ++$year; }
      $ym = sirius_smf_emphist_from_hours_calc_ym($year, $month);
      if (!$data[$ym]) { $data[$ym] = array(); }
      ++$avoid_infinite_loops; if ($avoid_infinite_loops > 10000) { break; }
    }
  }

  // Fetch all the dispatches
  $existing_dispatches = sirius_dispatch_emphist_search($worker_nid, array('Primary', 'Secondary'));

  // Delete any existing dispatches that start after start_date
  foreach ($existing_dispatches as $dispatch_nid => $dispatch_node) {
    $ts = sirius_fieldval($dispatch_node, 'field_sirius_date_start', 'ts');
    if ($ts) { $ym = gmdate('Y-m', $ts); } else { $ym = ''; }
    if ((!$ym) || ($ym >= $first_ym)) {
      sirius_minilog("... Deleting existing dispatch " . $dispatch_node->title . " ($dispatch_nid) because its start date, $ym, is after our computation date, $first_ym");
      node_delete($dispatch_nid);
      unset($existing_dispatches[$dispatch_nid]);
    }
  }

  // Pre-populate our existing records with any currently active dispatches
  $wh_records_active = $wh_records_complete = array();
  foreach ($existing_dispatches as $dispatch_nid => $dispatch_node) {
    $record = sirius_dispatch_emphist_record_from_node($dispatch_node);
    if (!$record['end'] || ($record['end'] >= "$first_ym-01")) {
      $key = $record['employer_nid'] . '|' . $record['type'];
      $wh_records_active[$key] = $record;
    } 
  }

  // The requirements say: Sort a worker's aggregated hours on the following:
  // - number of hours required for benefits (05 SMCCON-ELIGHRS), least to greatest
  // - number of hours reported, greatest to least (05 SMCCON-HRS)
  //
  // Because of the structure of $data, this is just a recursive ksort, how clever.
  //
  // [Note: this means that if e.g. you worked 10 hours as a 60-hour employer, and 120 hours at an 80-hour employer,
  // then your home employer is the 60-hour employer. Counterintuitive but that's the algorithm.]
  sirius_smf_emphist_from_hours_ksort_recursive($data);

  // Walk the month, generating employment records. As we go, we want to generate wh_records, extending as necessary.
  // There might be gaps in the data, so we want to generate a new record if necessary or extend an existing one.
  // So we maintain two arrays
  // 
  // $wh_records_active (keyed by [$employer_nid|$type]) and
  // $wh_records_complete (unkeyed)
  //

  foreach ($data as $ym => $month_record) {
    // if ($ym < $first_ym) { continue; }
    list ($year, $month) = explode('-', $ym);
    sirius_minilog("... Computing work history: $ym");

    $start_date = $ym . '-01';
    $end_date = $ym . '-' . date('t', strtotime($start_date));

    $primary_employer_nid = NULL;
    $secondary_employer_nids = array();

    foreach ($month_record as $elig_hrs => $elig_hrs_record) {
      foreach ($elig_hrs_record as $hrs => $employers) {
        $hrs *= -1;
        foreach ($employers as $employer_nid) {
          if (!$primary_employer_nid) { $primary_employer_nid = $employer_nid; }
          else { $secondary_employer_nids[$employer_nid]++; }
        }
      }
    }
    unset($secondary_employer_nids[$primary_employer_nid]);
    $secondary_employer_nids = array_keys($secondary_employer_nids);

    if ($primary_employer_nid) {
      $msg = "$primary_employer_nid (" . sirius_node_title($primary_employer_nid) . ")";
    } else { 
      $msg = "NONE";
    }
    sirius_minilog("... ... Primary employer: $msg");

    if ($secondary_employer_nids) {
      $msg = '';
      foreach ($secondary_employer_nids as $secondary_employer_nid) {
        if ($msg) { $msg .= "; "; }
        $msg .= $secondary_employer_nid . " (" . sirius_node_title($secondary_employer_nid) . ")";
      }
    } else {
      $msg = "NONE";
    }
    sirius_minilog("... ... Secondary employers: $msg");

    // Add our primary employer
    if ($primary_employer_nid) {
      $key = "$primary_employer_nid|Primary";
      if ($wh_records_active[$key]) {
        // $wh_records_active[$key]['end'] = $end_date;
      } else {
        $wh_records_active[$key] = array('employer_nid' => $primary_employer_nid, 'start' => $start_date, 'type' => 'Primary');
      }

      // Terminate any existing primary dispatches
      foreach ($wh_records_active as $key => $record) {
        if ($primary_employer_nid && ($key == "$primary_employer_nid|Primary")) { continue; }
        if (!preg_match('/Primary/', $key)) { continue; }
        $record['end'] = date('Y-m-t', strtotime("-1 day", strtotime($start_date)));
        unset($wh_records_active[$key]);
        $wh_records_complete[] = $record;
      }
    }

    // Add all our secondary employers
    foreach ($secondary_employer_nids as $secondary_employer_nid) {
      $key = "$secondary_employer_nid|Secondary";
      if ($wh_records_active[$key]) {
        // $wh_records_active[$key]['end'] = $end_date;
      } else {
        $wh_records_active[$key] = array('employer_nid' => $secondary_employer_nid, 'start' => $start_date, 'type' => 'Secondary');
      }
    }

    // Terminate any existing secondary dispatches
    /*
    foreach ($wh_records_active as $key => $record) {
      list ($secondary_employer_nid, $type) = explode('|', $key);
      if ($type != 'Secondary') { continue; }
      if (in_array($secondary_employer_nid, $secondary_employer_nids)) { continue; }
      $record['end'] = date('Y-m-t', strtotime("-1 day", strtotime($start_date)));
      unset($wh_records_active[$key]);
      $wh_records_complete[] = $record;
    }
    */
  }

  // All our records to save
  $wh_records = array_merge(array_values($wh_records_active), array_values($wh_records_complete));

  // Find stub jobs
  foreach ($wh_records as $idx => $record) {
    $wh_records[$idx]['worker_nid'] = $worker_nid;
    if (!$wh_records[$idx]['job_nid']) {
      $employer_nid = $wh_records[$idx]['employer_nid'];
      $employer_node = node_load($employer_nid);
      $industry_tid = $employer_node->field_sirius_industry['und'][0]['tid'];

      if (!$industry_tid) {
        sirius_minilog("Missing industry for employer " . $employer_node->title . ' (' . $employer_nid . ')', 'error');
        unset($wh_records[$idx]);
        continue;
      }

      $stub_nid = sirius_dispatch_job_stub_fetch($employer_nid, $industry_tid, TRUE);
      if (!$stub_nid) {
        sirius_minilog("Missing stub job for employer " . $employer_node->title . ' (' . $employer_nid . ') with industry ' . sirius_term_title($industry_tid) . ' (' . $industry_tid . ')', 'error');
        unset($wh_records[$idx]);
        continue;
      }

      $wh_records[$idx]['job_nid'] = $stub_nid;
    }
  }

  // Save
  foreach ($wh_records as $wh_record) {
      sirius_minilog("... Saving employment history record: " . print_r($record, TRUE) . " ...");
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

function sirius_smf_emphist_from_hours_ksort_recursive(&$a) {
  if (!is_array($a)) { return; }
  ksort($a);
  foreach ($a as $k=>$v) {
    sirius_smf_emphist_from_hours_ksort_recursive($a[$k]);
  }
}
