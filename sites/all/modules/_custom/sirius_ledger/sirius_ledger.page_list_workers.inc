<?php

function sirius_ledger_page_list_workers() {
	return drupal_get_form('sirius_ledger_page_list_workers_form');
}

function sirius_ledger_page_list_workers_form($form, &$form_state) {
	$accounts = sirius_ledger_account_list();

	$form['#tree'] = TRUE;
	$form['account_tid'] = array(
		'#title' => t('Account'),
		'#type' => 'select',
		'#options' => array(NULL => t('-- Select --')) + $accounts,
		'#required' => TRUE,
	);

	$form['balance_type'] = array(
		'#title' => t('Balance Type'),
		'#type' => 'select',
		'#options' => array(
			NULL => t('-- Select --'),
			'all' => t('All Workers'),
			'all_nonzero' => t('All workers with a nonzero balance'),
			'all_positive' => t('All workers with a positive balance'),
			'all_negative' => t('All workers with a negative balance'),
			'range' => t('All workers with a balance in a specific range')
		),
	);

	$form['balance_range_min'] = array(
		'#type' => 'textfield',
		'#size' => 5,
		'#title' => t('Minimum Balance'),
		'#states' => array(
			'visible' => array(
				":input[name=\"balance_type\"]" => array('value' => 'range')
			),
		),
	);

	$form['balance_range_max'] = array(
		'#type' => 'textfield',
		'#size' => 5,
		'#title' => t('Maximum Balance'),
		'#states' => array(
			'visible' => array(
				":input[name=\"balance_type\"]" => array('value' => 'range')
			),
		),
	);

	$form['over_65'] = array(
		'#type' => 'select',
		'#title' => t('Over 65?'),
		'#options' => array(
			'No' => t('No, select all workers'),
			'Yes' => t('Yes, restrict to workers over 65'),
		),
	);

	$employers = sirius_employer_list();
	$form['employer_nid'] = array(
		'#type' => 'select',
		'#title' => t('Current Employer'),
		'#options' => array(NULL => t('-- Select --')) + $employers,
	);

	$ms_options = sirius_taxonomy_load('sirius_member_status', 'tid', 'name', TRUE);
	$form['ms_tid'] = array(
		'#type' => 'select',
		'#title' => t('Member Status'),
		'#options' => $ms_options,
	);

	$ws_options = array(NULL => t('-- Select --'), 'active' => t('ALL ACTIVE')) + sirius_taxonomy_load('sirius_work_status', 'tid', 'name');
	$form['ws_tid'] = array(
		'#type' => 'select',
		'#title' => t('Work Status'),
		'#options' => $ws_options,
	);

	$ds_options = sirius_taxonomy_load('sirius_worker_dispatch_status', 'tid', 'name');
	$form['ds_tid'] = array(
		'#type' => 'checkboxes',
		'#title' => t('Dispatch Status'),
		'#options' => $ds_options,
	);

	$contact_definition_json = sirius_contact_definition();
	$contact_definition_form = sirius_jsonfield_util_form_get($contact_definition_json, 'default');
	$paths = array_values(sirius_jsonfield_util_form_list_paths($contact_definition_form, TRUE));
	$paths = array_combine($paths, $paths);
	if ($paths) {
		$form['contact_json_path'] = array(
			'#type' => 'select',
			'#title' => 'Additional Display Field',
			'#options' => array(NULL => t('-- Select --')) + $paths,
		);
	}

	$form['output'] = array(
		'#type' => 'select',
		'#title' => t('Output'),
		'#options' => array(
			'screen' => t('Display the results on the screen'),
			'csv' => t('Download the results as a a CSV file'),
		),
		'#default_value' => 'screen',
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	if ($form_state['results']) {
		$form['results'] = array('#markup' => '<hr>' . $form_state['results']);
	}

	return $form;
}

function sirius_ledger_page_list_workers_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;

	$output = $form_state['values']['output'];
	if ($output != 'csv') { $limit = 300; }

	$sql_args = array();
	$sql = "select  ";
	$sql .= "node.nid, ";
	$sql .= "node.title, ";
	$sql .= "field_sirius_id_value, ";
	$sql .= "field_sirius_work_status_tid, ";
	$sql .= "field_sirius_name_family, ";
	$sql .= "field_sirius_name_given, ";
	$sql .= "field_sirius_name_middle, ";
	$sql .= "field_sirius_dob_value, ";
	$sql .= "field_sirius_ssn_value, ";
	$sql .= "field_sirius_phone_value, ";
	$sql .= "field_sirius_email_value, ";
	$sql .= "balance_amount, ";
	$sql .= "group_concat(distinct ms_term.name order by ms_term.name separator ', ') as ms_names, ";
	$sql .= "contact_json.field_sirius_json_value as contact_json, ";

	$sql .= "(select concat(ledger_amount, '|', ledger_ts) ";
	$sql .= "  from sirius_ledger_ar ";
	$sql .= "  where ledger_account = :account_tid_1 ";
	$sql .= "  and ledger_participant = contact_node.nid ";
	$sql .= "  and ledger_amount < 0 ";
	$sql .= "  order by ledger_ts desc limit 1 ";
	$sql .= ") as last_payment ";

	$sql_args[':account_tid_1'] = $form_state['values']['account_tid'];

	$sql .= " from node ";

	$sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_name on field_data_field_sirius_name.entity_type = 'node' and field_data_field_sirius_name.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_id on field_data_field_sirius_id.entity_type = 'node' and field_data_field_sirius_id.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_phone on field_data_field_sirius_phone.entity_type = 'node' and field_data_field_sirius_phone.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_email on field_data_field_sirius_email.entity_type = 'node' and field_data_field_sirius_email.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_work_status on field_data_field_sirius_work_status.entity_type = 'node' and field_data_field_sirius_work_status.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_worker_dispstatus on field_data_field_sirius_worker_dispstatus.entity_type = 'node' and field_data_field_sirius_worker_dispstatus.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_ssn on field_data_field_sirius_ssn.entity_type = 'node' and field_data_field_sirius_ssn.entity_id = node.nid ";
	if ($form_state['values']['employer_nid']) { $sql .= "left join field_data_field_grievance_shop on field_data_field_grievance_shop.entity_type = 'node' and field_data_field_grievance_shop.entity_id = node.nid "; }
	if ($form_state['values']['ws_tid'] == 'active') { $sql .= "left join field_data_field_sirius_member_active ws_active on ws_active.entity_type = 'taxonomy_term' and ws_active.entity_id = field_sirius_work_status_tid "; }
	$sql .= "left join field_data_field_sirius_dob on field_data_field_sirius_dob.entity_type = 'node' and field_data_field_sirius_dob.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_contact on field_data_field_sirius_contact.entity_type = 'node' and field_data_field_sirius_contact.entity_id = node.nid ";
	$sql .= "left join node contact_node on contact_node.nid = field_data_field_sirius_contact.field_sirius_contact_target_id ";
	$sql .= "left join field_data_field_sirius_json contact_json on contact_json.entity_type = 'node' and contact_json.entity_id = contact_node.nid ";
	$sql .= "left join sirius_ledger_balance on (sirius_ledger_balance.balance_participant = contact_node.nid and balance_account = :account_tid) ";
	$sql_args[':account_tid'] = $form_state['values']['account_tid'];
	$sql .= "left join field_data_field_sirius_member_status on field_data_field_sirius_member_status.entity_type = 'node' and field_data_field_sirius_member_status.entity_id = node.nid ";
	$sql .= "left join taxonomy_term_data ms_term on ms_term.tid = field_sirius_member_status_tid ";
	if ($form_state['values']['ms_tid']) { $sql_args[':ms_tid'] = $form_state['values']['ms_tid']; }

	$sql .= "where node.type = 'sirius_worker' ";
	$sql .= "and field_sirius_domain_target_id = :domain_nid ";
	if ($form_state['values']['ms_tid']) { $sql .= "and field_sirius_member_status_tid = :ms_tid "; }
	if ($form_state['values']['ws_tid']) { 
		if ($form_state['values']['ws_tid'] == 'active') {
			$sql .= "and ws_active.field_sirius_member_active_value = 'Yes' ";
		} else {
			$sql_args[':ws_tid'] = $form_state['values']['ws_tid'];
			$sql .= "and field_sirius_work_status_tid = :ws_tid ";
		}
	}
	if ($form_state['values']['balance_type'] == 'all_nonzero') { $sql .= "and balance_amount is not null and balance_amount != 0 "; }
	if ($form_state['values']['balance_type'] == 'all_positive') { $sql .= "and balance_amount is not null and balance_amount > 0 "; }
	if ($form_state['values']['balance_type'] == 'all_negative') { $sql .= "and balance_amount is not null and balance_amount < 0 "; }
	if ($form_state['values']['balance_type'] == 'range') { 
		$sql .= "and balance_amount is not null ";
		if (is_numeric($form_state['values']['balance_range_min'])) {
			$sql_args[':balance_range_min'] = $form_state['values']['balance_range_min'];
			$sql .= "and balance_amount >= :balance_range_min ";
		}
		if (is_numeric($form_state['values']['balance_range_max'])) {
			$sql_args[':balance_range_max'] = $form_state['values']['balance_range_max'];
			$sql .= "and balance_amount <= :balance_range_max ";
		}
	}
	if ($form_state['values']['over_65'] == 'Yes') {
		$dob_cutoff = date('Y-m-d', strtotime('-65 years')) . ' 00:00:00';
		$sql_args[':dob_cutoff'] = $dob_cutoff;
		$sql .= "and field_sirius_dob_value <= :dob_cutoff ";
	}
	if ($form_state['values']['employer_nid']) {
		$sql_args[':employer_nid'] = $form_state['values']['employer_nid'];
		$sql .= "and field_grievance_shop_target_id = :employer_nid ";
	}

	$ds_tids = array_filter(array_values($form_state['values']['ds_tid']));
	if ($ds_tids) {
		$sql_args[':ds_tids'] = $ds_tids;
		$sql .= "and field_sirius_worker_dispstatus_tid in (:ds_tids) ";
	}

	$sql .= "group by node.nid ";
	$sql .= "order by field_sirius_name_family, field_sirius_name_given ";
	if ($limit) { $sql .= "limit $limit "; }
	$sql_args[':domain_nid'] = sirius_domain_nid();
	$stmt = sirius_sql_query($sql, $sql_args);

	$ws_options = sirius_taxonomy_load('sirius_work_status', 'tid', 'name');

	$header = array();
	$header[] = t('Last Name');
	$header[] = t('First Name');
	$header[] = t('Middle Name');
	if ($output == 'csv') {
		$header[] = t('Date of Birth');
		$header[] = t('SSN');
		$header[] = t('Phone');
		$header[] = t('Email');
	}
	$header[] = t('ID');
	if ($form_state['values']['contact_json_path']) {
		$header[] = $form_state['values']['contact_json_path'];
	}
	$header[] = t('Work Status');
	$header[] = t('Member Status');
	$header[] = t('Last Payment Amount');
	$header[] = t('Last Payment Date');
	$header[] = t('Balance');
	if ($output == 'screen') {
		$header[] = t('Tools');
	}

	if ($output == 'csv') {
		$fp = fopen('php://output', 'w');
		$filename = 'balance_report.csv';
		drupal_add_http_header('Content-Type', 'text/csv');
		drupal_add_http_header('Content-Disposition', 'attachment;filename=' . $filename);
		fputcsv($fp, $header);
	}

	$rows = array();
	$count = 0;
	while ($hr = $stmt->fetchAssoc()) {
		++$count;

		if ($output == 'screen') {
			$tools = '';
			$paths = array(
				'node/' . $hr['nid'] => t('Worker'),
				'node/' . $hr['nid'] . '/sirius_worker_ledger' => t('A/R'),
			);
			foreach ($paths as $path => $title) {
				if (drupal_valid_path($path)) {
					$tools .= l($title, $path, array('attributes' => array('class' => array('tool-button'))));
				}
			}
		}

		$balance = $hr['balance_amount'];
		if ($output == 'screen') { $balance = sirius_ledger_ar_format_balance($balance); }

		$last_payment_amount = $last_payment_date = ''; 
		list ($last_payment_amount, $last_payment_ts) = explode('|', $hr['last_payment']);
		if ($last_payment_ts) {
			if ($output == 'screen') { $last_payment_amount = sirius_ledger_ar_format_balance($last_payment_amount); }
			$last_payment_date = date('n/j/Y', $last_payment_ts);
		}

		$ssn = $hr['field_sirius_ssn_value'];
		if ($ssn) {
			$ssn = sirius_ssn_format_conditional($ssn);
		}

		if ($hr['field_sirius_dob_value']) { $dob = date('n/j/Y', strtotime($hr['field_sirius_dob_value'])); } else { $dob = ''; }

		if ($form_state['values']['contact_json_path']) {
			$contact_json = json_decode($hr['contact_json'], TRUE);
			$contact_json_results = sirius_jsonfield_util_results_get($contact_json, 'default');
			$contact_json_value = sirius_jsonfield_util_get_from_path($contact_json_results, $form_state['values']['contact_json_path']);
		} 

		$row = array();
		$row[] = $hr['field_sirius_name_family'];
		$row[] = $hr['field_sirius_name_given'];
		$row[] = $hr['field_sirius_name_middle'];

		if ($output == 'csv') {
			$row[] = $dob;
			$row[] = $ssn;
			$row[] = $hr['field_sirius_phone_value'];
			$row[] = $hr['field_sirius_email_value'];
		}
		$row[] = $hr['field_sirius_id_value'];
		if ($form_state['values']['contact_json_path']) { $row[] = $contact_json_value; }
		$row[] = $ws_options[$hr['field_sirius_work_status_tid']];
		$row[] = $hr['ms_names'];
		$row[] = $last_payment_amount;
		$row[] = $last_payment_date;
		$row[] = $balance;
		if ($output == 'screen') { $row[] = $tools; }

		if ($output == 'screen') {
			$rows[] = $row;
		}

		if ($output == 'csv') {
			fputcsv($fp, $row);
		}
	}

	if ($output == 'csv') {
		fclose($fp);
		exit();
	}

	$result_html = '';
	if ($rows) {
		if ($count == $limit) {
			$result_html .= '<p><em>' . t('Showing the first @limit rows. To view all results, download the output as a CSV file.', array('@limit' => $limit)) . '</em></p>';
		}
		$result_html .= theme_table(array('header' => $header, 'rows' => $rows, 'attributes' => array()));
	} else {
		$result_html .= t('No records found matching the search criteria.');
	}

	$form_state['results'] = $result_html;
}