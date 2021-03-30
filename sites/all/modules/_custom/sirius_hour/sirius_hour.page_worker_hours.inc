<?php

function sirius_hour_node_tab_worker_hours($worker_node) {
	return drupal_get_form('sirius_hour_node_tab_worker_hours_form', $worker_node);
}

function sirius_hour_node_tab_worker_hours_form($form, &$form_state, $worker_node) {
	$worker_nid = $worker_node->nid;
	$form_state['worker_nid'] = $worker_nid;

	$html = '<h2>' . t('Add or Edit Hours Entries') . '</h2>';

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
		'#default_value' => date('n'),
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

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Go')
	);

	$html = '<h2>' . t('Current Hours Entries') . '</h2>';
	$sql = "select sirius_hours_cache.*, field_data_field_sirius_attachments.entity_id as attachment_entity_id from sirius_hours_cache ";
	$sql .= "left join field_data_field_sirius_attachments on field_data_field_sirius_attachments.entity_type = 'node' and field_data_field_sirius_attachments.entity_id = sirius_hours_cache.payperiod_nid and field_data_field_sirius_attachments.delta = 0 ";
	$sql .= "where worker_nid = :worker_nid ";
	$sql .= "order by year desc, month desc, day desc, hours_type_name, department_name ";
	$stmt = db_query($sql, array(':worker_nid' => $worker_node->nid));
	$rows = array();
	$totals = array();
	$grand_total = 0;
	while ($hr = $stmt->fetchAssoc()) {
		$tools = l(t('View'), 'node/' . $hr['payperiod_nid'] . '/sirius_payperiod_entries_view', array('query' => array('destination' => current_path()), 'attributes' => array('class' => array('tool-button'))));
		$tools .= l(t('Edit'), 'node/' . $hr['payperiod_nid'] . '/sirius_payperiod_entries_edit', array('query' => array('destination' => current_path()), 'attributes' => array('class' => array('tool-button'))));
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
	$day = 1;
	$employer_nid = $form_state['values']['employer_nid'];

	// Get the payperiod node
	$domain_nid = sirius_domain_nid();
	$payperiod_node = sirius_hour_payperiod_load($domain_nid, $worker_nid, $employer_nid, $year, $month, $day, 'Yes');
	drupal_goto('node/' . $payperiod_node->nid . '/sirius_payperiod_entries_edit');
}

