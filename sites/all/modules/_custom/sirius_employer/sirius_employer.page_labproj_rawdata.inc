<?php

function sirius_employer_page_labproj_rawdata() {
	return drupal_get_form('sirius_employer_page_labproj_rawdata_form');
}

function sirius_employer_page_labproj_rawdata_form($form, &$form_state) {
	drupal_add_css(drupal_get_path('module', 'sirius_employer') . '/css/labproj.css', 'file');
	$form['#tree'] = TRUE;

	$start_year = date('Y') - 5;
	$end_year = date('Y') + 2;
	for ($year = $start_year; $year <= $end_year; ++$year) { $years[$year] = $year; }
	for ($month = 1; $month <= 12; ++$month) { $months[$month] = $month; }
	for ($day = 1; $day <= 31; ++$day) { $days[$day] = $day; }

	$default_year = date('Y');
	$default_month = date('n');
	++$default_month;
	if ($default_month > 12) { $default_month = 1; ++$default_year; }

	$form['open'] = array('#markup' => '<div class="sirius_noprint">');

	$form['start_year'] = array(
		'#title' => t('Start Year'),
		'#type' => 'select',
		'#options' => $years,
		'#default_value' => $default_year,
		'#required' => TRUE,
	);

	$form['start_month'] = array(
		'#title' => t('Start Month'),
		'#type' => 'select',
		'#options' => $months,
		'#default_value' => $default_month,
		'#required' => TRUE,
	);

	$form['end_year'] = array(
		'#title' => t('End Year'),
		'#type' => 'select',
		'#options' => $years,
		'#default_value' => $default_year,
		'#required' => TRUE,
	);

	$form['end_month'] = array(
		'#title' => t('End Month'),
		'#type' => 'select',
		'#options' => $months,
		'#default_value' => $default_month,
		'#required' => TRUE,
	);

	$form['output'] = array(
		'#title' => t('Output'),
		'#type' => 'select',
		'#options' => array(
			'screen' => 'Screen',
			'csv' => 'CSV',
		),
		'#required' => TRUE,
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	$form['close'] = array('#markup' => '</div>');

	if ($form_state['results']) {
		$form['results'] = array('#markup' => $form_state['results']);
	}

	return $form;
}

function sirius_employer_page_labproj_rawdata_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;
	$start_month = $form_state['values']['start_month'];
	$start_year = $form_state['values']['start_year'];
	$end_month = $form_state['values']['end_month'];
	$end_year = $form_state['values']['end_year'];
	$granularity = $form_state['values']['granularity'];
	$detail = $form_state['values']['detail'];
	$output = $form_state['values']['output'];
	$employers = sirius_employer_list();
	$status_options = sirius_employer_labproj_status_options();

	$group_nids = array();

	$year = $start_year;
	$month = $start_month;

	$data = array();
	while ($year < $end_year || ($year == $end_year && $month <= $end_month)) {
		$labproj_nids = sirius_employer_labproj_list(array('yearmonth' => "$year/$month"));
		$labproj_nodes = node_load_multiple($labproj_nids);

		foreach ($labproj_nodes as $labproj_nid => $labproj_node) {
			$employer_node = sirius_log_handler($labproj_node, 'grievance_shop');
			$employer_nid = $employer_node->nid;
			$labproj_json = sirius_json_get($labproj_node);
			if (!$labproj_json['days']) { continue; }

			foreach ($labproj_json['days'] as $day => $day_record) {
				if (!$day_record['groups']) { continue; }
				foreach ($day_record['groups'] as $group_nid => $value) {
					if (!$value) { continue; }
					$key = "$year-$month-$day-$employer_nid-$group_nid";
					$data[$key] = array(
						'year' => $year,
						'month' => $month,
						'day' => $day,
						'employer_nid' => $employer_nid,
						'group_nid' => $group_nid,
						'count' => $value,
					);
				}
			}
		}

		$month++;
		if ($month > 12) { $month = 1; $year++; }
	}

	$group_names = array();
	$employer_names = array();
	foreach ($data as $key => $record) {
		$group_nid = $record['group_nid'];
		if (!$group_names[$group_nid]) { $group_names[$group_nid] = sirius_node_title($group_nid); }
		$data[$key]['group_name'] = $group_names[$group_nid];

		$employer_nid = $record['employer_nid'];
		if (!$employer_names[$employer_nid]) { $employer_names[$employer_nid] = sirius_node_title($employer_nid); }
		$data[$key]['employer_name'] = $employer_names[$employer_nid];
	}

	uksort($data, function ($a, $b) use ($data) {
		foreach (array('year', 'month', 'day', 'employer_name', 'group_name') as $col) {
			if ($data[$a][$col] < $data[$b][$col]) { return -1; }
			if ($data[$a][$col] > $data[$b][$col]) { return 1; }
		}
	});

	$rows = array();
	foreach ($data as $key => $record) {
		$row = array();
		$row[] = $record['year'];
		$row[] = $record['month'];
		$row[] = $record['day'];
		$row[] = $record['employer_nid'];
		$row[] = $record['employer_name'];
		$row[] = $record['group_nid'];
		$row[] = $record['group_name'];
		$row[] = $record['count'];
		$rows[] = $row;
	}

	$header = array();
	$header[] = 'Year';
	$header[] = 'Month';
	$header[] = 'Day';
	$header[] = 'Employer NID';
	$header[] = 'Employer Name';
	$header[] = 'Show NID';
	$header[] = 'Show Name';
	$header[] = 'Count';

	if ($output == 'csv') {
    $fp = fopen('php://output', 'w');
    $filename = 'labor_projection.csv';
    drupal_add_http_header('Content-Type', 'text/csv');
    drupal_add_http_header('Content-Disposition', 'attachment;filename=' . $filename);
    fputcsv($fp, $header);

		foreach ($rows as $row) { fputcsv($fp, $row); }
		fclose($fp);
		exit();
	}

	$html = '<hr><h2>' . t('Results') . '</h2>';
	$html .= sirius_table_render($header, $rows);

	$form_state['results'] = $html;
}