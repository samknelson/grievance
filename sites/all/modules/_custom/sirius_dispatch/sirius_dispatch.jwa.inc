<?php

function sirius_dispatch_jwa_prefs() {
	global $user;
	$user_full = user_load($user->uid);
	$json = sirius_json_get($user_full);
	return $json['sirius']['dispatch']['jwa']['prefs'];
}

function sirius_dispatch_jwa_hours_render($prefs, $hr) {
	$hours = '';
	$is_first = TRUE;
	$delta = 0;
	foreach ($prefs['display']['hour_type'] as $type) { 
		if ($is_first) { $is_first = FALSE; } else { $hours .= '&nbsp;/&nbsp;'; }
		$hours .= round($hr["hours_$delta"], 2); 
		++$delta;
	}
	return $hours;
}

function sirius_dispatch_jwa_jobs($prefs) {
	$query = new EntityFieldQuery();
	$query->entityCondition('entity_type', 'node');
	$query->entityCondition('bundle', 'sirius_dispatch_job');
	$query->fieldCondition('field_sirius_dispatch_job_status', 'value', array('Unfilled', 'Running')); 
	$job_type_tids = array();
	if ($prefs['jobs']['job_type']) {
		$job_type_tids = array_filter(array_values($prefs['jobs']['job_type']));
	}
	if ($job_type_tids) {
		$query->fieldCondition('field_sirius_dispatch_job_type', 'tid', $job_type_tids, 'in');
	}
	$query->propertyOrderBy('nid', 'desc');
	$stmt = $query->execute();
	if ($stmt['node']) { 
		$job_nids = array_keys($stmt['node']);
	} else {
		$job_nids = array();
	}
	$job_max = variable_get('sirius_dispatch_jwa_job_max', 25);
	$job_count = count($job_nids);
	if ($job_count > $job_max) {
		drupal_set_message(t('There @job_count jobs with status "Pending", "Unfilled", or "Running" that  matching your criteria. Using the first @job_max jobs only.', array('@job_max' => $job_max, '@job_count' => $job_count)), 'warning');
		$job_nids = array_slice($job_nids, 0, $job_max);
	}
	return node_load_multiple($job_nids);
}

function sirius_dispatch_jwa_roster($prefs, $job_nids) {
	if (!$job_nids) { return arary(); }
	$roster = array();
	$sql = "select node.nid as dispatch_nid, ";
	$sql .= "worker_node.nid as worker_nid, ";
	$sql .= "field_sirius_dispatch_job_target_id as job_nid, ";
	$sql .= "worker_node.title as worker_name, ";
	$sql .= "field_sirius_dispatch_status_value as dispatch_status ";
	$sql .= "from node ";
	$sql .= "left join field_data_field_sirius_dispatch_job on field_data_field_sirius_dispatch_job.entity_type = 'node' and field_data_field_sirius_dispatch_job.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_worker on field_data_field_sirius_worker.entity_type = 'node' and field_data_field_sirius_worker.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_dispatch_status on field_data_field_sirius_dispatch_status.entity_type = 'node' and field_data_field_sirius_dispatch_status.entity_id = node.nid ";
	$sql .= "left join node worker_node on worker_node.nid = field_sirius_worker_target_id ";
	$sql .= "where node.type = 'sirius_dispatch' ";
	$sql .= "and field_sirius_dispatch_job_target_id in (:job_nids) ";
	$sql .= "order by worker_node.title, node.nid desc limit 5000 ";
	$stmt = db_query($sql, array(':job_nids' => $job_nids));
	$roster = array();
	while ($hr = $stmt->fetchAssoc()) {
		$roster[$hr['job_nid']][] = $hr;
	}
	return $roster;
}

