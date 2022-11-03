<?php

function sirius_ledger_payment_status_options() {
	return array(
		'Received' => 'Received',
		'Cleared' => 'Cleared',
		'Canceled' => 'Canceled',
	);
}

function sirius_ledger_payment_set_title(&$payment_node) {
	$from_nid = $payment_node->field_sirius_payer['und'][0]['target_id'];
	$from_node = node_load($from_nid);

	$to_nid = $payment_node->field_sirius_ledger_account['und'][0]['target_id'];
	$to_node = node_load($to_nid);

	$type_term = taxonomy_term_load($payment_node->field_sirius_payment_type['und'][0]['tid']);

	$title = $type_term->name;
	$title .= ' to '; 
	if ($to_node->field_sirius_name_short['und'][0]['value']) { 
		$title .= $to_node->field_sirius_name_short['und'][0]['value'];
	} else {
		$title .= $to_node->title;
	}
	
	$ts = sirius_fieldval($payment_node, 'field_sirius_datetime_created', 'ts');
	if ($ts > 1) {
		$title .= ' rcvd ';
		$title .= date('Y-m-d', $ts);
	}
	$title .= ' from ';
	$title .= substr($from_node->title, 0, 50);

	$title = substr($title, 0, 250);
	$payment_node->title = $title;
}

function sirius_ledger_payer_type($payer_node_type) {
	if ($payer_node_type == 'grievance_shop') { return 'employer'; }
	if ($payer_node_type == 'sirius_contact') { return 'worker'; }
	return 'unknown';
}

function sirius_ledger_payment_clear($payment_nid) {
	return sirius_ledger_ar_delete_by_reference($payment_nid);
}

function sirius_ledger_payment_memo($payment_node) {
	$payment_type_term = taxonomy_term_load($payment_node->field_sirius_payment_type['und'][0]['tid']);
	$payer_node = node_load($payment_node->field_sirius_payer['und'][0]['target_id']);

	$parts = array();
	if ($payment_type_term) { $parts[] = $payment_type_term->name; }
	if ($payer_node) { $parts[] = $payer_node->title; }
	if ($payment_node->field_sirius_check_number['und'][0]['value']) { $parts[] = $payment_node->field_sirius_check_number['und'][0]['value']; }
	if ($payment_node->field_sirius_merchant_name['und'][0]['value']) { $parts[] = $payment_node->field_sirius_merchant_name['und'][0]['value']; }
	$memo = join(' / ', $parts);
	$memo = substr($memo, 0, 255);
	return $memo;
}

function sirius_ledger_payment_allocate_simple($payment_node) {
	sirius_ledger_payment_clear($payment_node->nid);

	$entry = array();
	$entry['ledger_amount'] = $payment_node->field_sirius_dollar_amt['und'][0]['value'];
	$entry['ledger_account'] = $payment_node->field_sirius_ledger_account['und'][0]['target_id'];
	$entry['ledger_status'] = $payment_node->field_sirius_payment_status['und'][0]['value'];
	$payment_type_term = taxonomy_term_load($payment_node->field_sirius_payment_type['und'][0]['tid']);
	$payer_node = node_load($payment_node->field_sirius_payer['und'][0]['target_id']);
	$entry['ledger_memo'] = sirius_ledger_payment_memo($payment_node);
	$entry['ledger_ts'] = sirius_fieldval($payment_node, 'field_sirius_datetime', 'ts');
	$entry['ledger_reference'] = $payment_node->nid;
	$entry['ledger_participant'] = $payer_node->nid;

	sirius_ledger_ar_insert($entry);

	$payment_node->field_sirius_ledger_allocated['und'][0]['value'] = 'Yes';
	$payment_node->revision;
	node_save($payment_node);
}

