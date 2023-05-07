<?php

function sirius_employer_node_tab_labproj_list_access($employer_node) {
	// Admins can see
	if (user_access('sirius employer admin')) { return TRUE; }

	// Scheduling stewards can see
	if (user_access('sirius steward sched worker')) { return TRUE; }	

	// Employers can access their own page only
	if (user_access('sirius employer labor projection manager')) { 
		global $user;
		$user_full = user_load($user->uid);
		if ($user_full->field_grievance_shops['und'][0]['target_id'] == $employer_node->nid) { return TRUE; }
	}

	// Stewards can access if it's their page
	if (user_access("sirius steward")) {
		$my_employer = sirius_employer_get();
		if ($my_employer->nid == $employer_node->nid) { return TRUE; }
	}

	// Oh well.
	return FALSE;
}

function sirius_employer_node_tab_labproj_list($employer_node) {
	return drupal_get_form('sirius_employer_node_tab_labproj_list_form', $employer_node);
}

function sirius_employer_node_tab_labproj_list_form($form, &$form_state, $employer_node) {
	$form_state['employer_nid'] = $employer_node->nid; 

	$start_year = date('Y') - 5;
	$end_year = date('Y') + 2;
	for ($year = $start_year; $year <= $end_year; ++$year) { $years[$year] = $year; }
	for ($month = 1; $month <= 12; ++$month) { $months[$month] = $month; }
	for ($day = 1; $day <= 31; ++$day) { $days[$day] = $day; }

	$form['year'] = array(
		'#title' => t('Year'),
		'#type' => 'select',
		'#options' => $years,
		'#default_value' => date('Y'),
		'#required' => TRUE,
	);

	$form['month'] = array(
		'#title' => t('Month'),
		'#type' => 'select',
		'#options' => $months,
		'#default_value' => date('n'),
		'#required' => TRUE,
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Edit'),
	);

	$search_params = array();
	$search_params['employer_nid'] = $employer_node->nid;
	$labproj_nids = sirius_employer_labproj_list($search_params);
	$labproj_nodes = node_load_multiple($labproj_nids);

	uksort($labproj_nodes, function ($a, $b) use ($labproj_nodes) {
		list ($year_a, $month_a) = sirius_employer_labproj_get_yearmonth($labproj_nodes[$a]);
		list ($year_b, $month_b) = sirius_employer_labproj_get_yearmonth($labproj_nodes[$b]);
		if ($year_a > $year_b) { return -1; }
		if ($year_b > $year_a) { return 1; }
		if ($month_a > $month_b) { return -1; }
		if ($month_b > $month_a) { return 1; }
		return $a <=> $b;
	});

	$status_options = sirius_employer_labproj_status_options();

	$rows = array();
	foreach ($labproj_nodes as $labproj_nid => $labproj_node) {
		$paths = array(
			"node/$labproj_nid/employer_labproj_view" => t('View'),
			"node/$labproj_nid/employer_labproj_status" => t('Status'),
			"node/$labproj_nid/employer_labproj_edit" => t('Edit')
		);

		$tools = '';
		foreach ($paths as $path => $title) {
			if (drupal_valid_path($path)) {
				$tools .= l($title, $path, array('attributes' => array('class' => array('tool-button'))));
			}
		}


		$labproj_json = sirius_jsonfield_util_node_json_get($labproj_node);
		list ($year, $month) = sirius_employer_labproj_get_yearmonth($labproj_node);
		$status = sirius_employer_labproj_get_status($labproj_node);

		$row = array();
		$row[] = $year;
		$row[] = $month;
		$row[] = $labproj_json['totals']['total'];
		$row[] = $status_options[$status];
		$row[] = sirius_text_to_html($labproj_node->field_sirius_summary['und'][0]['value']);
		$row[] = $tools;
		$rows[] = $row;
	}

	if ($labproj_nodes) {
		$header = array(
			t('Year'),
			t('Month'), 
			t('Work Days'),
			t('Status'),
			t('Comments'),
			t('Tools'),
		);
		$html .= theme_table(array('header' => $header, 'rows' => $rows, 'attributes' => array('class' => array('sirius_worker_schedule_tors'))));
	} else {
		$html .= '<p>' . t('No labor projections found.') . '</p>';
	}

	$form['list'] = array('#markup' => $html);
	return $form;
}

function sirius_employer_node_tab_labproj_list_form_submit($form, &$form_state) {
	$employer_nid = $form_state['employer_nid'];
	$year = $form_state['values']['year'];
	$month = $form_state['values']['month'];

	$labproj_node = sirius_employer_labproj_fetch($employer_nid, $year, $month, TRUE);
	drupal_goto('node/' . $labproj_node->nid . '/employer_labproj_status');
}