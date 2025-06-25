<?php

function sirius_hour_node_tab_worker_hours_access($worker_node) {
	return Sirius_Hour_Access::getInstance()->access('worker_hours_view', array('worker_node' => $worker_node));
}

function sirius_hour_node_tab_worker_hours_form($form, &$form_state, $worker_node) {
	$is_staff = Sirius_Hour_Access::getInstance()->access('worker_hours_edit', array('worker_node' => $worker_node));

	$month_options = array(
		1 => t('January'),
		2 => t('February'),
		3 => t('March'),
		4 => t('April'),
		5 => t('May'),
		6 => t('June'),
		7 => t('July'),
		8 => t('August'),
		9 => t('September'),
		10 => t('October'),
		11 => t('November'),
		12 => t('December'),
	);

	$worker_nid = $worker_node->nid;
	$form_state['worker_nid'] = $worker_nid;

	$form['wrap_open'] = array('#markup' => '<div class="sirius-form">');

	if ($is_staff) {
		$html = '<h2>' . t('Add or Edit Hours Entries') . '</h2>';
		$form['intro'] = array('#markup' => $html);

		$start_year = date('Y') - 30;
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
	}

	$sql = "select sirius_hours_cache.*, field_data_field_sirius_attachments.entity_id as attachment_entity_id from sirius_hours_cache ";
	$sql .= "left join field_data_field_sirius_attachments on field_data_field_sirius_attachments.entity_type = 'node' and field_data_field_sirius_attachments.entity_id = sirius_hours_cache.payperiod_nid and field_data_field_sirius_attachments.delta = 0 ";
	$sql .= "where worker_nid = :worker_nid ";
	$sql .= "order by year desc, month desc, day desc, hours_type_name, department_name ";
	$stmt = db_query($sql, array(':worker_nid' => $worker_node->nid));
	$rows = array();
	$totals = $grand_totals = array();
	$grand_total = 0;

	$is_simple = sirius_domain_component_enabled('hour:simple');
	$is_complex = sirius_domain_component_enabled('hour:complex');

	while ($hr = $stmt->fetchAssoc()) {
		$tools = array();
		if ($is_complex) {
			$tools[] = l(t('View Entries'), 'node/' . $hr['payperiod_nid'] . '/sirius_payperiod_entries_view', array('query' => array('destination' => current_path()), 'attributes' => array('class' => array('tool-button'))));
			$tools[] = l(t('Edit Entries'), 'node/' . $hr['payperiod_nid'] . '/sirius_payperiod_entries_edit', array('query' => array('destination' => current_path()), 'attributes' => array('class' => array('tool-button'))));
		}
		if ($is_simple) {
			$tools[] = l(t('Edit'), 'node/' . $hr['payperiod_nid'] . '/sirius_payperiod_simple', array('query' => array('destination' => current_path()), 'attributes' => array('class' => array('tool-button'))));
		}

		$rate = 0;
		if ($hr['hours'] > 0) { $rate = round($hr['total'] / $hr['hours'], 2); } else { $rate = 0; }

		$row = array();
		$row[] = $hr['year'];
		$row[] = $month_options[$hr['month']];
		if ($is_complex) {
			$row[] = $hr['day'];
		}
		$row[] = $hr['employer_name'];
		$row[] = $hr['hours_type_name'];
		if ($is_staff) { $row[] = $hr['department_name']; }
		$row[] = round($hr['hours'], 2);
		if ($is_staff) { $row[] = $rate; }
		if ($is_staff) { $row[] = $hr['total']; }
		if ($is_staff) { $row[] = join('', $tools); }

		$rows[] = $row;

		$totals[$hr['hours_type_name']][$hr['department_name']] += round($hr['hours'], 2);
		$grand_totals[$hr['hours_type_name']] += round($hr['hours'], 2);
	}

	if ($is_staff) {
		foreach ($grand_totals as $hours_type_name => $total) {
			$html .= "<strong>Total Hours: $hours_type_name: $total</strong>";
			$html .= '<ul>';
			foreach ($totals[$hours_type_name] as $department_name => $subtotal) {
				$html .= '<li>';
				$html .= "Department: $department_name: $subtotal";
				$html .= '</li>';
			}
			$html .= '</ul>';
		}
		$form['hours_summary'] = array('#markup' => $html);
	}

	$header = array();
	$header[] = t('Year');
	$header[] = t('Month');
	if ($is_complex) { $header[] = t('Day'); }
	$header[] = t('Employer');
	$header[] = t('Hours Type');
	if ($is_staff) { $header[] = t('Department'); }
	$header[] = t('Hours');
	if ($is_staff) { $header[] = t('Rate'); }
	if ($is_staff) { $header[] = t('Total'); }
	if ($is_staff) { $header[] = t('Tools'); }

	if ($is_staff) { $html = '<h2>' . t('Details') . '</h2>'; }
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

	$is_simple = sirius_domain_component_enabled('hour:simple');
	$is_complex = sirius_domain_component_enabled('hour:complex');

	if ($is_complex) {
		drupal_goto('node/' . $payperiod_node->nid . '/sirius_payperiod_entries_edit');
	} else if ($is_simple) {
		drupal_goto('node/' . $payperiod_node->nid . '/sirius_payperiod_simple');
	} else {
		drupal_set_message("Please enable either a simple or complex hours component for this domain.", 'error');
	}
}

