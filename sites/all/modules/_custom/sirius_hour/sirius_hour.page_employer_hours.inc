<?php

// For the moment, this tab is only "simple monthly hours".

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
	for ($year=$year_now-10; $year<$year_now+2; ++$year) { $year_options[$year] = $year; }

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
	$form_state['employer_nid'] = $employer_node->nid;
	$form_state['year'] = $year;
	$form_state['month'] = $month;

	// Get (or create) the employer payperiod entry.
	$domain_nid = sirius_domain_nid();
	$employer_nid = $employer_node->nid;
	$day = 1;
	$epayperiod_node = sirius_hour_epayperiod_load($domain_nid, $employer_nid, $year, $month, $day, 'No');
	$epayperiod_json = sirius_jsonfield_util_node_json_get($epayperiod_node);

	$form['#tree'] = TRUE; 
	$form['intro'] = array('#markup' => '<h2>' . t('Simple Monthly Hours: ') . "$month/$year" . '</h2>');

	$form_state['payment_nid'] = $payment_node->nid;

	// Create dropdown ms_level dropdown options
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

	// Get all current dispatches for this employer. SQL for speed.
	$dispatches = sirius_ledger_transmittal_dispatches($employer_node->nid);

	$table_open_html .= '<table class="sirius_employer_hours">';
	$table_open_html .= '<tr>';
	$table_open_html .= '<th>SSN</th>';
	$table_open_html .= '<th>Name</th>';
	$table_open_html .= '<th>Member Status</th>';
	$table_open_html .= '<th>Hours</th>';
	$table_open_html .= '</tr>';
	$form['hours']['table_open'] = array('#markup' => $table_open_html);

	$extra_rows = sirius_domain_variable_get('sirius_ledger_transmittal_extra_rows', 5);
	for ($i=0; $i<$extra_rows; ++$i) {
		$dispatches["extra_" . $i] = array();
	}

	foreach ($dispatches as $dispatch_nid => $dispatch) {
		$worker_json = json_decode($dispatch['worker_json'], TRUE);

		// Figure out the member status. Since I might have multiple member statuses, I neede to pick the one that corresponds to the industry of the job.
		$industry_tid = $dispatch['industry_tid'];
		$dispatch_ms_level = NULL;
		if ($industry_tid) {
			$ms_tids = explode(',', $dispatch['ms_tids']);
			foreach ($ms_tids as $probe) {
				if ($ms_terms[$probe]->field_sirius_industry['und'][0]['tid'] == $industry_tid) {
					$dispatch_ms_level = $probe;
				}
			}
		}

		// I might have a level within an MS
		$level_code = $worker_json['training']['levels'][$ms_tid]['code'];
		if ($level_code) { $dispatch_ms_level .= '|' . $level_code; }

		$form['hours']['dispatches'][$dispatch_nid]['tr_open'] = array('#markup' => '<tr>');

		if ($dispatch) {
			$worker_title = $dispatch['worker_title'];
			$contact_nid = $dispatch['contact_nid'];
			if (!$worker_title) { $worker_title = ''; }
			$ssn = $dispatch['field_sirius_ssn_value'];
			$form['hours']['dispatches'][$dispatch_nid]['ssn'] = array('#markup' => '<td>' . $ssn . '</td>');
			$form['hours']['dispatches'][$dispatch_nid]['worker_nid'] = array('#type' => 'hidden', '#value' => $dispatch['worker_nid']);
			$form['hours']['dispatches'][$dispatch_nid]['contact_nid'] = array('#type' => 'hidden', '#value' => $dispatch['contact_nid']);
		} else {
			$worker_title = $payment_json['transmittal_calcs']['dispatches'][$dispatch_nid]['worker_name'];
			$contact_nid = $payment_json['transmittal_calcs']['dispatches'][$dispatch_nid]['contact_nid'];
			$form['hours']['dispatches'][$dispatch_nid]['ssn'] = array(
				'#type' => 'textfield',
				'#size' => 11,
				'#prefix' => '<td class="container-inline">SSN',
				'#suffix' => '</td>',
			);
			if (isset($payment_json['hours']['dispatches'][$dispatch_nid]['ssn'])) { $form['hours']['dispatches'][$dispatch_nid]['ssn']['#default_value'] = $payment_json['hours']['dispatches'][$dispatch_nid]['ssn']; }
		}

		$form['hours']['dispatches'][$dispatch_nid]['name'] = array('#markup' => '<td>' . $worker_title . '</td>');
		$form['hours']['dispatches'][$dispatch_nid]['ms_level'] = array(
			'#type' => 'select',
			'#options' => $ms_level_options,
			'#prefix' => '<td>',
			'#suffix' => '</td>',
			'#default_value' => $dispatch_ms_level,
		);

		$form['hours']['dispatches'][$dispatch_nid]['qty'] = array(
			'#type' => 'textfield',
			'#size' => 5,
			'#prefix' => '<td>',
			'#suffix' => '</td>',
		);
		if (isset($payment_json['hours']['dispatches'][$dispatch_nid]['qty'])) { $form['hours']['dispatches'][$dispatch_nid]['qty']['#default_value'] = $payment_json['hours']['dispatches'][$dispatch_nid]['qty']; }
	}

	$form['hours']['dispatches'][$dispatch_nid]['tr_close'] = array('#markup' => '</tr>');

	$payment_total = $payment_json['transmittal_calcs']['payment_total'];
	if ($payment_total) {
		$form['hours']['dispatches']['total']['tr_open'] = array('#markup' => '<tr>');
		$form['hours']['dispatches']['total']['title'] = array('#markup' => '<th colspan="3">Total</th>');
		$form['hours']['dispatches']['total']['balance'] = array('#markup' => '<th>' . sirius_ledger_ar_format_balance($balance_total) . '</th>');
		$form['hours']['dispatches']['total']['tr_close'] = array('#markup' => '</tr>');
	}

	$form['hours']['table_close'] = array('#markup' => '</table>');

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}