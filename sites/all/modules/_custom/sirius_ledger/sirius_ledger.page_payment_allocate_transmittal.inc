<?php

function sirius_ledger_node_tab_payment_allocate_transmittal_access($payment_node) {
	return in_array('transmittal', sirius_ledger_node_tab_payment_allocate_options($payment_node));
}

function sirius_ledger_node_tab_payment_allocate_transmittal($payment_node) {
	return drupal_get_form('sirius_ledger_node_tab_payment_allocate_transmittal_form', $payment_node);
}

function sirius_ledger_node_tab_payment_allocate_transmittal_form($form, &$form_state, $payment_node) {
	$form_state['payment_nid'] = $payment_node->nid;
	$employer_node = node_load($payment_node->field_sirius_payer['und'][0]['target_id']);
	if (!$employer_node) {
		drupal_set_message("No payer found for this payment.", 'error');
		return array();
	}
	if ($employer_node->type != 'grievance_shop') {
		drupal_set_message("The payer for this payment is not an employer", 'error');
		return array();
	}

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

	$account_node = node_load($payment_node->field_sirius_ledger_account['und'][0]['target_id']);
	if (!$account_node) {
		drupal_set_message("No account associated with this payment.", 'error');
		return array();
	}
	$account_nid = $account_node->nid;
	$account_name = $account_node->title;

	$rates = $policy_json['ledger']['policy']['rates'][$schedule_key];

	// Get all current dispatches for this employer. SQL for speed.
	$dispatches = sirius_ledger_transmittal_dispatches($employer_node->nid);

	$table_open_html .= '<table class="sirius_ledger_transmittal">';
	$table_open_html .= '<tr>';
	$table_open_html .= '<th>Worker<br />SSN</th>';
	$table_open_html .= '<th>Member Status</th>';
	$table_open_html .= '<th>Hours</th>';
	$table_open_html .= '<th>Rate<br />Balance</th>';
	$table_open_html .= '<th>Withheld</th>';
	$table_open_html .= '</tr>';
	$form['transmittal']['table_open'] = array('#markup' => $table_open_html);


	$extra_rows = sirius_domain_variable_get('sirius_ledger_transmittal_extra_rows', 5);
	for ($i=0; $i<$extra_rows; ++$i) {
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

	$form['transmittal']['dispatches']['total']['tr_open'] = array('#markup' => '<tr>');
	$form['transmittal']['dispatches']['total']['title'] = array('#markup' => '<th colspan="2">Total</th>');
	$form['transmittal']['dispatches']['total']['hours'] = array('#markup' => '<th>____________</th>');
	$form['transmittal']['dispatches']['total'][$account_nid] = array('#markup' => '<th></th><th>____________</th>');
	$form['transmittal']['dispatches']['total']['tr_close'] = array('#markup' => '</tr>');


	$form['transmittal']['table_close'] = array('#markup' => '<table>');

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}