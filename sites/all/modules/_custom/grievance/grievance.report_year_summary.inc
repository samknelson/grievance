<?php

function grievance_report_year_summary() {
	return drupal_get_form('grievance_report_year_summary_form');
}

function grievance_report_year_summary_form($form, &$form_state) {

	$y = date('Y');
	$options = array(NULL => '[select]');
	for ($i=-10; $i<=10; ++$i) {
		$options[$y+$i] = $y+$i;
	}
  $form['year'] = array(
    '#title' => t('Year'),
    '#type' => 'select',
    '#required' => TRUE,
    '#options' => $options,
	);

	$options = array(
		1 => 'January',
		2 => 'February',
		3 => 'March',
		4 => 'April',
		5 => 'May',
		6 => 'June',
		7 => 'July',
		8 => 'August',
		9 => 'September',
		10 => 'October',
		11 => 'November',
		12 => 'December',
	);

  $form['start_month'] = array(
    '#title' => t('Start Month'),
    '#type' => 'select',
    '#required' => TRUE,
    '#options' => $options,
	);

  $form['end_month'] = array(
    '#title' => t('End Month'),
    '#type' => 'select',
    '#required' => TRUE,
    '#options' => $options,
	);

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Generate Report',
  );

  if ($form_state['results']) {
  	$form['results'] = array(
  		'#type' => 'markup',
  		'#markup' => $form_state['results'],
  	);
  }

  return $form;
}

