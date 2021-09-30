<?php

function sirius_dispatch_jwa_prefs() {
	global $user;
	$user_full = user_load($user->uid);
	$json = json_decode($user_full->field_sirius_json['und'][0]['value'], TRUE);
	return $json['sirius']['dispatch']['jwa']['prefs'];
}

function sirius_dispatch_jwa_hours_clause($prefs) {
  $sql = "";
  $delta = 0;
  $date_range = $prefs['date_range'];
  $date_range_days = $prefs['date_range_days'];
  $date_range_months = $prefs['date_range_months'];
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
  foreach ($prefs['hour_type'] as $type) {
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

  return $sql;
}

function sirius_dispatch_jwa_hours_render($prefs, $hr) {
	$hours = '';
	$is_first = TRUE;
	$delta = 0;
	foreach ($prefs['hour_type'] as $type) { 
		if ($is_first) { $is_first = FALSE; } else { $hours .= '&nbsp;/&nbsp;'; }
		$hours .= round($hr["hours_$delta"], 2); 
		++$delta;
	}
	return $hours;
}

function sirius_dispatch_page_jwa_assign() {
	return drupal_get_form('sirius_dispatch_page_jwa_assign_form');
}

function sirius_dispatch_page_jwa_assign_form($form, &$form_state) {
	$prefs = sirius_dispatch_jwa_prefs();
	if (!$prefs) {
		drupal_set_message(t('Please set your Job / Worker Assign Preferences.'), 'warning');
		return array();
	}

	// Get our lookup table of employers
	$employers = sirius_employer_list();

	// Fetch the jobs
	$query = new EntityFieldQuery();
	$query->entityCondition('entity_type', 'node');
	$query->entityCondition('bundle', 'sirius_dispatch_job');
	$query->fieldCondition('field_sirius_dispatch_job_status', 'value', array('Unfilled', 'Running')); 
	if ($prefs['job_type']) { $query->fieldCondition('field_sirius_dispatch_job_type', 'tid', array_values($prefs['job_type']), 'in'); }
	$query->propertyOrderBy('nid', 'desc');
	$stmt = $query->execute();
	if ($stmt['node']) { 
		$job_nids = array_keys($stmt['node']);
	} else {
		drupal_set_message(t('There are no jobs matching the criteria set in your preferences.'), 'warning');
		$job_nids = array();
	}
	$job_max = variable_get('sirius_dispatch_jwa_job_max', 25);
	$job_count = count($job_nids);
	if ($job_count > $job_max) {
		drupal_set_message(t('There @job_count jobs with status "Pending", "Unfilled", or "Running" that  matching your criteria. Using the first @job_max jobs only.', array('@job_max' => $job_max, '@job_count' => $job_count)), 'warning');
		$job_nids = array_slice($job_nids, 0, $job_max);
	}
	$job_nodes = node_load_multiple($job_nids);

	// Fetch the roster for each job. SQL for speeed
	$roster = array();
	if ($job_nids) {
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
	}

	// Fetch the workers. SQL for speed
	$args = array();
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
	$sql .= "(select group_concat(substr(field_sirius_dispatch_eba_dates_value, 6, 5) order by field_sirius_dispatch_eba_dates_value separator '<br>') from field_data_field_sirius_dispatch_eba_dates where entity_type = 'node' and entity_id = node.nid) as eba_dates ";
	$sql .= "from node ";
	$sql .= "left join field_data_field_sirius_dispatch_availdate on field_data_field_sirius_dispatch_availdate.entity_type = 'node' and field_data_field_sirius_dispatch_availdate.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_member_status on field_data_field_sirius_member_status.entity_type = 'node' and field_data_field_sirius_member_status.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_work_status on field_data_field_sirius_work_status.entity_type = 'node' and field_data_field_sirius_work_status.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_id on field_data_field_sirius_id.entity_type = 'node' and field_data_field_sirius_id.entity_id = node.nid ";
	$sql .= "left join field_data_field_grievance_shop on field_data_field_grievance_shop.entity_type = 'node' and field_data_field_grievance_shop.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_worker_dispstatus on field_data_field_sirius_worker_dispstatus.entity_type = 'node' and field_data_field_sirius_worker_dispstatus.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_json on field_data_field_sirius_json.entity_type = 'node' and field_data_field_sirius_json.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_dispatch_eba on field_data_field_sirius_dispatch_eba.entity_type = 'node' and field_data_field_sirius_dispatch_eba.entity_id = node.nid ";
	// $sql .= "left join field_data_field_sirius_dispatch_eba_dates on field_data_field_sirius_dispatch_eba_dates.entity_type = 'node' and field_data_field_sirius_dispatch_eba_dates.entity_id = node.nid ";
	$sql .= "left join taxonomy_term_data ds_term on ds_term.tid = field_sirius_worker_dispstatus_tid ";
	$sql .= "left join sirius_hours_cache on sirius_hours_cache.worker_nid = node.nid ";
	$sql .= "where node.type = 'sirius_worker' ";
	if ($prefs['member_status']) {
		$sql .= "and field_sirius_member_status_tid in (:member_status) ";
		$args[':member_status'] = array_values($prefs['member_status']);
	}
	if ($prefs['work_status']) {
		$sql .= "and field_sirius_work_status_tid in (:work_status) ";
		$args[':work_status'] = array_values($prefs['work_status']);
	}
	$sql .= "group by node.nid ";
	$sql .= "order by field_sirius_dispatch_availdate_value limit $worker_max ";
	$stmt = db_query($sql, $args);
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

	// Build our output
  drupal_add_library('system', 'ui.accordion');
  drupal_add_js('jQuery(document).ready(function () { jQuery("#sirius_accordion").accordion({active: false, collapsible: true, autoehight: true, heightStyle: "content"}); });', 'inline');
	drupal_add_library('system', 'ui.tooltip');
	drupal_add_js(drupal_get_path('module', 'sirius_dispatch') . '/js/sirius_dispatch.jwa.js', 'file');

  //
  // RIGHT COLUMN
  //

  //$form['td_2_open'] = array('#markup' => '<td style="vertical-align: top; border: none; background: none; margin-right: 20;">');

	$html = '<div id="sirius_accordion">';

	$skill_lookup = sirius_taxonomy_load('sirius_skill', 'tid', 'name');

	foreach ($job_nodes as $job_nid => $job_node) {
		$count = $job_node->field_sirius_count['und'][0]['value']; if (!$count) { $count = 0; }
		$notified = $job_node->field_sirius_count_notified['und'][0]['value']; if (!$notified) { $notified = 0; }
		$yes = $job_node->field_sirius_count_yes['und'][0]['value']; if (!$yes) { $yes = 0; }
		$no = $job_node->field_sirius_count_no['und'][0]['value']; if (!$no) { $no = 0; }
		$tools = l(t('Manage'), "node/$job_nid/sirius_dispatch_job_manage", array('attributes' => array('target' => '_blank', 'class' => array('tool-button'))));

		$title = '<strong>' . $employers[$job_node->field_grievance_shop['und'][0]['target_id']] . '</strong><br />';
		$title .= sirius_date_date($job_node, 'field_sirius_datetime') . ', ';
		$title .= $job_node->field_sirius_dispatch_job_status['und'][0]['value'];


		$facility_title = sirius_node_title($job_node->field_sirius_dispatch_facility['und'][0]['target_id']);
		$hall_title = sirius_node_title($job_node->field_sirius_dispatch_hall['und'][0]['target_id']);
		$notes_title = $job_node->field_sirius_address_notes['und'][0]['value'];

		$skill_titles = array();
		if ($job_node->field_sirius_skill['und']) {
			foreach ($job_node->field_sirius_skill['und'] as $delta => $record) {
				$skill_titles[] = $skill_lookup[$record['tid']];
			}
		}
		if ($facility_title || $hall_title || $notes_title || $skill_titles) {
			if ($facility_title) { $title .= " / $facility_title"; }
			if ($hall_title) { $title .= " $hall_title"; }
			if ($notes_title) { $title .= " $notes_title"; }
			if ($skill_titles) { $title .= ' / ' . join(' / ', $skill_titles); }
		}

		$title .= '<br />';
		$title .= "Need: <strong>$count</strong> / Notified: <strong>$notified</strong> / Yes: <strong>$yes</strong> / No: $no";

		$html .= '<div class="sirius_accordion_label">' . $title . '</div><div class="sirius_accordion_body">';

		$html .= l(t("Select"), "<front>", array('attributes' => array('target' => '_blank', 'data-job-nid' => $job_nid, 'class' => array('tool-button', 'sirius_job_nid_set'))));
		$html .= ' ';
		$html .= l(t("Manage"), "node/$job_nid/sirius_dispatch_job_manage", array('attributes' => array('target' => '_blank', 'class' => array('tool-button'))));

		if ($roster[$job_nid]) {
			$header = array(t('Worker'), t('Status'), t('Tools'));
			$rows = array();
			foreach ($roster[$job_nid] as $hr) {
				$tools = l(t('Dispatch'), "node/$hr[dispatch_nid]/sirius_dispatch_manage", array('attributes' => array('target' => '_blank', 'class' => array('tool-button'))));
				$tools .= ' ';
				$tools .= l(t('Worker'), "node/$hr[worker_nid]", array('attributes' => array('target' => '_blank', 'class' => array('tool-button'))));
				$row = array();
				$row[] = $hr['worker_name'] . '<br />' . $hr['sirius_id'];
				$row[] = $hr['dispatch_status'];
				$row[] = $tools;
				$rows[] = $row;
			}
			$html .= theme_table(array('rows' => $rows, 'header' => $header, 'attributes' => array()));
		} else {
			$html .= '<br /><br /><em>' . t('No workers dispatched.') . '</em>';
		}
		$html .= '</div>';
	}

	$html .= '</div>';

	$form['td_2_body'] = array('#markup' => $html);

	if ($job_nids) {
	  $form['submit'] = array(
	  	'#type' => 'submit',
	  	'#value' => t('Dispatch the selected workers to the selected job'),
	  );
	}

	//
  // LEFT COLUMN
  //

  $header = array(
  	t('Worker'),
  	t('ID'),
  	t('Seniority'),
  	t('Member Status'),
  	t('Dispatch Status'),
  	t('EBA'),
  	t('Employer'),
  	t('Hours'),
  	t('Worker'),
  );

  $rows = array();
  $all_levels = sirius_training_levels_fetch_all();
  $ms_terms = sirius_taxonomy_load('sirius_member_status', 'tid', 'full');

	foreach ($workers as $worker_nid => $hr) {
		$ms_name = '';

  	$json = json_decode($hr['json_value'], TRUE);
  	$ms_tids = explode(',', $hr['ms_tid']);
  	$level_titles = array();
  	foreach ($ms_tids as $ms_tid) {
  		if ($ms_name) { $ms_name .= ' / '; }
  		$ms_name .= $ms_terms[$ms_tid]->name;
  		$level_code = $json['training']['levels'][$ms_tid]['code'];
  		if ($level_code) {
  			$level = $all_levels[$ms_tid][$level_code];
  			if ($level) {
  				$ms_name .= ' '; 
  				$ms_name .= $level['title'];
  			}
  		}
  	}

  	$employer_name = $employers[$hr['employer_nid']];
  	if (!$employer_name) { $employer_name = 'AVAILABLE'; }
 		$tools = l(t('Worker'), "node/$worker_nid", array('attributes' => array('target' => '_blank', 'class' => array('tool-button'))));
 		$tools .= ' ';
 		$tools .= l(t('Hours'), "node/$worker_nid/sirius_worker_hours", array('attributes' => array('target' => '_blank', 'class' => array('tool-button'))));

 		if ($hr['dispatches']) {
 			foreach ($hr['dispatches'] as $dispatch) {
		 		$tools .= ' ';
		 		$tools .= l($dispatch['dispatch_status'], "node/$dispatch[dispatch_nid]/sirius_dispatch_manage", array('attributes' => array('target' => '_blank', 'class' => array('tool-button')))) . ' ';
 			}
 		}

 		$hours = sirius_dispatch_jwa_hours_render($prefs, $hr);

		if ($hr['eba_name'] == 'Yes') { $eba = $hr['eba_dates']; } else { $eba = ''; }

  	$dispatch_status_render = $hr['dispatch_status_name'];
  	$schedule = sirius_worker_schedule($worker_nid);
  	if ($schedule) { $dispatch_status_render .= sirius_worker_schedule_render($schedule, 'tiny'); }

   	$row = array();
		$row[] = $hr['worker_name'];
		$row[] = $hr['sirius_id'];
		$row[] = $hr['availdate'];
		$row[] = $ms_name;
		$row[] = $dispatch_status_render;
		$row[] = $eba;
  	$row[] = $employer_name;
  	$row[] = $hours;
		$row[] = $tools;

		$rows[$worker_nid] = $row;
  }

  $form['worker_nid'] = array(
  	'#type' => 'tableselect',
  	'#header' => $header,
  	'#options' => $rows,
  );

  $form['job_nid'] = array('#required' => TRUE, '#type' => 'hidden', '#attributes' => array('id' => 'sirius_job_nid'));

	return $form;
}



function sirius_dispatch_page_jwa_assign_form_submit($form, &$form_state) {
	$job_nid = $form_state['values']['job_nid'];
	$worker_nids = array_filter(array_values($form_state['values']['worker_nid']));

	if (!$job_nid) { 
		drupal_set_message(t('Please select a job.'), 'warning');
		$form_state['rebuild'] = TRUE;
		return; 
	}
	$job_node = node_load($job_nid);

	if (!$worker_nids) {
		drupal_set_message(t('Please select at least one worker.'), 'warning');
		return; 
	}

	$success = 0;
	$failure = 0;
	$count = 0;
	foreach ($worker_nids as $worker_nid) {
		++$count;
		$sql = "select title from node where nid = :worker_nid";
		$stmt = db_query($sql, array(':worker_nid' => $worker_nid));
		$hr = $stmt->fetchAssoc();
		$worker_name = $hr['title'];

		$dispatch_node = sirius_dispatch_get($worker_nid, $job_nid);
		if (!$dispatch_node) {
			$result = sirius_dispatch_actions_create($job_node, $worker_nid, 'Yes');
			if (!$result['success']) { 
				drupal_set_message(t('Failed to create dispatch for @worker_name to @job_name: @msg', array('@worker_name' => $worker_name, '@job_name' => $job_node->title, '@msg' => $result['msg'])), 'warning');
				++$failure;
				continue;
			}
			$dispatch_node = $result['dispatch_node'];
		}

		$result = sirius_dispatch_notify($dispatch_node);
		if (!$result['success']) { 
			drupal_set_message(t('Notification failed for @worker_name to @job_name: @msg', array('@worker_name' => $worker_name, '@job_name' => $job_node->title, '@msg' => $result['msg'])), 'warning');
			$result = sirius_dispatch_job_get_dispatchable_workers_explain($job_node, $worker_nid, array('cbn' => 'Yes', 'ignore_redispatch' => TRUE));
			if (!$result['success']) {
				drupal_set_message($result['msg'], 'warning');
			}
			++$failure;
			continue;
		}

		++$success;
		drupal_set_message(t('Notified @worker_name to @job_name', array('@worker_name' => $worker_name, '@job_name' => $job_node->title, '@msg' => $result['msg'])));
	}

	drupal_set_message(t('Assigned @count workers, with @success notifications and @failure failures.', array('@count' => $count, '@success' => $success, '@failure' => $failure)));
}