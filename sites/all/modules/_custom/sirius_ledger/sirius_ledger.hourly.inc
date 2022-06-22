<?php

function sirius_ledger_node_tab_payperiod_test($payperiod_node) {
	return drupal_get_form('sirius_ledger_node_tab_payperiod_test_form', $payperiod_node);
}

function sirius_ledger_node_tab_payperiod_test_form($form, &$form_state, $payperiod_node) {
	$form_state['payperiod_nid'] = $payperiod_node->nid;

	$form['mode'] = array(
		'#type' => 'select',
		'#title' => t('Mode'),
		'#required' => TRUE,
		'#options' => array(
			'test' => t('Test mode - execute all the steps but do not save any charges.'),
			'live' => t('Live mode - create or update A/R entries'),
		),
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

  $log = sirius_minilog_render_html();
  if ($log) { $form['results'] = array('#markup' => '<hr>' . $log); }

	return $form;
}

function sirius_ledger_node_tab_payperiod_test_form_submit($form, &$form_state) {
	$payperiod_node = node_load($form_state['payperiod_nid']);
	$form_state['rebuild'] = TRUE;

	sirius_ledger_payperiod_set($payperiod_node, $form_state['values']['mode']);
	drupal_set_message("Payperiod ledger entries updated.");
}

function sirius_ledger_payperiod_clear($payperiod_node, $mode) {
	sirius_minilog("Payperiod " . $payperiod_node->title . ' (' . $payperiod_node->nid . '): ' . 'Cleared');
	if ($mode == 'live') {
		sirius_ledger_payment_clear($payperiod_node->nid);
	} else {
		sirius_minilog("(test mode, skipping delete.");
	}
}

function sirius_ledger_payperiod_set($payperiod_node, $mode, $options = array()) {
	sirius_minilog("Payperiod " . l($payperiod_node->title, 'node/' . $payperiod_node->nid) . ' (' . $payperiod_node->nid . '): ' . "Calculating ledger (mode = $mode)");
	$schedule = 'hour';

	// Clear the cache
	sirius_ledger_payperiod_clear($payperiod_node, $mode);

	// If not final, don't write the cache
	if ($payperiod_node->field_sirius_active['und'][0]['value'] != 'Yes') { 
		sirius_minilog("Payperiod " . $payperiod_node->title . ' (' . $payperiod_node->nid . '): ' . 'Payperiod is not active, no entries written');
		return 0;
	}

	// Get JSON data
	$payperiod_json = sirius_jsonfield_util_node_json_get($payperiod_node);
	$payperiod_nid = $payperiod_node->nid;
	$employer_nid = $payperiod_node->field_grievance_shop['und'][0]['target_id'];
	$employer_node = node_load($employer_nid);
	$worker_nid = $payperiod_node->field_sirius_worker['und'][0]['target_id'];
	$worker_node = node_load($worker_nid);
	$contact_nid = $worker_node->field_sirius_contact['und'][0]['target_id'];
	$account_lookup = sirius_ledger_account_list();

	if (!$payperiod_json['totals']['hours']['by_ms_level']) {
		sirius_minilog("Payperiod " . $payperiod_node->title . ' (' . $payperiod_node->nid . '): ' . 'No totals found by member status and level');
		return 0;
	}

	// And now, also write the ledger
	if ($options['policy_node']) {
		$policy_node = $options['policy_node'];
		sirius_minilog("Forcing policy " . $policy_node->title . ". This may not be the current policy for this worker.", 'warning');
	} else {
		$policy_node = sirius_ledger_policy_lookup($worker_node, $employer_node);
	}
	if (!$policy_node) {
		sirius_minilog("Payperiod " . $payperiod_node->title . ' (' . $payperiod_node->nid . '): ' . 'No ledger policy found', 'warning');
		return 0;
	}
	sirius_minilog("Using policy " . l($policy_node->title, 'node/' . $poicy_node->nid));
	$policy_json = sirius_jsonfield_util_node_json_get($policy_node);


	$ledger_ts = sirius_fieldval($payperiod_node, 'field_sirius_datetime', 'ts');

	$rates = sirius_ledger_policy_lookup_rates_new($worker_node, $policy_node, $schedule, array('ts' => $ledger_ts));
	// $rates = array('2402505' => array('rate' => 13, 'triggers' => array('hour' => TRUE)));

	$total_charged = 0;
	foreach ($payperiod_json['totals']['hours']['by_ms_level'] as $ms_level => $qty) {
		sirius_minilog("Payperiod " . $payperiod_node->title . ' (' . $payperiod_node->nid . '): ' . "$qty hours at ms $ms_level: Creating charge.");
		if (!$qty) { continue; }
		list ($ms_tid, $level_code) = explode('|', $ms_level);

		if (!$rates) {
			sirius_minilog("Payperiod " . $payperiod_node->title . ' (' . $payperiod_node->nid . '): ' . "$qty hours at ms $ms_level: No rate record found.");
			continue;
		}

		foreach ($rates as $account_nid => $account_record) {
			if ($account_nid == 'pay') { continue; }

			$account_label = $account_lookup[$account_nid] . ' (' . $account_nid . ')';
			$rate = $account_record['rate'];
			if (!$rate) {
				sirius_minilog("Payperiod " . $payperiod_node->title . ' (' . $payperiod_node->nid . '): ' . "$qty hours at ms $ms_level: Account $account_label: Rate is 0.");
				continue;
			}

			if (!$account_record['triggers'][$schedule]) { 
				sirius_minilog("Payperiod " . $payperiod_node->title . ' (' . $payperiod_node->nid . '): ' . "$qty hours at ms $ms_level: Account $account_label: Not triggered on hour");
				continue;
			}

			$amt = $qty * $rate;
		  $memo = "Payperiod: $qty hours at $rate";
		  $entry = array();
		  $entry['ledger_amount'] = $amt;
		  $entry['ledger_account'] = $account_nid;
		  $entry['ledger_status'] = 'Cleared';
		  $entry['ledger_memo'] = $memo;
		  $entry['ledger_ts'] = $ledger_ts;
		  $entry['ledger_reference'] = $payperiod_node->nid;
		  $entry['ledger_participant'] = $contact_nid;
		  $entry['ledger_key'] = "$contact_nid|" . $payperiod_node->nid . "|$ms_level";
			sirius_minilog("Payperiod " . $payperiod_node->title . ' (' . $payperiod_node->nid . '): ' . "$qty hours at ms $ms_level: Account $account_label Charged $qty * $rate = $amt");
		  if ($mode == 'live') {
				sirius_ledger_ar_insert($entry);
			} else {
				sirius_minilog("(Test mode, skipping insert)");
			}
			$total_charged += $amt;
		}
	}

	return $total_charged;
}