function sirius_ledger_payment_save($form_state) {
	global $user;
	$domain_nid = sirius_domain_nid();

	$msgs = array();

	if ($form_state['payment_nid']) {
		$payment_node = node_load($form_state['payment_nid']);

		// Clear the ledger if the amount has changed
		if ($form_state['values']['payment']['dollar_amt'] != $payment_node->field_sirius_dollar_amt['und'][0]['value']) {
			$count = sirius_ledger_payment_clear($payment_node->nid);
			if ($count) { $msgs[] = "$count A/R entries have been cleared. This payment must be re-allocated."; }
		}
	} else {
		$payment_node = new stdClass();
		$payment_node->type = 'sirius_payment';
		$payment_node->no_recurse = TRUE;
		$payment_node->language = $user->language;
		$payment_node->uid = $user->uid;
		$payment_node->status = 1;
		$payment_node->promote = 0;
		$payment_node->comment = 0;
		if ($domain_nid) { $payment_node->field_sirius_domain['und'][0]['target_id'] = $domain_nid; }
	}

	$payment_node->field_sirius_ledger_allocated['und'][0]['value'] = 'No';
	$payment_node->field_sirius_ledger_account['und'][0]['target_id'] = $form_state['values']['payment']['ledger_account'];
	$payment_node->field_sirius_payment_type['und'][0]['tid'] = $form_state['values']['payment']['payment_type'];
	$payment_node->field_sirius_dollar_amt['und'][0]['value'] = $form_state['values']['payment']['dollar_amt'];
	$payment_node->field_sirius_merchant_name['und'][0]['value'] = $form_state['values']['payment']['merchant_name'];
	$payment_node->field_sirius_check_number['und'][0]['value'] = $form_state['values']['payment']['check_number'];
	$payment_node->field_sirius_notes['und'][0]['value'] = $form_state['values']['payment']['notes'];
	$payment_node->field_sirius_payment_status['und'][0]['value'] = $form_state['values']['payment']['payment_status'];
	$payment_node->field_sirius_id['und'][0]['value'] = $form_state['values']['payment']['id'];

	if ($form_state['values']['payment']['json']) {
		sirius_json_set($payment_node, $form_state['values']['payment']['json']);
	}

	if ($form_state['values']['payment']['payer_type'] == 'employer') {
		$payment_node->field_sirius_payer['und'][0]['target_id'] = $form_state['values']['payment']['payer_employer'];
	} else {
		$worker_node = sirius_worker_get_by_id($form_state['values']['payment']['payer_worker']);
		if (!$worker_node) { return array('success' => FALSE, 'msg' => "Worker not found."); }
		$payment_node->field_sirius_payer['und'][0]['target_id'] = $worker_node->field_sirius_contact['und'][0]['target_id'];
	}

	if ($form_state['values']['payment']['datetime_created']) {
		$payment_node->field_sirius_datetime_created['und'][0]['value'] = gmdate('Y-m-d H:i:s', strtotime($form_state['values']['payment']['datetime_created']));
	} else {
		unset($payment_node->field_sirius_datetime_created['und']);
	}

	if ($form_state['values']['payment']['datetime']) {
		$payment_node->field_sirius_datetime['und'][0]['value'] = gmdate('Y-m-d H:i:s', strtotime($form_state['values']['payment']['datetime']));
	} else {
		unset($payment_node->field_sirius_datetime['und']);
	}
	
	sirius_ledger_payment_set_title($payment_node);

	node_save($payment_node);
	$msgs[] = "Payment updated.";

	$allocated = FALSE;
	if ($form_state['values']['payment']['allocate'] == 'Yes') {
		if (sirius_ledger_node_tab_payment_allocate_simple_access($payment_node)) { 
			sirius_ledger_payment_allocate_simple($payment_node);
			$msgs[] = "The payment has been allocated.";
			$allocated = TRUE;
		} else {
			$msgs[] = "Simple allocation is not available for this payment. The payment must be allocated manually.";
		}
	}

	if (!$allocated) {
		$entries = sirius_ledger_ar_fetch(array('ledger_reference' => $payment_node->nid));
		$count = 0;
		$ts = sirius_fieldval($payment_node, 'field_sirius_datetime_created', 'ts');
		foreach ($entries as $entry) {
			++$count;
			$entry['ledger_status'] = $form_state['values']['payment']['payment_status'];
			$entry['ledger_ts'] = $ts;
			sirius_ledger_ar_update($entry);
		}
		if ($count) { $msgs[] = "$count A/R entries updated."; }
	}

	return array('success' => TRUE, 'msg' => join('<br />', $msgs), 'payment_node' => $payment_node);
}