function grievance_report_year_summary_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;
	$year = $form_state['values']['year'];

  $status_vocabulary = taxonomy_vocabulary_machine_name_load('grievance_status');
  $status_terms = taxonomy_get_tree($status_vocabulary->vid);
  $open_status_tids = array();
  $closed_status_tids = array();
  foreach ($status_terms as $status_term) {
  	$status_term_full = taxonomy_term_load($status_term->tid);
  	if ($status_term_full->field_grievance_open['und'][0]['value'] == 1) {
  		$open_status_tids[] = $status_term_full->tid;
  	} else {
  		$closed_status_tids[] = $status_term_full->tid;
  	}
  }

  $insert_log_tid = sirius_get_special_tid('grievance_log_types|insert');
  $update_log_tid = sirius_get_special_tid('grievance_log_types|update_with_status_change');

  // A grievance might be opened and closed multiple times, so we need to pull them all and sort in memory; simply doing a "group by ... distinct" in the SQL doesn't work.

	$start_month = $form_state['values']['start_month'];
	$end_month = $form_state['values']['end_month'];

  //
  // Count the grievances opened, by month. 
  //

  $sql = "select ";
	$sql .= "extract(month from field_data_field_grievance_status_date.field_grievance_status_date_value) as m, ";
	$sql .= "field_data_field_grievance.field_grievance_target_id as grievance_nid ";
	$sql .= "from node ";
	$sql .= "join field_data_field_grievance_log_type on (field_data_field_grievance_log_type.entity_id = node.nid and field_data_field_grievance_log_type.entity_type = 'node') ";
	$sql .= "join field_data_field_grievance_status_date on (field_data_field_grievance_status_date.entity_id = node.nid and field_data_field_grievance_status_date.entity_type = 'node') ";
	$sql .= "/* join field_data_field_grievance_status on (field_data_field_grievance_status.entity_id = node.nid and field_data_field_grievance_status.entity_type = 'node') */ ";
	$sql .= "join field_data_field_grievance on (field_data_field_grievance.entity_id = node.nid and field_data_field_grievance.entity_type = 'node') ";
	$sql .= "where node.type = 'grievance_log' ";
	$sql .= "and field_data_field_grievance_log_type.field_grievance_log_type_tid = :insert_log_tid ";
	$sql .= "and extract(year from field_data_field_grievance_status_date.field_grievance_status_date_value) = $year ";
	$sql .= "order by field_data_field_grievance_status_date.field_grievance_status_date_value ";

	$stmt = db_query($sql, array(':insert_log_tid' => $insert_log_tid));
	$opened_count = array();
	$opened_count_total = 0;
	$already_seen = array();
	while ($hr = $stmt->fetchAssoc()) {
		$month = $hr['m'];
		if ($start_month && $month < $start_month) { continue; }
		if ($end_month && $month > $end_month) { continue; }

		if ($already_seen[$hr['grievance_nid']]) { continue; }
		$already_seen[$hr['grievance_nid']] = TRUE;
		$opened_count[$hr['m']]++;
		$opened_count_total++;
	}

  //
  // Count the grievances opened, by month. 
  //

  $sql = "select ";
	$sql .= "extract(month from field_data_field_grievance_status_date.field_grievance_status_date_value) as m, ";
	$sql .= "field_data_field_grievance.field_grievance_target_id as grievance_nid ";
	$sql .= "from node ";
	$sql .= "join field_data_field_grievance_log_type on (field_data_field_grievance_log_type.entity_id = node.nid and field_data_field_grievance_log_type.entity_type = 'node') ";
	$sql .= "join field_data_field_grievance_status_date on (field_data_field_grievance_status_date.entity_id = node.nid and field_data_field_grievance_status_date.entity_type = 'node') ";
	$sql .= "join field_data_field_grievance_status on (field_data_field_grievance_status.entity_id = node.nid and field_data_field_grievance_status.entity_type = 'node') ";
	$sql .= "join field_data_field_grievance on (field_data_field_grievance.entity_id = node.nid and field_data_field_grievance.entity_type = 'node') ";
	$sql .= "where node.type = 'grievance_log' ";
	$sql .= "and field_data_field_grievance_log_type.field_grievance_log_type_tid = :update_log_tid ";
	$sql .= "and field_data_field_grievance_status.field_grievance_status_tid in (:closed_status_tids) ";
	$sql .= "and extract(year from field_data_field_grievance_status_date.field_grievance_status_date_value) = $year ";
	$sql .= "order by field_data_field_grievance_status_date.field_grievance_status_date_value ";

	$stmt = db_query($sql, array(':update_log_tid' => $update_log_tid, ':closed_status_tids' => $closed_status_tids));
	$closed_count = array();
	$already_seen = array();
	$closed_count_total = 0;
	while ($hr = $stmt->fetchAssoc()) {
		$month = $hr['m'];
		if ($start_month && $month < $start_month) { continue; }
		if ($end_month && $month > $end_month) { continue; }

		if ($already_seen[$hr['grievance_nid']]) { continue; }
		$already_seen[$hr['grievance_nid']] = TRUE;
		$closed_count[$month]++;
		$closed_count_total++;
	}


	$rows = array();
	$months = array(
		1 => 'January', 
		2 => 'February', 
		3 => 'March', 
		4 => 'April', 
		5 => 'May', 
		6 => 'June', 
		7 => 'July', 
		8 => 'August', 
		9 => 'September', 
		10 => 'October', 
		11 => 'November', 
		12 => 'December', 
	);

	foreach ($months as $month => $month_name) {
		if ($start_month && $month < $start_month) { continue; }
		if ($end_month && $month > $end_month) { continue; }

		$row = array();
		$row[] = $month_name;
		if (!$opened_count[$month]) { $opened_count[$month] = 0; }
		if (!$closed_count[$month]) { $closed_count[$month] = 0; }
		$row[] = $opened_count[$month];
		$row[] = $closed_count[$month];

		$rows[] = $row;
	}

	$row = array();
	$row[] = '<strong>' . t('Total') . '</strong>';
	$row[] = '<strong>' . number_format($opened_count_total, 0) . '</strong>';
	$row[] = '<strong>' . number_format($closed_count_total, 0) . '</strong>';
	$rows[] = $row;

	$header = array(
		'Month',
		'Opened',
		'Closed'
	);

	$html = '<h2>' . t('Opened / Closed Report: ') . $year . '</h2>';
	$html .= theme_table(array('header' => $header, 'rows' => $rows, 'attributes' => array()));

	$form_state['results'] = $html;

	return $html;
}