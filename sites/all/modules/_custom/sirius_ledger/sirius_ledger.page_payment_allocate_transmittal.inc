<?php

function sirius_ledger_node_tab_payment_allocate_transmittal_access($payment_node) {
	return in_array('transmittal', sirius_ledger_node_tab_payment_allocate_options($payment_node));
}

function sirius_ledger_node_tab_payment_allocate_transmittal($payment_node) {
	return drupal_get_form('sirius_ledger_node_tab_payment_allocate_transmittal_form', $payment_node);
}

function sirius_ledger_node_tab_payment_allocate_transmittal_form($form, &$form_state, $payment_node) {
	$form_state['payment_nid'] = $payment_node->nid;
	$payment_json = sirius_jsonfield_util_node_json_get($payment_node);
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


	// Get all current dispatches for this employer. SQL for speed.
	$dispatches = sirius_ledger_transmittal_dispatches($employer_node->nid);

	$form['#tree'] = TRUE;

  $form['transmittal']['ledger_date'] = array(
    '#type' => 'date_popup',
    '#title' => t('Enter payments as of'),
    '#attributes' => array(
      'autocomplete' => 'off',
    ),
    '#description' => t('If there is no payment date in the upload, use this date.'),
    '#required' => TRUE,
  );
  if ($payment_json['transmittal']['ledger_date']) { $form['transmittal']['ledger_date']['#default_value'] = $payment_json['transmittal']['ledger_date']; }


	$table_open_html .= '<table class="sirius_ledger_transmittal">';
	$table_open_html .= '<tr>';
	$table_open_html .= '<th>SSN</th>';
	$table_open_html .= '<th>Name</th>';
	$table_open_html .= '<th>Member Status</th>';
	$table_open_html .= '<th>Withheld</th>';
	$table_open_html .= '<th>Current Balance</th>';
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

		$form['transmittal']['dispatches'][$dispatch_nid]['tr_open'] = array('#markup' => '<tr>');

		if ($dispatch) {
			$worker_title = $dispatch['worker_title'];
			$contact_nid = $dispatch['contact_nid'];
			if (!$worker_title) { $worker_title = ''; }
			$ssn = $dispatch['field_sirius_ssn_value'];
			$form['transmittal']['dispatches'][$dispatch_nid]['ssn'] = array('#markup' => '<td>' . $ssn . '</td>');
			$form['transmittal']['dispatches'][$dispatch_nid]['worker_nid'] = array('#type' => 'hidden', '#value' => $dispatch['worker_nid']);
			$form['transmittal']['dispatches'][$dispatch_nid]['contact_nid'] = array('#type' => 'hidden', '#value' => $dispatch['contact_nid']);
		} else {
			$worker_title = $payment_json['transmittal_calcs']['dispatches'][$dispatch_nid]['worker_name'];
			$contact_nid = $payment_json['transmittal_calcs']['dispatches'][$dispatch_nid]['contact_nid'];
			$form['transmittal']['dispatches'][$dispatch_nid]['ssn'] = array(
				'#type' => 'textfield',
				'#size' => 11,
				'#prefix' => '<td class="container-inline">SSN',
				'#suffix' => '</td>',
			);
			if (isset($payment_json['transmittal']['dispatches'][$dispatch_nid]['ssn'])) { $form['transmittal']['dispatches'][$dispatch_nid]['ssn']['#default_value'] = $payment_json['transmittal']['dispatches'][$dispatch_nid]['ssn']; }
		}

		$form['transmittal']['dispatches'][$dispatch_nid]['name'] = array('#markup' => '<td>' . $worker_title . '</td>');
		$form['transmittal']['dispatches'][$dispatch_nid]['ms_name'] = array('#markup' => '<td>' . $ms_name . '</td>');

		$form['transmittal']['dispatches'][$dispatch_nid]['payment'] = array(
			'#type' => 'textfield',
			'#size' => 5,
			'#prefix' => '<td class="container-inline">$',
			'#suffix' => '</td>',
		);
		if (isset($payment_json['transmittal']['dispatches'][$dispatch_nid]['payment'])) { $form['transmittal']['dispatches'][$dispatch_nid]['payment']['#default_value'] = $payment_json['transmittal']['dispatches'][$dispatch_nid]['payment']; }

		// Get the balance
		$balance = 0;
		if ($contact_nid) {
			$params = array();
			$params['ledger_account'] = $account_nid;
			$params['ledger_participant'] = $dispatch['contact_nid'];
			$params['balance'] = TRUE;
			$balance = sirius_ledger_ar_fetch($params);
			if (!$balance) { $balance = 0; }
		}
		if (isset($balance)) {
			$balance_format = sirius_ledger_ar_format_balance($balance);
		} else {
			$balance_format = '';
		}

		$form['transmittal']['dispatches'][$dispatch_nid]['balance'] = array('#markup' => '<td>' . $balance_format . '</td>');
		$form['transmittal']['dispatches'][$dispatch_nid]['balance_calc'] = array('#type' => 'hidden', '#value' => $balance);
	}

	$form['transmittal']['dispatches'][$dispatch_nid]['tr_close'] = array('#markup' => '</tr>');

	$payment_total = $payment_json['transmittal_calcs']['payment_total'];
	$balance_total = $payment_json['transmittal_calcs']['balance_total'];
	if ($payment_total) {
		$form['transmittal']['dispatches']['total']['tr_open'] = array('#markup' => '<tr>');
		$form['transmittal']['dispatches']['total']['title'] = array('#markup' => '<th colspan="3">Total</th>');
		$form['transmittal']['dispatches']['total']['total'] = array('#markup' => '<th>' . sirius_ledger_ar_format_balance($payment_total) . '</th>');
		$form['transmittal']['dispatches']['total']['balance'] = array('#markup' => '<th>' . sirius_ledger_ar_format_balance($balance_total) . '</th>');
		$form['transmittal']['dispatches']['total']['tr_close'] = array('#markup' => '</tr>');
	}

	$form['transmittal']['table_close'] = array('#markup' => '</table>');

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_ledger_node_tab_payment_allocate_transmittal_form_validate($form, &$form_state) {
	$payment_node = node_load($form_state['payment_nid']);

	$payment_total = 0;
	$balance_total = 0;
	foreach ($form_state['values']['transmittal']['dispatches'] as $dispatch_nid => $transmittal) {
		$payment = $transmittal['payment'];
		if ($payment < 0) {
			form_set_error("transmittal][dispatches][$dispatch_nid][payment", 'Payment cannot be negative');
			continue;
		}

		if (is_numeric($payment)) { $payment_total += $payment; }
		if (is_numeric($transmittal['balance_calc'])) { $balance_total += $transmittal['balance_calc']; }

		if ($dispatch_nid > 0) {
			if (!$transmittal['contact_nid']) {
				form_set_error("transmittal][dispatches][$dispatch_nid][payment", 'There is no contact associated with this worker.');
				continue;
			}
		} else {
			$ssn = $transmittal['ssn'];
			if ($ssn || $payment) {
				if (!$payment) {
					form_set_error("transmittal][dispatches][$dispatch_nid][payment", 'Please enter a payment');
					continue; 
				}
				if (!$ssn) {
					form_set_error("transmittal][dispatches][$dispatch_nid][ssn", 'Please enter a SSN');
					continue; 
				}
				if (!sirius_ssn_validate($ssn)) {
					form_set_error("transmittal][dispatches][$dispatch_nid][ssn", 'Please enter a valid SSN');
					continue; 
				}
				$ssn = sirius_ssn_format($ssn);
				$worker_node = sirius_worker_get_by_ssn($ssn);
				if (!$worker_node) {
					form_set_error("transmittal][dispatches][$dispatch_nid][ssn", 'No worker found with this SSN');
					continue; 
				}
				$form_state['storage']['dispatches'][$dispatch_nid]['worker_nid'] = $worker_node->nid;
				$form_state['storage']['dispatches'][$dispatch_nid]['worker_name'] = $worker_node->title;
				$form_state['storage']['dispatches'][$dispatch_nid]['contact_nid'] = $worker_node->field_sirius_contact['und'][0]['target_id'];
				if (!$worker_node->field_sirius_contact['und'][0]['target_id']) {
					form_set_error("transmittal][dispatches][$dispatch_nid][ssn", 'There is no contact associated with this worker.');
					continue; 
				}
			}
		}
	}
	$form_state['storage']['payment_total'] = $payment_total;
	$form_state['storage']['balance_total'] = $balance_total;
	$check_value = -1 * $payment_node->field_sirius_dollar_amt['und'][0]['value'];

	if ($check_value != $payment_total) {
		form_set_error('transmittal][dispatches', 'The total payment amount (' . sirius_ledger_ar_format_balance($payment_total) . '), must match the payment value (' . sirius_ledger_ar_format_balance($check_value) . ')');
	}
}


