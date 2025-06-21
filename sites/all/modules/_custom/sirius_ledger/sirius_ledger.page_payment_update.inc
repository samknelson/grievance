<?php


function sirius_ledger_node_tab_payment_update_form($form, &$form_state, $payment_node) {
	$ea = Sirius_Ledger_EA::getInstance();

	$form_state['payment_nid'] = $payment_node->nid;

	$form['#tree'] = TRUE;

	$payment_types = sirius_taxonomy_load('sirius_payment_type', 'tid', 'name');
  $payment_type_tid = $payment_node->field_sirius_payment_type['und'][0]['tid'];
  $payment_type_term = taxonomy_term_load($payment_type_tid);

  $amt = $payment_node->field_sirius_dollar_amt['und'][0]['value'];
  $display_amt = $amt;
  if ($display_amt < 0) { $display_amt *= -1; }

  $form['payment']['display_amt'] = array(
    '#title' => t('Amount'),
		'#type' => 'textfield',
		'#required' => TRUE,
		'#size' => 5,
		'#default_value' => $display_amt,
		'#description' => $description,
  );

  $form['payment']['merchant_name'] = array(
    '#title' => t('Merchant'),
		'#type' => 'textfield',
		'#default_value' => $payment_node->field_sirius_merchant_name['und'][0]['value'],
  );

  $form['payment']['check_number'] = array(
    '#title' => t('Check / Txn #'),
		'#type' => 'textfield',
		'#default_value' => $payment_node->field_sirius_check_number['und'][0]['value'],
  );

  $form['payment']['notes'] = array(
    '#title' => t('Memo'),
		'#type' => 'textfield',
		'#default_value' => $payment_node->field_sirius_notes['und'][0]['value'],
  );

	$form['payment']['payment_status'] = array(
		'#title' => t('Payment Status'),
		'#type' => 'select',
		'#options' => sirius_ledger_payment_status_options(),
		'#required' => TRUE,
		'#default_value' => $payment_node->field_sirius_payment_status['und'][0]['value'],
	);

	$form['payment']['allocate'] = array(
		'#title' => t('Auto-Allocate?'),
		'#type' => 'select',
		'#options' => array(
			'Yes' => t('Yes, attempt to auto-allocate this payment'),
			'No' => t('No, this payment will be allocated manually'),
		),
		'#description' => t('Generally simple payments (i.e. a check or adjustment to a member) can be auto-allocated. Complex payments, such as a check from an employer intended to cover many workers, must be allocated manually')
	);
	if ($payment_node && sirius_ledger_node_tab_payment_allocate_simple_access($payment_node)) {
		$form['payment']['allocate']['#default_value'] = 'Yes';
	}

  $form['payment']['datetime_created'] = array(
    '#title' => t('Received'),
		'#type' => 'date_popup',
		'#default_value' => NULL,
		'#attributes' => array('autocomplete' => 'off'),
  );
  if ($payment_node->field_sirius_datetime_created['und'][0]['value']) {
  	$ts = sirius_fieldval($payment_node, 'field_sirius_datetime_created', 'ts');
  	if ($ts > 1) { $form['payment']['datetime_created']['#default_value'] = date('Y-m-d H:i:s', $ts); }
  }

  $form['payment']['datetime'] = array(
    '#title' => t('Posted'),
		'#type' => 'date_popup',
		'#required' => FALSE,
		'#default_value' => NULL,
		'#attributes' => array('autocomplete' => 'off'),
  );
  if ($payment_node->field_sirius_datetime['und'][0]['value']) {
  	$ts = sirius_fieldval($payment_node, 'field_sirius_datetime', 'ts');
  	if ($ts > 1) { $form['payment']['datetime']['#default_value'] = date('Y-m-d H:i:s', $ts); }
  }

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_ledger_node_tab_payment_update_form_validate($form, &$form_state) {
	$payment_node = node_load($form_state['payment_nid']);
  $payment_type_tid = $payment_node->field_sirius_payment_type['und'][0]['tid'];
	$payment_type_term = taxonomy_term_load($payment_type_tid);
	$payment_type_json = sirius_jsonfield_util_term_json_get($payment_type_term);
	$sign = $payment_type_json["payment_settings"]["sign"];
	$max = $payment_type_json["payment_settings"]["max"];
	$min = $payment_type_json["payment_settings"]["min"];

	$display_amt = $form_state['values']['payment']['display_amt'];
	if (!is_numeric($display_amt)) {
		form_set_error('payment][display_amt', 'Please enter a number.');
		return;
	}

	if ($display_amt < 0) {
		form_set_error('payment][display_amt', 'Amount should always be positive.');
		return;
	}
	if ($max && ($display_amt > $max)) { 
		form_set_error('payment][display_amt', 'Entries of type ' . $payment_type_term->name . ' must have a value less than ' . number_format($max, 2));
		return;
	}
	if ($min && ($display_amt < $min)) { 
		form_set_error('payment][display_amt', 'Entries of type ' . $payment_type_term->name . ' must have a value greater than ' . number_format($min, 2));
		return;
	}
}

function sirius_ledger_node_tab_payment_update_form_submit($form, &$form_state) {
	$payment_node = node_load($form_state['payment_nid']);

	$payment_node = node_load($form_state['payment_nid']);
  $payment_type_tid = $payment_node->field_sirius_payment_type['und'][0]['tid'];
	$payment_type_term = taxonomy_term_load($payment_type_tid);
	$payment_type_json = sirius_json_get($payment_type_term);
	$sign = $payment_type_json["payment_settings"]["sign"];

	$form_state['values']['payment']['dollar_amt'] = $form_state['values']['payment']['display_amt'];
	if ($sign == 'payment' && $form_state['values']['payment']['dollar_amt'] > 0) {
		$form_state['values']['payment']['dollar_amt'] *= -1;
	}

	$result = sirius_ledger_payment_update($payment_node, $form_state['values']['payment']);
	if ($result['success']) {
		drupal_set_message($result['msg']);
	} else {
		drupal_set_message($result['msg'], 'error');
	}
}
