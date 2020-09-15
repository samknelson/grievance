<?php

function sirius_ledger_node_tab_payperiod_test($payperiod_node) {
	return drupal_get_form('sirius_ledger_node_tab_payperiod_test_form', $payperiod_node);
}

function sirius_ledger_node_tab_payperiod_test_form($form, &$form_state, $payperiod_node) {
	$form_state['payperiod_nid'] = $payperiod_node->nid;

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
	node_save($payperiod_node);
	drupal_set_message("Payperiod saved.");
}

function sirius_ledger_payperiod_clear($payperiod_node) {
	sirius_ledger_payment_clear($payperiod_node->nid);
	sirius_minilog("Payperiod " . $payperiod_node->title . ' (' . $payperiod_node->nid . '): ' . 'Cleared');
}

function sirius_ledger_payperiod_set($payperiod_node) {
	sirius_minilog("Payperiod " . $payperiod_node->title . ' (' . $payperiod_node->nid . '): ' . 'Calculating ledger');
	// Clear the cache
	sirius_ledger_payperiod_clear($payperiod_node);

	// If not final, don't write the cache
	if ($payperiod_node->field_sirius_active['und'][0]['value'] != 'Yes') { 
		sirius_minilog("Payperiod " . $payperiod_node->title . ' (' . $payperiod_node->nid . '): ' . 'Payperiod is not active, no entries written');
		return;
	}

	// Get JSON data
	$payperiod_json = sirius_jsonfield_util_node_json_get($payperiod_node);
	$payperiod_nid = $payperiod_node->nid;
	$employer_nid = $payperiod_node->field_grievance_shop['und'][0]['target_id'];
	$employer_node = node_load($employer_nid);
	$worker_nid = $payperiod_node->field_sirius_worker['und'][0]['target_id'];
	$worker_node = node_load($worker_nid);
	$contact_nid = $worker_node->field_sirius_contact['und'][0]['target_id'];

	// And now, also write the ledger
	$policy_node = sirius_ledger_policy_lookup('hour', $worker_node, $employer_node);
	if (!$policy_node) {
		sirius_minilog("Payperiod " . $payperiod_node->title . ' (' . $payperiod_node->nid . '): ' . 'No ledger policy found');
		return;
	}
	$policy_json = sirius_jsonfield_util_node_json_get($policy_node);

	if (!$payperiod_json['totals']['hours']['by_ms_level']) {
		sirius_minilog("Payperiod " . $payperiod_node->title . ' (' . $payperiod_node->nid . '): ' . 'No totals found by member status and level');
		return;
	}

	$ledger_ts = sirius_fieldval($payperiod_node, 'field_sirius_datetime', 'ts');

	foreach ($payperiod_json['totals']['hours']['by_ms_level'] as $ms_level => $qty) {
		sirius_minilog("Payperiod " . $payperiod_node->title . ' (' . $payperiod_node->nid . '): ' . "$qty hours at ms $ms_level: Creating charge.");
		if (!$qty) { continue; }
		list ($ms_tid, $level_code) = explode('|', $ms_level);

		$rate_record = $policy_json['ledger']['policy']['rates']['hour'][$ms_level];

		if (!$rate_record) {
			sirius_minilog("Payperiod " . $payperiod_node->title . ' (' . $payperiod_node->nid . '): ' . "$qty hours at ms $ms_level: No rate record found.");
			continue;
		}

		foreach ($rate_record as $account_nid => $account_record) {
			$rate = $account_record['rate'];
			if (!$rate) {
				sirius_minilog("Payperiod " . $payperiod_node->title . ' (' . $payperiod_node->nid . '): ' . "$qty hours at ms $ms_level: Account $account_nid: Rate is 0.");
				continue;
			}

			if (!$account_record['triggers']['hour']) { 
				sirius_minilog("Payperiod " . $payperiod_node->title . ' (' . $payperiod_node->nid . '): ' . "$qty hours at ms $ms_level: Account $account_nid: Not triggered on hour");
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
			sirius_ledger_ar_insert($entry);
			sirius_minilog("Payperiod " . $payperiod_node->title . ' (' . $payperiod_node->nid . '): ' . "$qty hours at ms $ms_level: Account $account_nid: Charged $qty * $rate = $amt");
		}
	}
}