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

