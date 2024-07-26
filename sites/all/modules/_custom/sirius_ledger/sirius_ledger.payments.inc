<?php

function sirius_ledger_payment_status_options() {
	return array(
		'Pending' => 'Pending',
		'Cleared' => 'Cleared',
		'Canceled' => 'Canceled',
	);
}

function sirius_ledger_payment_status_color($status) {
	if ($status == 'Pending') { return 'grey'; }
	if ($status == 'Received') { return 'grey'; }
	if ($status == 'Cleared') { return 'green'; }
	if ($status == 'Canceled') { return 'red'; }
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
	if ($payment_node->field_sirius_notes['und'][0]['value']) { $parts[] = $payment_node->field_sirius_notes['und'][0]['value']; }
	$memo = join(' / ', $parts);
	$memo = substr($memo, 0, 255);
	return $memo;
}

function sirius_ledger_payment_allocate_simple($payment_node) {
	sirius_ledger_payment_clear($payment_node->nid);

	if ($payment_node->field_sirius_datetime['und'][0]['value']) {
		$ts = sirius_fieldval($payment_node, 'field_sirius_datetime', 'ts');
	} else {
		$ts = time(); 
	}

	$entry = array();
	$entry['ledger_amount'] = $payment_node->field_sirius_dollar_amt['und'][0]['value'];
	$entry['ledger_account'] = $payment_node->field_sirius_ledger_account['und'][0]['target_id'];
	$entry['ledger_status'] = $payment_node->field_sirius_payment_status['und'][0]['value'];
	$payment_type_term = taxonomy_term_load($payment_node->field_sirius_payment_type['und'][0]['tid']);
	$payer_node = node_load($payment_node->field_sirius_payer['und'][0]['target_id']);
	$entry['ledger_memo'] = sirius_ledger_payment_memo($payment_node);
	$entry['ledger_ts'] = $ts;
	$entry['ledger_reference'] = $payment_node->nid;
	$entry['ledger_participant'] = $payer_node->nid;

	sirius_ledger_ar_insert($entry);

	$payment_node->field_sirius_ledger_allocated['und'][0]['value'] = 'Yes';
	node_save($payment_node);
}

function sirius_ledger_payment_create($params) {
	global $user;
	$domain_nid = sirius_domain_nid();

	if (!$params['ledger_account']) { return array('success' => FALSE, 'msg' => 'No account specified.'); }
	if (!$params['ledger_participant']) { return array('success' => FALSE, 'msg' => 'No payer specified.'); }
	if (!$params['payment_type']) { return array('success' => FALSE, 'msg' => 'NO payment type specified.'); }

	$payment_node = new stdClass();
	$payment_node->type = 'sirius_payment';
	$payment_node->no_recurse = TRUE;
	$payment_node->language = $user->language;
	$payment_node->uid = $user->uid;
	$payment_node->status = 1;
	$payment_node->promote = 0;
	$payment_node->comment = 0;

	if ($domain_nid) { $payment_node->field_sirius_domain['und'][0]['target_id'] = $domain_nid; }
	$payment_node->field_sirius_ledger_account['und'][0]['target_id'] = $params['ledger_account'];
	$payment_node->field_sirius_payer['und'][0]['target_id'] = $params['ledger_participant'];
	$payment_node->field_sirius_payment_type['und'][0]['tid'] = $params['payment_type'];

	$payment_node->field_sirius_payment_status['und'][0]['value'] = 'Received';
	$payment_node->field_sirius_ledger_allocated['und'][0]['value'] = 'No';

	if ($params['json']) { sirius_json_set($payment_node, $params['json']); }

	$payment_node->field_sirius_datetime_created['und'][0]['value'] = gmdate('Y-m-d H:i:s');

	sirius_ledger_payment_set_title($payment_node);

	node_save($payment_node);

	return array('success' => TRUE, 'msg' => 'Payment created', 'payment_node' => $payment_node);
}

function sirius_ledger_payment_save($defunct) {
	drupal_set_message("sirius_ledger_payment_save no longer implemented.", 'error');
	return;
}

function sirius_ledger_payment_update($payment_node, $params) {
	$msgs = array();

	// Update the payment node.
	$payment_node->field_sirius_dollar_amt['und'][0]['value'] = $params['dollar_amt'];
	$payment_node->field_sirius_merchant_name['und'][0]['value'] = $params['merchant_name'];
	$payment_node->field_sirius_check_number['und'][0]['value'] = $params['check_number'];
	$payment_node->field_sirius_notes['und'][0]['value'] = $params['notes'];
	$payment_node->field_sirius_payment_status['und'][0]['value'] = $params['payment_status'];
	$payment_node->field_sirius_id['und'][0]['value'] = $params['id'];

	if ($params['json']) { sirius_json_set($payment_node, $params['json']); }

	if ($params['datetime_created']) {
		$payment_node->field_sirius_datetime_created['und'][0]['value'] = gmdate('Y-m-d H:i:s', strtotime($params['datetime_created']));
	} else {
		unset($payment_node->field_sirius_datetime_created['und']);
	}

	if ($params['datetime']) {
		$payment_node->field_sirius_datetime['und'][0]['value'] = gmdate('Y-m-d H:i:s', strtotime($params['datetime']));
	} else {
		unset($payment_node->field_sirius_datetime['und']);
	}

	// We always set allocation to "No" here. A simple allocation may set it to "Yes" later.
	$payment_node->field_sirius_ledger_allocated['und'][0]['value'] = 'No';
	sirius_ledger_payment_set_title($payment_node);
	node_save($payment_node);
	$msgs[] = "Payment updated.";

	// Update the allocation. After updating a payment, we ALWAYS re-allocate, since we don't know what might have changed.
	// (@todo: If we were in the mood, we could say that if nothing changes, we don't need to re-allocate. But remember that
	// e.g. changing a transaction note or a memo will affect all the allocated entries, so we'll pretty much always have to
	// re-allocate after saving.)

	if ($payment_node->field_sirius_payment_status['und'][0]['value'] != 'Cleared') {

		// Terrible terminology, but ... if the payment is canceled or pending (i.e. not "Cleared") then we 
		// need to erase ("clear") all the ledger entries.
		$msgs[] = "The payment is NOT allocated.";
		sirius_ledger_payment_clear($payment_node->nid);

	} else {

		// The payment is final. We can try to allocate it.
		if ($params['allocate'] == 'Yes') {
			if (sirius_ledger_node_tab_payment_allocate_simple_access($payment_node)) { 
				sirius_ledger_payment_allocate_simple($payment_node);
				$msgs[] = "The payment has been allocated.";
			} else {
				sirius_ledger_payment_clear($payment_node->nid);
				$msgs[] = "Simple allocation is not available for this payment. The payment must be allocated manually.";
			}
		}

	}

	return array('success' => TRUE, 'msg' => join('<br />', $msgs), 'payment_node' => $payment_node);
}

