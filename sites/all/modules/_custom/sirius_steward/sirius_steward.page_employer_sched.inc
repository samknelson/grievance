<?php

function sirius_steward_node_tab_employer_sched_access($employer_node) {
	return sirius_steward_sched_access('view', $employer_node->nid);
}

function sirius_steward_node_tab_employer_sched_form($form, &$form_state, $employer_node) {
	$form_state['employer_nid'] = $employer_node->nid; 

	// Render the schedule for the next few days
	$html = '<div class="sirius_form_intro">';
	$found = FALSE;
	for ($i = 0; $i<7; ++$i) {
		if ($i > 0) { $ts = strtotime("+$i days"); } else { $ts = time(); }
		$sched_node = sirius_steward_sched_fetch($employer_node->nid, date('Y-m-d', $ts), FALSE);
		if ($sched_node) {
			$options = array();
			$options['hide_checkins'] = TRUE;
			if ($found) { $options['hide_header'] = TRUE; } else { $found = TRUE; }
			$html .= sirius_steward_sched_render($sched_node, $options);
		}
	}
	if (!$found) {
		$html .= t('No steward schedules have been entered for the upcoming seven days.');
	}
	$html .= '</div>';
	$html .= '<h2>' . t('Create or Edit a Steward Schedule') . '</h2>';

	$form['intro'] = array('#markup' => $html);

	$start_year = date('Y') - 5;
	$end_year = date('Y') + 2;
	for ($year = $start_year; $year <= $end_year; ++$year) { $years[$year] = $year; }
	for ($month = 1; $month <= 12; ++$month) { $months[$month] = $month; }
	for ($day = 1; $day <= 31; ++$day) { $days[$day] = $day; }

	if (user_access('sirius steward sched worker')) {
		$form['date'] = array(
			'#title' => t('Date'),
			'#type' => 'date_popup',
	    '#attributes' => array(
	      'autocomplete' => 'off',
	    ),
	    '#required' => TRUE,
	    '#date_format' => 'Y-m-d',
	    '#default_value' => date('Y-m-d'),
		);

		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Edit'),
		);
	}

	/*
	$search_params = array();
	$search_params['employer_nid'] = $employer_node->nid;
	$labproj_nids = sirius_employer_labproj_list($search_params);
	$sched_nodes = node_load_multiple($labproj_nids);

	uksort($sched_nodes, function ($a, $b) use ($sched_nodes) {
		list ($year_a, $month_a) = sirius_employer_labproj_get_yearmonth($sched_nodes[$a]);
		list ($year_b, $month_b) = sirius_employer_labproj_get_yearmonth($sched_nodes[$b]);
		if ($year_a > $year_b) { return -1; }
		if ($year_b > $year_a) { return 1; }
		if ($month_a > $month_b) { return -1; }
		if ($month_b > $month_a) { return 1; }
		return $a <=> $b;
	});

	$status_options = sirius_employer_labproj_status_options();

	$rows = array();
	foreach ($sched_nodes as $labproj_nid => $sched_node) {
		$tools = '';
		$tools .= l('View', "node/$labproj_nid/employer_labproj_view", array('attributes' => array('class' => array('tool-button'))));
		$tools .= l('Status', "node/$labproj_nid/employer_labproj_status", array('attributes' => array('class' => array('tool-button'))));
		$tools .= l('Edit', "node/$labproj_nid/employer_labproj_edit", array('attributes' => array('class' => array('tool-button'))));

		$labproj_json = sirius_jsonfield_util_node_json_get($sched_node);
		list ($year, $month) = sirius_employer_labproj_get_yearmonth($sched_node);
		$status = sirius_employer_labproj_get_status($sched_node);

		$row = array();
		$row[] = $year;
		$row[] = $month;
		$row[] = $labproj_json['totals']['total'];
		$row[] = $status_options[$status];
		$row[] = sirius_text_to_html($sched_node->field_sirius_summary['und'][0]['value']);
		$row[] = $tools;
		$rows[] = $row;
	}

	if ($sched_nodes) {
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
	*/

	return $form;
}

function sirius_steward_node_tab_employer_sched_form_validate($form, &$form_state) {
	$date = $form_state['values']['date'];
	if (strtotime($date) < strtotime('-1 day')) {
		form_set_error('date', 'Please enter a date in the future.');
	}
}


function sirius_steward_node_tab_employer_sched_form_submit($form, &$form_state) {
	$employer_nid = $form_state['employer_nid'];
	$sched_node = sirius_steward_sched_fetch($employer_nid, $form_state['values']['date'], TRUE);
	drupal_goto('node/' . $sched_node->nid . '/sirius_steward_log_sched/edit');
}
