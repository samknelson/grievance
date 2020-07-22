<?php

function sirius_ledger_node_tab_employer_transmittal_access($employer_node) {
	return TRUE;
}

function sirius_ledger_node_tab_employer_transmittal($employer_node) {
	$arg = sirius_node_tab_arg(0);
	if ($arg == 'print') { return drupal_get_form('sirius_ledger_node_tab_employer_transmittal_form', $employer_node); }

	$html .= 'To generate the transmittal form for this employer, ';
	$html .= l('click here.', current_path() . '/print', array('attributes' => array('target' => '_blank')));
	return $html;
}

function sirius_ledger_node_tab_employer_transmittal_form($form, &$form_state, $employer_node) {
	drupal_add_css('sites/all/modules/_custom/sirius_ledger/css/transmittal.css');
	grievance_classes_array('loopback');

	$form_state['employer_nid'] = $employer_node->nid;

	// Fetch the indsutries
	$industries = sirius_taxonomy_load('sirius_industry', 'tid', 'full');
	$ms_terms = sirius_taxonomy_load('sirius_member_status', 'tid', 'full');

	// Get the policy for this employer
	$schedule_key = 'hour';
	$policy_node = sirius_ledger_policy_lookup($schedule_key, NULL, $employer_node);
	if (!$policy_node) {
		drupal_set_message("No charge policy found: " . sirius_minilog_render_html(), 'warning');
		return array();
	}
	$policy_json = sirius_jsonfield_util_node_json_get($policy_node);
	$account_options = sirius_ledger_policy_accounts($policy_json);
	unset($account_options['pay']);
	$rates = $policy_json['ledger']['policy']['rates'][$schedule_key];

	$form['#tree'] = TRUE;
	$form['wrap_open'] = array('#markup' => '<div class="sirius_ledger_transmittal">');

	$intro_html .= '<div class="sirius_form_intro">';
	$intro_html .= '<h1>' . $employer_node->title . '<br />' . 'Transmittal Form ' . date('F Y') . '</h1>';
	/*
	if ($rates) {
		$intro_html .= '<div class="sirius_ledger_transmittal_rates">';
		$intro_html .= '<h2>Rates</h2>';
		$level_options = sirius_ledger_ms_level_options();
		$intro_html .= '<table>';
		$intro_html .= '<tr>';
		$intro_html .= '<th>Member Status</th>';
		foreach ($account_options as $account_nid => $account_name) {
			$intro_html .= '<th>' . $account_name . '</th>';
		}
		$intro_html .= '</tr>';
		foreach ($rates as $key => $values) {
			$intro_html .= '<tr>';
			$intro_html .= '<td>' . $level_options[$key] . '</td>';
			foreach ($account_options as $account_nid => $account_name) {
				$intro_html .= '<td>' . '$' . number_format($values[$account_nid], 2) . '</td>';
			}
			$intro_html .= '</tr>';
		}
		$intro_html .= '</table>';
		$intro_html .= '</div>';
	}
	*/

	$intro_html .= '</div>';
	$form['intro'] = array('#markup' => $intro_html);

	// Get all current dispatches for this employer. SQL for speed.
	$sql_args = array();
	$sql = "select node.nid, ";
	$sql .= "field_sirius_dispatch_status_value, ";
	$sql .= "job_node.title as job_title, ";
	$sql .= "worker_node.title as worker_title, ";
	$sql .= "field_sirius_payrate_value,  ";
	$sql .= "industry_term.tid as industry_tid, ";
	$sql .= "field_sirius_ssn_value, ";
	$sql .= "worker_json.field_sirius_json_value as worker_json, ";
	$sql .= "group_concat(distinct field_sirius_member_status_tid separator ',') as ms_tids, ";
	$sql .= "field_sirius_contact_target_id as contact_nid ";
	$sql .= "from node ";
	$sql .= "left join field_data_field_sirius_dispatch_job on field_data_field_sirius_dispatch_job.entity_type = 'node' and field_data_field_sirius_dispatch_job.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_worker on field_data_field_sirius_worker.entity_type = 'node' and field_data_field_sirius_worker.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_dispatch_status on field_data_field_sirius_dispatch_status.entity_type = 'node' and field_data_field_sirius_dispatch_status.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_payrate on field_data_field_sirius_payrate.entity_type = 'node' and field_data_field_sirius_payrate.entity_id = node.nid ";
	$sql .= "left join node job_node on job_node.nid = field_sirius_dispatch_job_target_id ";
	$sql .= "left join field_data_field_grievance_shop on field_data_field_grievance_shop.entity_type = 'node' and field_data_field_grievance_shop.entity_id = job_node.nid ";
	$sql .= "left join node worker_node on worker_node.nid = field_sirius_worker_target_id ";
	$sql .= "left join field_data_field_sirius_ssn on field_data_field_sirius_ssn.entity_type = 'node' and field_data_field_sirius_ssn.entity_id = worker_node.nid ";
	$sql .= "left join field_data_field_sirius_contact on field_data_field_sirius_contact.entity_type = 'node' and field_data_field_sirius_contact.entity_id = worker_node.nid ";
	$sql .= "left join field_data_field_sirius_json worker_json on worker_json.entity_type = 'node' and worker_json.entity_id = worker_node.nid ";
	$sql .= "left join field_data_field_sirius_member_status on field_data_field_sirius_member_status.entity_type = 'node' and field_data_field_sirius_member_status.entity_id = worker_node.nid ";
	$sql .= "left join field_data_field_sirius_industry on field_data_field_sirius_industry.entity_type = 'node' and field_data_field_sirius_industry.entity_id = job_node.nid ";
	$sql .= "left join taxonomy_term_data industry_term on field_sirius_industry_tid = industry_term.tid ";
	$sql .= "where node.type = 'sirius_dispatch' ";
	$sql .= "and field_grievance_shop_target_id = :employer_nid ";
	$sql_args[':employer_nid'] = $employer_node->nid;
	$sql .= "and field_sirius_dispatch_status_value in ('accepted', 'accepted_secondary') ";
	$sql .= "group by worker_node.nid ";
	$sql .= "order by node.nid ";

	$stmt = sirius_sql_query($sql, $sql_args);

	$table_open_html .= '<table class="sirius_ledger_transmittal">';
	$table_open_html .= '<tr>';
	$table_open_html .= '<th rowspan="2">Worker<br />SSN</th>';
	$table_open_html .= '<th rowspan="2">Member Status</th>';
	$table_open_html .= '<th rowspan="2">Hours</th>';
	foreach ($account_options as $account_nid => $account_name) {
		$table_open_html .= '<th colspan="2">' . $account_name . '</th>';
	}
	$table_open_html .= '</tr>'; 
	$table_open_html .= '<tr>';
	foreach ($account_options as $account_nid => $account_name) {
		$table_open_html .= '<th>Rate<br />Balance</th>';
		$table_open_html .= '<th>Withheld</th>';
	}
	$table_open_html .= '</tr>';
	$form['transmittal']['table_open'] = array('#markup' => $table_open_html);


	$dispatches = array();
	while ($hr = $stmt->fetchAssoc()) {
		$dispatches[$hr['nid']] = $hr;
	}
	for ($i=0; $i<10; ++$i) {
		$dispatches["extra_" . $i] = array();
	}

	foreach ($dispatches as $dispatch_nid => $dispatch) {
		$worker_json = json_decode($dispatch['worker_json'], TRUE);

		// Figure out the member status. Since I might have multiple member statuses, I neede to pick the one that corresponds to the industry of the job.
		$industry_tid = $dispatch['industry_tid'];
		$industry_name = $ms_tid = $ms_name = NULL;
		if ($industry_tid) {
			$industry_name = $industries[$industry_tid]->name;
			$ms_tids = explode(',', $dispatch['ms_tids']);
			foreach ($ms_tids as $probe) {
				if ($ms_terms[$probe]->field_sirius_industry['und'][0]['tid'] == $industry_tid) {
					$ms_tid = $probe;
					$ms_name = $ms_terms[$probe]->name;
				}
			}
		}

		// I might have a level within an MS
		$level_code = $worker_json['training']['levels'][$ms_tid]['code'];
		$level_name = NULL; 
		if ($level_code) { $level_name = sirius_training_levels_fetch_title($ms_tid, $level_code); }
		if ($level_name) { $ms_name .= '<br />' . $level_name; }


		$worker_title = $dispatch['worker_title'];
		if (!$worker_title) { $worker_title = '____________'; }

		$ssn = $dispatch['field_sirius_ssn_value'];
		if (!$ssn) { $ssn = '___-__-____'; }
		if (!$ms_name) { $ms_name = '____________'; }

		$form['transmittal']['dispatches'][$dispatch_nid]['tr_open'] = array('#markup' => '<tr>');

		$form['transmittal']['dispatches'][$dispatch_nid]['title'] = array('#markup' => '<td>' . $worker_title . '<br />' . $ssn . '</td>');
		$form['transmittal']['dispatches'][$dispatch_nid]['ms_name'] = array('#markup' => '<td>' . $ms_name . '</td>');

		$form['transmittal']['dispatches'][$dispatch_nid]['hours'] = array('#markup' => '<td>____________</td>');

		foreach ($account_options as $account_nid => $account_name) {
			// Get the rate
			$rate = $rates[$ms_tid][$account_nid];
			if ($level_code && $rates["$ms_tid|$level_code"][$account_nid]) { $rate = $rates["$ms_tid|$level_code"][$account_nid]; }
			if ($rate) { $rate = '$' . number_format($rate, 2); }
			if (!$rate) { $rate = '____________'; }

			// Get the balance
			$balance = NULL;
			if ($dispatch) {
				$params = array();
				$params['ledger_account'] = $account_nid;
				$params['ledger_participant'] = $dispatch['contact_nid'];
				$params['balance'] = TRUE;
				$balance = sirius_ledger_ar_fetch($params);
			}
			if (isset($balance)) {
				$balance = sirius_ledger_ar_format_balance($balance);
			} else {
				$balance = '';
			}

			$form['transmittal']['dispatches'][$dispatch_nid][$account_nid]['rate'] = array('#markup' => '<td>' . $rate . '<br />' . $balance . '</td>');
			$form['transmittal']['dispatches'][$dispatch_nid][$account_nid]['payments'] = array('#markup' => '<td>____________</td>');
		}

		$form['transmittal']['dispatches'][$dispatch_nid]['tr_close'] = array('#markup' => '</tr>');
	}

	$form['transmittal']['dispatches']['total']['tr_open'] = array('#markup' => '<tr>');
	$form['transmittal']['dispatches']['total']['title'] = array('#markup' => '<th colspan="2">Total</th>');
	$form['transmittal']['dispatches']['total']['hours'] = array('#markup' => '<th>____________</th>');
	foreach ($account_options as $account_nid => $account_name) {
		$form['transmittal']['dispatches']['total'][$account_nid] = array('#markup' => '<th></th><th>____________</th>');
	}
	$form['transmittal']['dispatches']['total']['tr_close'] = array('#markup' => '</tr>');


	$form['transmittal']['table_close'] = array('#markup' => '<table>');

	$form['wrap_close'] = array('#markup' => '</div>');

  return $form;
}