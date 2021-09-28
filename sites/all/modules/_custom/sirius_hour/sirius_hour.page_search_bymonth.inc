<?php

/*
| payperiod_nid   | int(11)       | YES  | MUL | NULL    |       |
| year            | int(11)       | YES  | MUL | NULL    |       |
| month           | int(11)       | YES  |     | NULL    |       |
| day             | int(11)       | YES  |     | NULL    |       |
| worker_nid      | int(11)       | YES  | MUL | NULL    |       |
| employer_nid    | int(11)       | YES  | MUL | NULL    |       |
| hours_type_tid  | int(11)       | YES  |     | NULL    |       |
| department_tid  | int(11)       | YES  |     | NULL    |       |
| total           | decimal(10,2) | YES  |     | NULL    |       |
| worker_name     | varchar(255)  | YES  |     | NULL    |       |
| employer_name   | varchar(255)  | YES  |     | NULL    |       |
| hours_type_name | varchar(255)  | YES  |     | NULL    |       |
| department_name | varchar(255)  | YES  |     | NULL    |       |
| hours           | decimal(10,4) | YES  |     | NULL    |       |
*/

function sirius_hour_page_search_month() {
	return drupal_get_form('sirius_hour_page_search_month_form');
}

function sirius_hour_page_search_month_form($form, &$form_state) {
	$form['#tree'] = TRUE;

	$start_year = date('Y') - 5;
	$end_year = date('Y') + 5;
	for ($year = $start_year; $year <= $end_year; ++$year) { $years[$year] = $year; }
	for ($month = 1; $month <= 12; ++$month) { $months[$month] = $month; }
	for ($day = 1; $day <= 31; ++$day) { $days[$day] = $day; }

	$this_year = date('Y');
	$this_month = date('n');
	$this_month--;
	if ($this_month < 1) { $this_month = 12; $this_year--; }

	$employer_lookup = sirius_employer_list();
	$ws_lookup = sirius_taxonomy_load('sirius_work_status', 'tid', 'name');
	$ms_lookup = sirius_taxonomy_load('sirius_member_status', 'tid', 'name');
	$industry_lookup = sirius_taxonomy_load('sirius_industry', 'tid', 'name');

	$form['start_year'] = array(
		'#type' => 'select',
		'#title' => 'Start Year',
		'#options' => $years,
		'#default_value' => $this_year,
		'#required' => TRUE,
	);
	
	$form['start_month'] = array(
		'#type' => 'select',
		'#title' => 'Start Month',
		'#options' => $months,
		'#default_value' => $this_month,
		'#required' => TRUE,
	);

	$form['end_year'] = array(
		'#type' => 'select',
		'#title' => 'End Year',
		'#options' => $years,
		'#default_value' => $this_year,
		'#required' => TRUE,
	);
	
	$form['end_month'] = array(
		'#type' => 'select',
		'#title' => 'End Month',
		'#options' => $months,
		'#default_value' => $this_month,
		'#required' => TRUE,
	);

	$form['min_hours'] = array(
		'#type' => 'textfield',
		'#title' => 'Minimum Hours Per Month',
		'#size' => 4,
	);

	$form['employer_nid'] = array(
		'#type' => 'select',
		'#title' => 'Employer',
		'#options' => array(NULL => t('-- Select --')) + $employer_lookup,
	);

	$form['industry_tid'] = array(
		'#type' => 'select',
		'#title' => 'Industry',
		'#options' => array(NULL => t('-- Select --')) + $industry_lookup,
	);

	$form['ws_tid'] = array(
		'#type' => 'select',
		'#title' => 'Work Status',
		'#options' => array(NULL => t('-- Select --')) + $ws_lookup,
	);

	$form['ms_tid'] = array(
		'#type' => 'select',
		'#title' => 'Member Status',
		'#options' => array(NULL => t('-- Select --')) + $ms_lookup,
	);

	$form['output'] = array(
		'#title' => t('Output'),
		'#type' => 'select',
		'#options' => array(
			'screen' => 'Screen',
			'csv' => 'CSV',
		),
		'#default_value' => 'screen',
		'#required' => TRUE,
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	if ($form_state['results']) { $form['results'] = array('#markup' => $form_state['results']); }

	return $form;
}

function sirius_hour_page_search_month_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;

	$ws_lookup = sirius_taxonomy_load('sirius_work_status', 'tid', 'name');
	$ms_lookup = sirius_taxonomy_load('sirius_member_status', 'tid', 'name');

	$output = $form_state['values']['output'];

	$sql_args = array();
	$sql_args[':domain_nid'] = sirius_domain_nid();

	$sql_args[':start_year'] = $form_state['values']['start_year'];
	$sql_args[':end_year'] = $form_state['values']['end_year'];
	$sql_args[':start_month'] = $form_state['values']['start_month'];
	$sql_args[':end_month'] = $form_state['values']['end_month'];

	$industry_tid = $form_state['values']['industry_tid'];

	$sql = "select worker_nid, ";
	$sql .= "worker_node.title as worker_title, ";
	$sql .= "field_sirius_work_status_tid, ";
	$sql .= "group_concat(distinct field_sirius_member_status_tid order by field_sirius_member_status_tid separator ',') as ms_tids, ";
	$sql .= "year, month, ";
	$sql .= "sum(hours) / count(*) as s from sirius_hours_cache ";
	$sql .= "left join node worker_node on worker_node.nid = worker_nid ";
	$sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = payperiod_nid ";
	$sql .= "left join field_data_field_sirius_work_status on field_data_field_sirius_work_status.entity_type = 'node' and field_data_field_sirius_work_status.entity_id = worker_node.nid ";
	$sql .= "left join field_data_field_sirius_member_status on field_data_field_sirius_member_status.entity_type = 'node' and field_data_field_sirius_member_status.entity_id = worker_node.nid ";
	if ($industry_tid) {
		$sql .= "left join field_data_field_sirius_industry on field_data_field_sirius_industry.entity_type = 'node' and field_data_field_sirius_industry.entity_id = employer_nid ";
	}
	$sql .= "where field_sirius_domain_target_id = :domain_nid ";
	$sql .= "and year >= :start_year ";
	$sql .= "and year <= :end_year ";
	$sql .= "and month >= :start_month ";
	$sql .= "and month <= :end_month ";
	if ($form_state['values']['employer_nid']) {
		$sql .= "and employer_nid = :employer_nid ";
		$sql_args[':employer_nid'] = $form_state['values']['employer_nid'];
	}
	if ($form_state['values']['ms_tid']) {
		$sql .= "and field_sirius_member_status_tid = :ms_tid ";
		$sql_args[':ms_tid'] = $form_state['values']['ms_tid'];
	}
	if ($form_state['values']['ws_tid']) {
		$sql .= "and field_sirius_work_status_tid = :ws_tid ";
		$sql_args[':ws_tid'] = $form_state['values']['ws_tid'];
	}
	if ($industry_tid) {
		$sql .= "and field_sirius_industry_tid = :industry_tid ";
		$sql_args[':industry_tid'] = $industry_tid; 
	}
	$sql .= "group by worker_nid, year, month ";
	$sql .= "order by worker_title, year, month ";
	$sql .= "limit 10000 ";

	$stmt = sirius_sql_query($sql, $sql_args);
	$rows = array();
	$min_hours = (float) $form_state['values']['min_hours'];
	while ($hr = $stmt->fetchAssoc()) {
		$hours = $hr['s'];
		if ($min_hours && ($min_hours > $hours)) { continue; }

		$tools = '';
		$tools .= l('Worker', "node/$hr[worker_nid]/sirius_worker_hours", array('attributes' => array('class' => array('tool-button'))));

		$ms_tids = explode(',', $hr['ms_tids']);
		$ms_names = array();
		if ($ms_tids) {
			foreach ($ms_tids as $ms_tid) { $ms_names[] = $ms_lookup[$ms_tid]; }
		}
		$ms_rendered = join(',', $ms_names);

		$row = array();
		$row[] = $hr['worker_title'];
		$row[] = $ws_lookup[$hr['field_sirius_work_status_tid']];
		$row[] = $ms_rendered;
		$row[] = $hr['year'];
		$row[] = $hr['month'];
		$row[] = $hours;
		if ($output == 'screen') { $row[] = $tools; }
		$rows[] = $row;
	}

	$header = array();
	$header[] = t('Worker Name');
	$header[] = t('Work Status');
	$header[] = t('Member Status');
	$header[] = t('Year');
	$header[] = t('Month');
	$header[] = t('Total Hours');
	if ($output == 'screen') {	$header[] = t('Tools'); }

	if ($output == 'csv') {
    $fp = fopen('php://output', 'w');
    $filename = 'monthly_hours.csv';
    drupal_add_http_header('Content-Type', 'text/csv');
    drupal_add_http_header('Content-Disposition', 'attachment;filename=' . $filename);
    fputcsv($fp, $header);

		foreach ($rows as $row) { fputcsv($fp, $row); }
		fclose($fp);
		exit();
	}
	
	$html = '<hr>';
	$html .= '<h2>' . t('Results') . '</h2>';
	if (!$rows) {
		$html .= '<p><em>' . t('No hours entries found matching the search criteria.') . '</em></p>';
	} else {
		$html .= theme_table(array('header' => $header, 'rows' => $rows, 'attributes' => array()));
	}

	$form_state['results'] = $html;
}