function sirius_dispatch_jwa_hours_clause($prefs) {
  $sql = "";
  $delta = 0;
  $date_range = $prefs['display']['date_range'];
  $date_range_days = $prefs['display']['date_range_days'];
  $date_range_months = $prefs['display']['date_range_months'];
  $month = date('m');
  $year = date('Y');
  $day = date('d');
  if ($date_range == 'all') {
    // No date range
  } else if ($date_range == 'this_month') {
    $target_year = $year;
    $target_month = $month;
    $date_range_sql = "(year = $target_year and month = target_month) ";
  } else if ($date_range == 'last_month') {
    $target_year = $year;
    $target_month = $month--;
    if ($target_month < 1) { $target_month = 12; --$target_year; }
    $date_range_sql = "(year = $target_year and month = target_month) ";
  } else if ($date_range == 'this_year') {
    $target_year = $year;
    $date_range_sql = "(year = $target_year) ";
  } else if ($date_range == 'last_year') {
    $target_year = $year-1;
    $date_range_sql = "(year = $target_year) ";
  } else if ($date_range == 'days') {
    $target_date = date('Y-m-d', strtotime("-$date_range_days days"));
    list ($target_year, $target_month, $target_day) = explode('-', $target_date);
    $target_thingy = ($target_year * 1000000) + $target_month * 1000 + $target_day;
    $date_range_sql = "( (year * 1000000 + month * 1000 + day) >= $target_thingy ) ";
  } else if ($date_range == 'months') {
    $target_date = date('Y-m', strtotime("-$date_range_months months"));
    list ($target_year, $target_month) = explode('-', $target_date);
    $target_thingy = ($target_year * 1000000) + $target_month * 1000;
    $date_range_sql = "( (year * 1000000 + month * 1000) >= $target_thingy ) ";
  }

  $delta = 0;
  if ($prefs['display']['hour_type']) {
	  foreach ($prefs['display']['hour_type'] as $type) {
	    $clauses = array();
	    list ($hours_type_tid, $department_tid) = explode('|', $type);
	    if ($hours_type_tid > 0) { $clauses[] = "hours_type_tid = $hours_type_tid "; }
	    if ($department_tid > 0) { $clauses[] = "department_tid = $department_tid "; }
	    if ($date_range_sql) { $clauses[] = $date_range_sql; }
	    if (!$clauses) { $clauses[] = 'TRUE'; }
	    $sql .= "\nSUM(CASE WHEN ( ";
	    $sql .= join(' AND ', $clauses);
	    $sql .= ") THEN hours ELSE 0 END) AS hours_$delta,\n ";
	    ++$delta;
	  }
	}

  return $sql;
}

