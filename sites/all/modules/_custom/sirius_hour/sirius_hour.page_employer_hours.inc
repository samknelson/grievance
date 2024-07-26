<?php

// For the moment, this tab is only "simple monthly hours" and is tied to transmittal sheets.
// We should eventually separate it out...

function sirius_hour_node_tab_employer_hours_access($employer_node) {
	return sirius_domain_variable_get('sirius_hour_enable_simple_monthly') == 'Yes';
}

function sirius_hour_node_tab_employer_hours($employer_node) {
	$year = sirius_node_tab_arg(0);
	$month = sirius_node_tab_arg(1);

	if (!$year || !$month) { 
		return drupal_get_form('sirius_hour_node_tab_employer_hours_date_form', $employer_node);
	}


	return drupal_get_form('sirius_hour_node_tab_employer_hours_form', $employer_node, $year, $month);
}

function sirius_hour_node_tab_employer_hours_date_form($form, &$form_state, $employer_node) {
	$form_state['employer_nid'] = $employer_node->nid;

	$year_now = date('Y');
	$month_now = date('n');

	$year_options = array();
	for ($year=$year_now-30; $year<$year_now+2; ++$year) { $year_options[$year] = $year; }

	$month_options = array();
	for ($month=1; $month<=12; ++$month) { $month_options[$month] = $month; }

	$form['#tree'] = TRUE; 

	$form['year'] = array(
		'#type' => 'select',
		'#options' => $year_options,
		'#title' => t('Year'),
		'#default_value' => $year_now,
	);

	$form['month'] = array(
		'#type' => 'select',
		'#options' => $month_options,
		'#title' => t('Month'),
		'#default_value' => $month_now,
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_hour_node_tab_employer_hours_date_form_submit($form, &$form_state) {
	$path = current_path() . '/' . $form_state['values']['year'] . '/' . $form_state['values']['month'];
	drupal_goto($path);
}

function sirius_hour_node_tab_employer_hours_form($form, &$form_state, $employer_node, $year, $month) {
	$domain_nid = sirius_domain_nid();
	$day = 1;

	$epayperiod_node = sirius_hour_epayperiod_load($domain_nid, $employer_node->nid, $year, $month, $day);
	$epayperiod_json = sirius_jsonfield_util_node_json_get($epayperiod_node);

	// Get our transmittal NID
	$transmittal_nid = $epayperiod_json['transmittal_nid'];
	$transmittal_node = node_load($transmittal_nid);
	$transmittal_json = sirius_jsonfield_util_node_json_get($transmittal_node);
	if (!$transmittal_node) { drupal_set_message("There is no transmittal form associated with this month and employer."); return array(); }
	
	sirius_backlink_add('node/' . $employer_node->nid . '/sirius_employer_ledger/transmittal', array('name' => 'Transmittal Form'));

	$form_state['transmittal_nid'] = $transmittal_nid;
	$form_state['employer_nid'] = $employer_node->nid;
	$form_state['year'] = $year;
	$form_state['month'] = $month;

	$account_names = sirius_ledger_account_list();
	$accounts = array();
	foreach ($transmittal_json['transmittal']['accounts'] as $account_nid => $record) {
		if ($record['active'] == 'Yes') { $accounts[$account_nid] = $account_names[$account_nid]; }
	}

	$hour_types = sirius_taxonomy_load('sirius_hour_type', 'tid', 'name', TRUE);
	$departments = sirius_taxonomy_load('grievance_department', 'tid', 'name', TRUE);
	$form['hour_type'] = array(
		'#type' => 'select',
		'#title' => t('Hour Type'),
		'#options' => $hour_types,
	);
	if ($epayperiod_json['hour_type']) { $form['hour_type']['#default_value'] = $epayperiod_json['hour_type']; } 
	else { $form['hour_type']['#default_value'] = sirius_domain_variable_get('sirius_hour_default_type', ''); }

	$form['department'] = array(
		'#type' => 'select',
		'#title' => t('Department'),
		'#options' => $departments,
	);
	if ($epayperiod_json['department']) { $form['department']['#default_value'] = $epayperiod_json['department']; }
	else {$form['department']['#default_value'] = sirius_domain_variable_get('sirius_hour_default_department', ''); }

	// Get (or create) the employer payperiod entry.
	$domain_nid = sirius_domain_nid();
	$employer_nid = $employer_node->nid;

	$form['#tree'] = TRUE; 
	$form['intro'] = array('#markup' => '<h2>' . t('Simple Monthly Hours: ') . "$month/$year" . '</h2>');

	// Create ms_level dropdown options
	$industries = sirius_taxonomy_load('sirius_industry', 'tid', 'full');
	$ms_terms = sirius_taxonomy_load('sirius_member_status', 'tid', 'full');
	$employer_industry_tids = sirius_fieldvals($employer_node, 'field_sirius_industry', 'tid');
	$employer_ms_tids = array();
	foreach ($ms_terms as $ms_tid => $ms_term) {
		$industry_tid = $ms_term->field_sirius_industry['und'][0]['tid'];
		if (in_array($industry_tid, $employer_industry_tids)) { $employer_ms_tids[] = $ms_tid; }
	}

	$ms_level_options = array(NULL => t('-- Select --'));
	foreach ($employer_ms_tids as $ms_tid) {
		$levels = sirius_training_levels_fetch($ms_tid);
		$ms_level_options[$ms_tid] = $ms_terms[$ms_tid]->name;
		foreach ($levels as $level_code => $level) {
			$ms_level_options["$ms_tid|$level_code"] = $ms_terms[$ms_tid]->name . ': ' . $level['title'];
		}
	}

	// Get all current dispatches for this employer.
	$dispatches = $transmittal_json['transmittal']['dispatches'];

	$table_open_html .= '<table class="sirius_employer_hours">';
	$table_open_html .= '<tr>';
	$table_open_html .= '<th>Name</th>';
	$table_open_html .= '<th>Member Status</th>';
	$table_open_html .= '<th>Hours</th>';
	foreach ($accounts as $account_nid => $account_name) {
		$table_open_html .= '<th>' . $account_name . '</th>';
	}
	$table_open_html .= '</tr>';
	$form['hours']['table_open'] = array('#markup' => $table_open_html);

	$deltas = array();
	foreach ($dispatches as $dispatch_nid => $dispatch) {
		$worker_nid = $dispatch['worker_nid'];
		if (!$worker_nid) { continue; }
		$payperiod_node = sirius_hour_payperiod_load($domain_nid, $worker_nid, $employer_node->nid, $year, $month, $day, 'Yes');
		$payperiod_json = sirius_jsonfield_util_node_json_get($payperiod_node);
		$entry = array();
		if ($payperiod_json['entries']['employer_monthly']['rows']) {
			if (!$deltas[$worker_nid]) { $deltas[$worker_nid] = 0; }
			$entry = $payperiod_json['entries']['employer_monthly']['rows'][$deltas[$worker_nid]];
			++$deltas[$worker_nid];
		}

		$form['hours']['dispatches'][$dispatch_nid]['tr_open'] = array('#markup' => '<tr>');
		$form['hours']['dispatches'][$dispatch_nid]['name'] = array('#markup' => '<td>' . $dispatch['worker_name'] . '</td>');
		$form['hours']['dispatches'][$dispatch_nid]['ms_level'] = array('#markup' => '<td>' . $ms_level_options[$dispatch['ms_level']] . '</td>');

		$form['hours']['dispatches'][$dispatch_nid]['qty'] = array(
			'#type' => 'textfield',
			'#size' => 5,
			'#prefix' => '<td>',
			'#suffix' => '</td>',
		);
		if ($entry['qty']) { $form['hours']['dispatches'][$dispatch_nid]['qty']['#default_value'] = $entry['qty']; }

		foreach ($accounts as $account_nid => $account_name) {
			$balance = 0;
			$params = array();
			$params['ledger_account'] = $account_nid;
			$params['ledger_participant'] = $dispatch['contact_nid'];
			$params['ledger_reference'] = $payperiod_node->nid;
			$params['ledger_key'] = $dispatch['contact_nid'] . '|' . $payperiod_node->nid . '|' . $dispatch['ms_level'];
			$params['balance'] = TRUE;
			$balance = sirius_ledger_ar_fetch($params);
			if (!$balance) { $balance = 0; }
			if (isset($balance)) {
				$balance_format = sirius_ledger_ar_format_balance($balance);
			} else {
				$balance_format = '';
			}

			$form['hours']['dispatches'][$dispatch_nid][$account_nid] = array('#markup' => '<td>' . $balance_format . '</td>');
		}
	}

	$form['hours']['dispatches'][$dispatch_nid]['tr_close'] = array('#markup' => '</tr>');

	/*
	$payment_total = $payment_json['transmittal_calcs']['payment_total'];
	if ($payment_total) {
		$form['hours']['dispatches']['total']['tr_open'] = array('#markup' => '<tr>');
		$form['hours']['dispatches']['total']['title'] = array('#markup' => '<th colspan="3">Total</th>');
		$form['hours']['dispatches']['total']['balance'] = array('#markup' => '<th>' . sirius_ledger_ar_format_balance($balance_total) . '</th>');
		$form['hours']['dispatches']['total']['tr_close'] = array('#markup' => '</tr>');
	}
	*/

	$form['hours']['table_close'] = array('#markup' => '</table>');

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_hour_node_tab_employer_hours_form_validate($form, &$form_state) {
	$employer_node = node_load($form_state['employer_nid']);
	$year = $form_state['year'];
	$month = $form_state['month'];
	$day = 1;

	foreach ($form_state['values']['hours']['dispatches'] as $dispatch_nid => $hour_record) {
		$qty = $hour_record['qty'];
		if ($qty < 0) {
			form_set_error("hours][dispatches][$dispatch_nid][qty", 'Payment cannot be negative');
			continue;
		}

		if ($qty && !is_numeric($qty)) { 
			form_set_error("hours][dispatches][$dispatch_nid][qty", 'Payment must be a number');
			continue;
		}
	}
}

function sirius_hour_node_tab_employer_hours_form_submit($form, &$form_state) {
	$employer_node = node_load($form_state['employer_nid']);
	$transmittal_node = node_load($form_state['transmittal_nid']);
	$year = $form_state['year'];
	$month = $form_state['month'];
	$day = 1;
	$domain_nid = sirius_domain_nid();

	// Save the transmittal nid in the epayperiod node
	$epayperiod_node = sirius_hour_epayperiod_load($domain_nid, $employer_node->nid, $year, $month, $day, 'Yes');
	$epayperiod_json = sirius_jsonfield_util_node_json_get($epayperiod_node);
	$epayperiod_json['transmittal_nid'] = $transmittal_node->nid;
	$epayperiod_json['hour_type'] = $form_state['values']['hour_type'];
	$epayperiod_json['department'] = $form_state['values']['department'];
	sirius_jsonfield_util_node_json_set($epayperiod_node, $epayperiod_json);
	node_save($epayperiod_node);

	$transmittal_json = sirius_jsonfield_util_node_json_get($transmittal_node);

	$source_key = 'employer_monthly';
	$source_name = 'Employer Monthly';

	$payperiod_nodes = array();
	foreach ($form_state['values']['hours']['dispatches'] as $dispatch_nid => $hour_record) {
		$qty = $hour_record['qty'];
		$ms_level = $transmittal_json['transmittal']['dispatches'][$dispatch_nid]['ms_level'];
		$worker_nid = $transmittal_json['transmittal']['dispatches'][$dispatch_nid]['worker_nid'];

		$is_first = FALSE;
		if (!$payperiod_nodes[$worker_nid]) { 
			$payperiod_nodes[$worker_nid] = sirius_hour_payperiod_load($domain_nid, $worker_nid, $employer_node->nid, $year, $month, $day, 'Yes');
			$is_first = TRUE;
		}
		
		$payperiod_json = sirius_jsonfield_util_node_json_get($payperiod_nodes[$worker_nid]);

		if ($is_first) { 
			$payperiod_json['entries'][$source_key]['source_name'] = $source_name;
			$payperiod_json['entries'][$source_key]['status'] = 'live';
			$payperiod_json['entries'][$source_key]['rows'] = array();
		}	

		$payperiod_json['entries'][$source_key]['rows'][] = array(
			'qty' => $qty,
			'ms_level' => $ms_level,
			'day' => $day,
			'hour_type' => $form_state['values']['hour_type'],
			'department' => $form_state['values']['department'],
		);
		sirius_jsonfield_util_node_json_set($payperiod_nodes[$worker_nid], $payperiod_json);
	}

	foreach (array_keys($payperiod_nodes) as $worker_nid) {
		$payperiod_nodes[$worker_nid]->field_sirius_active['und'][0]['value'] = 'Yes';
		$payperiod_json = sirius_jsonfield_util_node_json_get($payperiod_nodes[$worker_nid]);
		sirius_hour_payperiod_compute($payperiod_json);
		sirius_jsonfield_util_node_json_set($payperiod_nodes[$worker_nid], $payperiod_json);
		node_save($payperiod_nodes[$worker_nid]);
	}

	drupal_set_message("Hours saved.");

	// drupal_set_message('<pre>' . sirius_minilog_render_html() . '</pre>');
}
