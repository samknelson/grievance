<?php

function sirius_employer_page_labproj_enforce() {
	return drupal_get_form('sirius_employer_page_labproj_enforce_form');
}

function sirius_employer_page_labproj_enforce_form($form, &$form_state) {
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

	$form['year'] = array(
		'#title' => t('Year'),
		'#type' => 'select',
		'#options' => $years,
		'#default_value' => $default_year,
		'#required' => TRUE,
	);

	$form['month'] = array(
		'#title' => t('Month'),
		'#type' => 'select',
		'#options' => $months,
		'#default_value' => $default_month,
		'#required' => TRUE,
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	if ($form_state['results']) {
		$form['results'] = array('#markup' => $form_state['results']);
	}

	return $form;
}

function sirius_employer_page_labproj_enforce_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;
	$month = $form_state['values']['month'];
	$year = $form_state['values']['year'];
	$employers = sirius_employer_list();
	$status_options = sirius_employer_labproj_status_options();

	$delinquent = sirius_employer_labproj_delinquent($year, $month);

	$header = array('Employer', 'Status', 'Total', 'Tools');
	$rows = array();
	$grand_total = 0;
	$count_yes = 0;
	$count_no = 0;
	$count = 0;
	foreach ($employers as $employer_nid => $employer_name) {
		$labproj_nids = sirius_employer_labproj_list(array('key' => "$employer_nid/$year/$month"));
		$labproj_nid = array_shift($labproj_nids);
		$labproj_node = node_load($labproj_nid);
		++$count;

		$tools = '';
		if ($labproj_nid) {
			$tools .= l('View', "node/$labproj_nid/employer_labproj_view", array('attributes' => array('class' => array('tool-button'))));
			$tools .= l('Status', "node/$labproj_nid/employer_labproj_status", array('attributes' => array('class' => array('tool-button'))));
			$tools .= l('Edit', "node/$labproj_nid/employer_labproj_edit", array('attributes' => array('class' => array('tool-button'))));
		}

		if ($labproj_node) {
			$labproj_json = sirius_json_get($labproj_node);
			$total = $labproj_json['totals']['total'];
			$status = $labproj_node->field_sirius_type['und'][0]['value'];
			$status_render = $status_options[$status];
			$grand_total += $total;
			++$count_yes;
		} else {
			$total = 'n/a';
			if ($delinquent) { $status_render = 'DELINQUENT'; } else { $status_render = 'Missing'; }
			++$count_no;
		}

		$row = array();
		$row[] = $employer_name;
		$row[] = $status_render;
		$row[] = $total;
		$row[] = $tools;
		$rows[] = $row;
	}

	$row = array();
	$row[] = '<strong>' . t('Total') . '</strong>';
	$row[] = "<strong>$count_yes / $count</strong>";
	$row[] = "<strong>$grand_total</strong>";
	$row[] = '';
	$rows[] = $row;

	$html = '<hr><h2>' . t('Results: ') . date('l') . '</h2>';
	$html .= "<p>For $month/$year, out of <strong>$count</strong> active employer(s), <strong>$count_yes</strong> have entered labor projection for a total of <strong>$grand_total work days</strong>. <strong>$count_no</strong> employer(s) have not yet submitted labor projections.</p>";
	$html .= theme_table(array('header' => $header, 'rows' => $rows, 'attributes' => array()));

	$form_state['results'] = $html;
}