function sirius_ledger_payments_render($payment_nodes, $options = array()) {
	if (!$payment_nodes) {
		return '<div class="sirius_ledger_payments_not_found">' . t('No payments found.') . '</div>';
	}

	$access = Sirius_Ledger_Access::getInstance();

	$rows = array();
	$account_options = sirius_ledger_account_list();
	$status_options = sirius_ledger_payment_status_options();
	$has_tools = FALSE;
	foreach ($payment_nodes as $payment_nid => $payment_node) {
		$row = array();

		$tools = array();
		if ($access->access('payment_view', array('payment_node' => $payment_node))) {
			$tools[] = l('View', 'node/' . $payment_node->nid . '/sirius_payment_allocate', array('attributes' => array('class' => array('tool-button'))));
			$has_tools = TRUE;
		}

		$currency = sirius_ledger_currency_from_account_nid($payment_node->field_sirius_ledger_account['und'][0]['target_id']);

		$color = sirius_ledger_payment_status_color($payment_node->field_sirius_payment_status['und'][0]['value']);

		if (!$options['hide_account']) {
			$row[] = $account_options[$payment_node->field_sirius_ledger_account['und'][0]['target_id']];
		}
		$row[] = sirius_term_title($payment_node->field_sirius_payment_type['und'][0]['tid']);
		$row[] = $status_options[$payment_node->field_sirius_payment_status['und'][0]['value']];
		$row[] = $currency->render_amt(-1 * $payment_node->field_sirius_dollar_amt['und'][0]['value']);
		$row[] = $payment_node->field_sirius_ledger_allocated['und'][0]['value'];
		$row[] = $payment_node->field_sirius_notes['und'][0]['value'];
		$row[] = join('', $tools);
		$rows[] = array('data' => $row, 'class' => array($color));
	}

	$header = array();
	if (!$options['hide_account']) { $header[] = t('Account'); }
	$header[] = t('Type');
	$header[] = t('Status');
	$header[] = t('Amount');
	$header[] = t('Allocated?');
	$header[] = t('Memo');

	if ($has_tools) { $header[] = t('Tools'); }

	return sirius_table_render($header, $rows);
}

function sirius_ledger_payments_search($params) {
	$domain_nid = sirius_domain_nid();

	$sql_args = array();
	$sql = "select node.nid from node ";
	$sql .= "left join field_data_field_sirius_payer on field_data_field_sirius_payer.entity_type = 'node' and field_data_field_sirius_payer.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_ledger_account on field_data_field_sirius_ledger_account.entity_type = 'node' and field_data_field_sirius_ledger_account.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_payment_type on field_data_field_sirius_payment_type.entity_type = 'node' and field_data_field_sirius_payment_type.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_dollar_amt on field_data_field_sirius_dollar_amt.entity_type = 'node' and field_data_field_sirius_dollar_amt.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_payment_status on field_data_field_sirius_payment_status.entity_type = 'node' and field_data_field_sirius_payment_status.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_ledger_allocated on field_data_field_sirius_ledger_allocated.entity_type = 'node' and field_data_field_sirius_ledger_allocated.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_datetime_created on field_data_field_sirius_datetime_created.entity_type = 'node' and field_data_field_sirius_datetime_created.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_datetime on field_data_field_sirius_datetime.entity_type = 'node' and field_data_field_sirius_datetime.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_id on field_data_field_sirius_id.entity_type = 'node' and field_data_field_sirius_id.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";

	$sql .= "where node.type = 'sirius_payment' ";

	$sql .= "and field_sirius_domain_target_id = :domain_nid ";
	$sql_args[':domain_nid'] = $domain_nid;

  if ($params['payer_nid']) {
  	$sql .= "and field_sirius_payer_target_id = :payer_nid ";
  	$sql_args[':payer_nid'] = $params['payer_nid'];
  }

  if ($params['account_nid']) {
  	$sql .= "and field_sirius_ledger_account_target_id = :account_nid ";
  	$sql_args[':account_nid'] = $params['account_nid'];
  }

  $sql .= "order by field_sirius_datetime_value desc ";

  $offset = $params['offset'];
  if (!$offset) { $offset = 0; }
  $limit = $params['limit'];
  if (!$limit) { $limit = 100; }
  $sql .= "limit $limit offset $offset ";

  $stmt = sirius_sql_query($sql, $sql_args);
  $election_nids = array();
  while ($hr = $stmt->fetchAssoc()) {
  	$election_nids[] = $hr['nid'];
  }

  if (!$election_nids) { return array(); }
  
  if ($params['nids_only']) { return $election_nids; }
	return node_load_multiple($election_nids);
}