function sirius_ledger_node_tab_payment_allocate_transmittal_form_submit($form, &$form_state) {
	$payment_node = node_load($form_state['payment_nid']);
	$json = sirius_jsonfield_util_node_json_get($payment_node);
	$json['transmittal'] = $form_state['values']['transmittal'];
	$json['transmittal_calcs'] = $form_state['storage'];
	unset($json['allocate']);
	sirius_jsonfield_util_node_json_set($payment_node, $json);
	node_save($payment_node);

	sirius_ledger_payment_clear($payment_node->nid);
  $memo = sirius_ledger_payment_memo($payment_node);
	$ledger_ts = strtotime($form_state['values']['transmittal']['ledger_date']);

	foreach ($form_state['values']['transmittal']['dispatches'] as $dispatch_nid => $transmittal) {
		$payment = $transmittal['payment'];
		if (!$payment) { continue; }
		if ($dispatch_nid > 0) {
			$worker_nid = $transmittal['worker_nid'];
			$contact_nid = $transmittal['contact_nid'];
		} else {
			$worker_nid = $form_state['storage']['dispatches'][$dispatch_nid]['worker_nid'];
			$contact_nid = $form_state['storage']['dispatches'][$dispatch_nid]['contact_nid'];
		}

	  $entry = array();
	  $entry['ledger_amount'] = -1 * $payment;
	  $entry['ledger_account'] = $payment_node->field_sirius_ledger_account['und'][0]['target_id'];
	  $entry['ledger_status'] = $payment_node->field_sirius_payment_status['und'][0]['value'];
	  $entry['ledger_memo'] = $memo;
	  $entry['ledger_ts'] = $ledger_ts;
	  $entry['ledger_reference'] = $payment_node->nid;
	  $entry['ledger_participant'] = $contact_nid;
		sirius_ledger_ar_insert($entry);
	}

  drupal_set_message("Allocation saved.");
}

