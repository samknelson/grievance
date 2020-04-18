<?php

function sirius_hour_node_tab_worker_hours($worker_node) {
	return drupal_get_form('sirius_hour_node_tab_worker_hours_form', $worker_node);
}

function sirius_hour_node_tab_worker_hours_form($form, &$form_state, $worker_node) {
	$worker_nid = $worker_node->nid;
	$form_state['worker_nid'] = $worker_nid;

	$html = '<h2>' . t('New Hours Entry') . '</h2>';

	$form['wrap_open'] = array('#markup' => '<div class="sirius-form">');
	$form['intro'] = array('#markup' => $html);

	$start_year = date('Y') - 5;
	$end_year = date('Y') + 5;
	for ($year = $start_year; $year <= $end_year; ++$year) { $years[$year] = $year; }
	for ($month = 1; $month <= 12; ++$month) { $months[$month] = $month; }
	for ($day = 1; $day <= 31; ++$day) { $days[$day] = $day; }

	$department_names = sirius_taxonomy_load('grievance_department', 'tid', 'name', TRUE);
	$hour_type_names = sirius_taxonomy_load('sirius_hour_type', 'tid', 'name', TRUE);
	$employers = sirius_employer_list();

	$form['year'] = array(
		'#title' => t('Year'),
		'#type' => 'select',
		'#options' => $years,
		'#default_value' => date('Y'),
		'#prefix' => '<div class="container-inline">',
		'#suffix' => '</div>',
		'#required' => TRUE,
	);

	$form['month'] = array(
		'#title' => t('Month'),
		'#type' => 'select',
		'#options' => $months,
		'#default_value' => date('m'),
		'#prefix' => '<div class="container-inline">',
		'#suffix' => '</div>',
		'#required' => TRUE,
	);

	$form['day'] = array(
		'#title' => t('Day'),
		'#type' => 'select',
		'#options' => $days,
		'#default_value' => date('d'),
		'#prefix' => '<div class="container-inline">',
		'#suffix' => '</div>',
		'#required' => TRUE,
	);

	$form['employer_nid'] = array(
		'#title' => t('Employer'),
		'#type' => 'select',
		'#options' => $employers,
		'#default_value' => $worker_node->field_grievance_shop['und'][0]['target_id'],
		'#prefix' => '<div class="container-inline">',
		'#suffix' => '</div>',
		'#required' => TRUE,
	);

	$form['hour_type_tid'] = array(
		'#title' => t('Hour Type'),
		'#type' => 'select',
		'#options' => $hour_type_names,
		'#prefix' => '<div class="container-inline">',
		'#suffix' => '</div>',
		'#required' => TRUE,
	);

	$form['department_tid'] = array(
		'#title' => t('Department'),
		'#type' => 'select',
		'#options' => $department_names,
		'#prefix' => '<div class="container-inline">',
		'#suffix' => '</div>',
		'#required' => TRUE,
	);

	$form['hours'] = array(
		'#title' => t('Hours'),
		'#type' => 'textfield',
		'#size' => 5,
		'#prefix' => '<div class="container-inline">',
		'#suffix' => '</div>',
		'#required' => TRUE,
	);

	$form['comment'] = array(
		'#title' => t('Comment'),
		'#type' => 'textfield',
		'#prefix' => '<div class="container-inline">',
		'#suffix' => '</div>',
		'#required' => TRUE,
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Add Hours Entry')
	);

	$html = '<h2>' . t('Current Hours Entries') . '</h2>';
	$sql = "select * from sirius_hours_cache ";
	$sql .= "where worker_nid = :worker_nid ";
	$sql .= "order by year desc, month desc, day desc, hours_type_name, department_name ";
	$stmt = db_query($sql, array(':worker_nid' => $worker_node->nid));
	$rows = array();
	$totals = array();
	$grand_total = 0;
	while ($hr = $stmt->fetchAssoc()) {
		$tools = l(t('Edit'), 'node/' . $hr['payperiod_nid'] . '/sirius_payperiod_entries/edit', array('query' => array('destination' => current_path()), 'attributes' => array('class' => array('tool-button'))));
		$rate = 0;
		if ($hr['hours'] > 0) { $rate = round($hr['total'] / $hr['hours'], 2); } else { $rate = 0; }

		$row = array();
		$row[] = $hr['year'];
		$row[] = $hr['month'];
		$row[] = $hr['day'];
		$row[] = $hr['employer_name'];
		$row[] = $hr['hours_type_name'];
		$row[] = $hr['department_name'];
		$row[] = $hr['hours'];
		$row[] = $rate;
		$row[] = $hr['total'];
		$row[] = $tools;

		$rows[] = $row;

		$totals[$hr['department_name']] += round($hr['hours'], 2);
		$grand_total += round($hr['hours'], 2);
	}

	$html = '<h2>' . t('Summary') . '</h2>';
	$html .= '<ul>';
	foreach ($totals as $key => $value) {
		$html .= '<li>';
		$html .= "$key: $value";
		$html .= '</li>';
	}
	$html .= "<li>Grand Total: $grand_total</li>";
	$html .= '</ul>';
	$form['hours_summary'] = array('#markup' => $html);

	$header = array(
		t('Year'), 
		t('Month'),
		t('Day'), 
		t('Employer'), 
		t('Hours Type'),
		t('Department'),
		t('Hours'),
		t('Rate'),
		t('Total'),
		t('Tools'),
	);

	$html = '<h2>' . t('Details') . '</h2>';
	if (!$rows) { $html .= 'There are no hours entries for this worker.'; } 
	else { $html .= theme_table(array('header' => $header, 'rows' => $rows, 'attributes' => array())); }

	$form['current_hours'] = array('#markup' => $html);
	$form['wrap_close'] = array('#markup' => '</div>');

	return $form;
}

function sirius_hour_node_tab_worker_hours_form_submit($form, &$form_state) {
	$worker_nid = $form_state['worker_nid'];
	$form_state['rebuild'] = TRUE;

	$year = $form_state['values']['year'];
	$month = $form_state['values']['month'];
	$day = $form_state['values']['day'];
	$employer_nid = $form_state['values']['employer_nid'];
	$hour_type_tid = $form_state['values']['hour_type_tid'];
	$department_tid = $form_state['values']['department_tid'];
	$hours = $form_state['values']['hours'];
	$start_ts = strtotime("$year-$month-$day 00:00:00");
	$end_ts = $start_ts + ($hours*60*60);
	$comment = $form_state['values']['comment'];

	// Get the payperiod node
	$payperiod_node = sirius_hour_payperiod_load($worker_nid, $employer_nid, $year, $month, $day, 'Yes');
	$payperiod_node->field_sirius_payperiod_type['und'][0]['value'] = 'timestamp';
	$json = json_decode($payperiod_node->field_sirius_json['und'][0]['value'], TRUE);
	$json['entries'][] = array(
		'hour_type_tid' => $hour_type_tid,
		'start_ts' => $start_ts,
		'end_ts' => $end_ts,
		'rate' => $rate,
		'department_tid' => $department_tid,
		'qty' => $hours,
		'comment' => $comment,
	);

	sirius_hour_payperiod_compute($json);
	$payperiod_node->field_sirius_json['und'][0]['value'] = json_encode($json);
	node_save($payperiod_node);
}

