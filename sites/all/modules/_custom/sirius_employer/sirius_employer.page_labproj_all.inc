<?php

function sirius_employer_page_labproj_all() {
	return drupal_get_form('sirius_employer_page_labproj_all_form');
}

function sirius_employer_page_labproj_all_form($form, &$form_state) {
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

	$form['granularity'] = array(
		'#title' => t('Granularity'),
		'#type' => 'select',
		'#options' => array(
			'month' => 'One row per month',
			'day' => 'One row per day',
		),
		'#required' => TRUE,
	);

	$form['detail'] = array(
		'#title' => t('Detail'),
		'#type' => 'select',
		'#options' => array(
			'total' => 'Show Overall Totals',
			'employer' => 'Show Per-Employer Totals',
		),
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

function sirius_employer_page_labproj_all_form_submit($form, &$form_state) {
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

	$included_group_nids = array();

	$grand_total = 0;
	$data_by_month = array();
	$data_by_day = array();
	$data_by_employermonth = array();
	while ($year < $end_year || ($year == $end_year && $month <= $end_month)) {
		$count_yes = 0;
		$count_no = 0;
		$count = 0;
		$labproj_nids = sirius_employer_labproj_list(array('yearmonth' => "$year/$month"));
		$labproj_nodes = node_load_multiple($labproj_nids);

		foreach ($labproj_nodes as $labproj_nid => $labproj_node) {
			$employer_node = sirius_log_handler($labproj_node, 'grievance_shop');
			$employer_nid = $employer_node->nid;
			$labproj_json = sirius_json_get($labproj_node);

			$data_by_month["$year-$month"]['total'] += $labproj_json['totals']['total'];
			$data_by_month["$year-$month"]['max'] += $labproj_json['totals']['max'];
			if ($labproj_json['totals']['group_totals']) {
				foreach ($labproj_json['totals']['group_totals'] as $group_nid => $value) {
					++$included_group_nids[$group_nid];
					$data_by_month["$year-$month"]['group_totals'][$group_nid] += $value;
				}
			}
			if ($labproj_json['totals']['group_maxes']) {
				foreach ($labproj_json['totals']['group_maxes'] as $group_nid => $value) {
					$data_by_month["$year-$month"]['group_maxes'][$group_nid] += $value;
				}
			}

			$data_by_employermonth["$year-$month"][$employer_nid] = $labproj_json['totals'];

			if ($labproj_json['days']) {
				foreach ($labproj_json['days'] as $day => $day_record) {
					if ($day_record['groups']) {
						if ($day_record['total']) { $data_by_day["$year-$month"][$day]['total'] += $day_record['total']; }
						foreach ($day_record['groups'] as $group_nid => $value) {
							if ($value) { $data_by_day["$year-$month"][$day]['groups'][$group_nid] += $value; }
						}
					}
				}
			}
		}

		$month++;
		if ($month > 12) { $month = 1; $year++; }
	}

	foreach ($data_by_day as $key => $records) {
		ksort($data_by_day[$key]);
	}

	$rows = array();
	foreach ($data_by_month as $key => $month_record) {
		list ($year, $month) = explode('-', $key);

		if ($granularity == 'day') {
			foreach ($data_by_day[$key] as $day => $day_record) {
				$daystr = date('D', strtotime("$year-$month-$day"));
				$class = array();
				if ($daystr == 'Sat' || $daystr == 'Sun') { $class[] = 'weekend'; }
				$row = array();
				$row[] =  "$month/$year";
				$row[] = "$daystr $day";
				$row[] = $day_record['total'];
				foreach (array_keys($included_group_nids) as $group_nid) {
					$row[] = $day_record['groups'][$group_nid];
				}
				$rows[] = array('class' => $class, 'data' => $row);
			}
		}

		if ($detail == 'employer') {
			foreach ($data_by_employermonth[$key] as $employer_nid => $totals) {
				$row = array();
				$row[] = "$month/$year";
				$row[] = $employers[$employer_nid] . ' Total';
				$row[] = $totals['total'];
				foreach (array_keys($included_group_nids) as $group_nid) {
					$row[] = $totals['group_totals'][$group_nid];
				}
				$rows[] = $row;

				$row = array();
				$row[] = "$month/$year";
				$row[] = $employers[$employer_nid] .' Max';
				$row[] = $totals['max'];
				foreach (array_keys($included_group_nids) as $group_nid) {
					$row[] = $totals['group_maxes'][$group_nid];
				}
				$rows[] = $row;
			}
		}

		$row = array();
		$row[] = "$month/$year";
		if ($output == 'csv') { 
			$row[] = t('Total');
		} else {
			$row[] = '<strong>' . t('Total') . '</strong>';
		}
		$row[] = $month_record['total'];
		foreach (array_keys($included_group_nids) as $group_nid) {
			$row[] = $month_record['group_totals'][$group_nid];
		}
		$rows[] = $row;

		$row = array();
		$row[] = "$month/$year";
		if ($output == 'csv') { 
			$row[] = t('Max');
		} else {
			$row[] = '<strong>' . t('Max') . '</strong>';
		}
		$row[] = $month_record['max'];
		foreach (array_keys($included_group_nids) as $group_nid) {
			$row[] = $month_record['group_maxes'][$group_nid];
		}
		$rows[] = $row;
	}

	$include_group_nodes = node_load_multiple(array_keys($included_group_nids));

	$header = array(
		'Month',
		'',
		'Overall',
	);
	foreach ($include_group_nodes as $group_nid => $group_node) {
		$header[] = $group_node->title;
	}

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
	$html .= theme_table(array('header' => $header, 'rows' => $rows, 'attributes' => array()));

	$form_state['results'] = $html;
}