function sirius_dispatch_jwa_workers($prefs) {
	$employers_lookup = sirius_employer_list();
  $all_levels = sirius_training_levels_fetch_all();
  $ms_lookup = sirius_taxonomy_load('sirius_member_status', 'tid', 'name');
  $ms_level_lookup = sirius_ledger_ms_level_options();
	$skill_lookup = sirius_taxonomy_load('sirius_skill', 'tid', 'name');

	$workers = array();

	$sql_args = array();
	$sql = "select node.nid as worker_nid, ";
	$sql .= sirius_dispatch_jwa_hours_clause($prefs);

	$worker_max = variable_get('sirius_dispatch_jwa_worker_max', 500);
	$sql .= "field_sirius_dispatch_availdate_value as availdate, ";
	$sql .= "node.title as worker_name, ";
	$sql .= "field_grievance_shop_target_id as employer_nid, ";
	$sql .= "field_sirius_id_value as sirius_id, ";
	$sql .= "ds_term.name as dispatch_status_name, ";
	$sql .= "field_sirius_dispatch_eba_value as eba_name, ";
	$sql .= "field_sirius_json_value as json_value, ";
	$sql .= "group_concat(distinct field_sirius_member_status_tid separator ',') as ms_tid, ";
	// $sql .= "group_concat(substr(field_sirius_dispatch_eba_dates_value, 6, 5) order by field_sirius_dispatch_eba_dates_value separator '<br>') as eba_dates ";
	$sql .= "(select group_concat(field_sirius_dispatch_eba_dates_value order by field_sirius_dispatch_eba_dates_value separator '|') from field_data_field_sirius_dispatch_eba_dates where entity_type = 'node' and entity_id = node.nid) as eba_dates ";
	$sql .= "from node ";
	$sql .= "left join field_data_field_sirius_dispatch_availdate on field_data_field_sirius_dispatch_availdate.entity_type = 'node' and field_data_field_sirius_dispatch_availdate.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_member_status on field_data_field_sirius_member_status.entity_type = 'node' and field_data_field_sirius_member_status.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_work_status on field_data_field_sirius_work_status.entity_type = 'node' and field_data_field_sirius_work_status.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_id on field_data_field_sirius_id.entity_type = 'node' and field_data_field_sirius_id.entity_id = node.nid ";
	$sql .= "left join field_data_field_grievance_shop on field_data_field_grievance_shop.entity_type = 'node' and field_data_field_grievance_shop.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_worker_dispstatus on field_data_field_sirius_worker_dispstatus.entity_type = 'node' and field_data_field_sirius_worker_dispstatus.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_json on field_data_field_sirius_json.entity_type = 'node' and field_data_field_sirius_json.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_dispatch_eba on field_data_field_sirius_dispatch_eba.entity_type = 'node' and field_data_field_sirius_dispatch_eba.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
	// $sql .= "left join field_data_field_sirius_dispatch_eba_dates on field_data_field_sirius_dispatch_eba_dates.entity_type = 'node' and field_data_field_sirius_dispatch_eba_dates.entity_id = node.nid ";
	$sql .= "left join taxonomy_term_data ds_term on ds_term.tid = field_sirius_worker_dispstatus_tid ";
	$sql .= "left join sirius_hours_cache on sirius_hours_cache.worker_nid = node.nid ";
	$sql .= "where node.type = 'sirius_worker' ";

	$domain_nid = sirius_domain_nid();
	if ($domain_nid) {
		$sql .= "and field_sirius_domain_target_id = :domain_nid ";
		$sql_args[':domain_nid'] = $domain_nid; 
	} else {
		$sql .= "and field_sirius_domain_target_id is null ";
	}

	$ws_tids = array();
	if ($prefs['workers']['work_status']) {
		$ws_tids = array_filter(array_values($prefs['workers']['work_status']));
	}
	if ($ws_tids) {
		$sql .= "and field_sirius_work_status_tid in (:work_status) ";
		$sql_args[':work_status'] = $ws_tids;
	}

	$dispstatus_tids = array();
	if ($prefs['workers']['dispstatus']) {
		$dispstatus_tids = array_filter(array_values($prefs['workers']['dispstatus']));
	}
	if ($dispstatus_tids) { 
		$sql .= "and field_sirius_worker_dispstatus_tid in (:dispstatus_tids) ";
		$sql_args[':dispstatus_tids'] = $dispstatus_tids;
	}

	$sql .= "group by node.nid ";
	$sql .= "order by field_sirius_dispatch_availdate_value limit $worker_max ";
	$stmt = sirius_sql_query($sql, $sql_args);
	$workers = array();
	while ($hr = $stmt->fetchAssoc()) {
		$workers[$hr['worker_nid']] = $hr;
	}
	if (count($workers) > $worker_max - 1) {
		drupal_set_message(t('Limiting to @worker_max workers only.', array('@worker_max' => $worker_max)), 'warning');
	}

	// Fetch the current dispatches for each worker
	if ($workers) { 
		$sql_args = array();
		$sql_args[':worker_nids'] = array_keys($workers);
		$sql_args[':now'] = gmdate('Y-m-d 00:00:00', strtotime("-2 days"));

		$sql = "select nid as dispatch_nid, ";
		$sql .= "field_sirius_worker_target_id as worker_nid, ";
		$sql .= "field_sirius_dispatch_status_value as dispatch_status, ";
		$sql .= "field_grievance_shop_target_id as employer_nid, ";
		$sql .= "field_sirius_datetime_value as report_date ";
		$sql .= "from node ";
		$sql .= "left join field_data_field_sirius_worker on field_data_field_sirius_worker.entity_type = 'node' and field_data_field_sirius_worker.entity_id = node.nid ";
		$sql .= "left join field_data_field_sirius_dispatch_job on field_data_field_sirius_dispatch_job.entity_type = 'node' and field_data_field_sirius_dispatch_job.entity_id = node.nid ";
		$sql .= "left join field_data_field_sirius_dispatch_status on field_data_field_sirius_dispatch_status.entity_type = 'node' and field_data_field_sirius_dispatch_status.entity_id = node.nid ";
		$sql .= "left join field_data_field_grievance_shop on field_data_field_grievance_shop.entity_type = 'node' and field_data_field_grievance_shop.entity_id = field_sirius_dispatch_job_target_id ";
		$sql .= "left join field_data_field_sirius_datetime on field_data_field_sirius_datetime.entity_type = 'node' and field_data_field_sirius_datetime.entity_id = field_sirius_dispatch_job_target_id ";
		$sql .= "where type = 'sirius_dispatch' ";
		$sql .= "and field_sirius_worker_target_id in (:worker_nids) ";
		$sql .= "and field_sirius_dispatch_status_value in ('notified', 'notification failed', 'accepted', 'accepted_secondary') ";
		$sql .= "and not (field_sirius_dispatch_status_value in ('notified', 'notification failed') and field_sirius_datetime_value < :now) ";
		$sql .= "limit " . 10 * $worker_max;
		$stmt = db_query($sql, $sql_args);
		while ($hr = $stmt->fetchAssoc()) {
			$workers[$hr['worker_nid']]['dispatches'][] = $hr;
		}
	}

	// Set the level for each worker
	if ($workers) { 
		$ms_level_filter = array();
		if ($prefs['workers']['ms_level']) {
			$ms_level_filter = array_filter(array_values($prefs['workers']['ms_level']));
		}
		foreach ($workers as $worker_nid => $hr) {
	  	$json = json_decode($hr['json_value'], TRUE);
	  	$ms_tids = explode(',', $hr['ms_tid']);
	  	$level_titles = array();
	  	$filter_found = FALSE;
	  	$ms_names = array();
	  	$level_weight = 0;
	  	foreach ($ms_tids as $ms_tid) {
	  		$ms_level_key = $ms_tid;
	  		if ($ms_name) { $ms_name .= '<br />'; }
	  		$ms_name = $ms_lookup[$ms_tid];
	  		$level_code = $json['training']['levels'][$ms_tid]['code'];
	  		if ($level_code) {
	  			$ms_level_key .= "|$level_code";
	  			$level = $all_levels[$ms_tid][$level_code];
	  			if ($level) {
	  				$ms_name .= ' ' . $level['title'];
	  				$level_weight = max($level_weight, $level['weight']);
	  			}
	  		}
	  		if (in_array($ms_level_key, $ms_level_filter)) { $filter_found = TRUE; }
	  		$ms_names[] = $ms_name;
	  	}
			$workers[$worker_nid]['ms_name'] = join('<br>', $ms_names);
			$workers[$worker_nid]['level_weight'] = $level_weight;

			if ($ms_level_filter && !$filter_found) { unset($workers[$worker_nid]); }
		}
	}

	// Sort
	if ($prefs['workers']['sort'][0]) {
	  uksort($workers, function($a, $b) use ($workers, $prefs) {
	  	foreach ($prefs['workers']['sort'] as $sort) {
	  		$key = $sort['key'];
	  		$direction = $sort['direction'];
	  		if (!$key || !$direction) { continue; }

	  		if ($direction == 'asc') {
		  		if ($workers[$a][$key] > $workers[$b][$key]) { return 1; }
		  		if ($workers[$a][$key] < $workers[$b][$key]) { return -1; }
	  		}
	  		if ($direction == 'desc') {
		  		if ($workers[$a][$key] > $workers[$b][$key]) { return -1; }
		  		if ($workers[$a][$key] < $workers[$b][$key]) { return 1; }
	  		}
	  	}
	  	return 0;
	  });
	}

	return $workers; 
}