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
		'#default_value' => 'active',
	);

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
	$sql .= "balance_amount, ";
	$sql .= "group_concat(distinct ms_term.name order by ms_term.name separator ', ') as ms_names ";

	$sql .= " from node ";

	$sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_name on field_data_field_sirius_name.entity_type = 'node' and field_data_field_sirius_name.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_id on field_data_field_sirius_id.entity_type = 'node' and field_data_field_sirius_id.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_work_status on field_data_field_sirius_work_status.entity_type = 'node' and field_data_field_sirius_work_status.entity_id = node.nid ";
	if ($form_state['values']['ws_tid'] == 'active') { $sql .= "left join field_data_field_sirius_member_active ws_active on ws_active.entity_type = 'taxonomy_term' and ws_active.entity_id = field_sirius_work_status_tid "; }
	$sql .= "left join field_data_field_sirius_contact on field_data_field_sirius_contact.entity_type = 'node' and field_data_field_sirius_contact.entity_id = node.nid ";
	$sql .= "left join sirius_ledger_balance on (sirius_ledger_balance.balance_participant = field_sirius_contact_target_id and balance_account = :account_tid) ";
	$sql_args[':account_tid'] = $form_state['values']['account_tid'];
	$sql .= "left join field_data_field_sirius_member_status on field_data_field_sirius_member_status.entity_type = 'node' and field_data_field_sirius_member_status.entity_id = node.nid ";
	$sql .= "left join taxonomy_term_data ms_term on ms_term.tid = field_sirius_member_status_tid ";
	if ($form_state['values']['ms_tid']) { $sql_args[':ms_tid'] = $form_state['values']['ms_tid']; }

	$sql .= "where type = 'sirius_worker' ";
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

	$sql .= "group by node.nid ";
	$sql .= "order by field_sirius_name_family, field_sirius_name_given ";
	if ($limit) { $sql .= "limit $limit "; }
	$sql_args[':domain_nid'] = sirius_domain_nid();
	$stmt = sirius_sql_query($sql, $sql_args);

	$ws_options = sirius_taxonomy_load('sirius_work_status', 'tid', 'name');

	if ($output == 'csv') {
		$header = array(
			t('Worker'),
			t('ID'),
			t('Work Status'),
			t('Member Status'),
			t('Balance'),
		);
	}

	if ($output == 'screen') {
		$header = array(
			t('Worker'),
			t('ID'),
			t('Work Status'),
			t('Member Status'),
			t('Balance'),
			t('Tools'),
		);
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

		$row = array();
		$row[] = $hr['title'];
		$row[] = $hr['field_sirius_id_value'];
		$row[] = $ws_options[$hr['field_sirius_work_status_tid']];
		$row[] = $hr['ms_names'];
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