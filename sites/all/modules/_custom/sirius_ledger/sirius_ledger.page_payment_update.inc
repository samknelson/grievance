<?php

function sirius_ledger_node_tab_payment_update($payment_node) {
	return drupal_get_form('sirius_ledger_node_tab_payment_addedit_form', $payment_node);
}

function sirius_ledger_node_tab_payment_addedit_form($form, &$form_state, $payment_node = NULL) {
	if ($payment_node) {
		$form_state['payment_nid'] = $payment_node->nid;
	}

	$form['#tree'] = TRUE;

	$payer_node = node_load($payment_node->field_sirius_payer['und'][0]['target_id']);
	$worker_node = sirius_worker_get_by_contact_nid($payer_node->nid);
	$employers = sirius_employer_list(FALSE);
	$form['payment']['payer_type'] = array(
		'#title' => t('Payment From'),
		'#type' => 'radios',
		'#options' => array(
			'worker' => 'Worker',
			'employer' => 'Employer',
		),
		'#required' => TRUE,
		'#default_value' => sirius_ledger_payer_type($payer_node->type),
	);
	$form['payment']['payer_employer'] = array(
		'#type' => 'select',
		'#title' => t('Employer'),
		'#options' => $employers,
		'#default_value' => $payer_node->nid,
		'#states' => array(
			'visible' => array(
				":input[name=\"payment[payer_type]\"]" => array('value' => 'employer')
			),
		),
	);
	$form['payment']['payer_worker'] = array(
		'#type' => 'textfield',
		'#title' => t('Worker ID'),
		'#default_value' => $worker_node->field_sirius_id['und'][0]['value'],
		'#states' => array(
			'visible' => array(
				":input[name=\"payment[payer_type]\"]" => array('value' => 'worker')
			),
		),
	);

	if (!$payment_node) {
		$default_payer_worker = $_REQUEST['payer_worker'];
		if ($default_payer_worker) {
			$form['payment']['payer_type']['#default_value'] = 'worker';
			$form['payment']['payer_worker']['#default_value'] = $default_payer_worker;
		}

		$default_payer_employer = $_REQUEST['payer_employer'];
		if ($default_payer_employer) {
			$form['payment']['payer_type']['#default_value'] = 'employer';
			$form['payment']['payer_employer']['#default_value'] = $default_payer_employer;
		}
	}

	$accounts = sirius_ledger_account_list();
	$form['payment']['ledger_account'] = array(
		'#title' => t('To'),
		'#type' => 'select',
		'#required' => TRUE,
		'#options' => $accounts,
		'#default_value' => $payment_node->field_sirius_ledger_account['und'][0]['target_id'],
	);

	$payment_types = sirius_taxonomy_load('sirius_payment_type', 'tid', 'name');
  $form['payment']['payment_type'] = array(
    '#title' => t('Type'),
		'#type' => 'select',
		'#options' => $payment_types,
		'#required' => TRUE,
		'#default_value' => $payment_node->field_sirius_payment_type['und'][0]['tid'],
		'#ajax' => array(
			'callback' => 'sirius_ledger_page_payment_addedit_form_ajax_change_payment_type',
			'wrapper' => 'sirius_ledger_page_payment_addedits_form_ajax_dollar_amt',
			'method' => 'replace',
		),
  );

  $description = '';
  $payment_type_tid = $form_state['values']['payment']['payment_type'];
  if (!$payment_type_tid) { $payment_type_tid = $payment_node->field_sirius_payment_type['und'][0]['tid']; }
  if ($payment_type_tid) {
  	$descriptions = array(
			'payment' => t('Entering a payment. The value must be negative.'),
			'charge' => t('Entering a charge. The value must be positive.'),
			'adjustment' => t('Entering an adjustment. The value may be positive (a charge, or credit to the fund) or negative (a payment, or credit to the payer).'),
		);
  	$payment_type_term = taxonomy_term_load($payment_type_tid);
  	$payment_type_json = sirius_jsonfield_util_term_json_get($payment_type_term);
  	$sign = $payment_type_json["payment_settings"]["sign"];
  	$description = $descriptions[$sign];
  }

  $form['payment']['dollar_amt'] = array(
    '#title' => t('Amount'),
		'#type' => 'textfield',
		'#required' => TRUE,
		'#size' => 5,
		'#default_value' => $payment_node->field_sirius_dollar_amt['und'][0]['value'],
		'#prefix' => '<div id="sirius_ledger_page_payment_addedit_form_ajax_dollar_amt">',
		'#suffix' => '</div>',
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
		'#required' => TRUE,
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

function sirius_ledger_node_tab_payment_addedit_form_validate($form, &$form_state) {
  $payment_type_tid = $form_state['values']['payment']['payment_type'];
	$payment_type_term = taxonomy_term_load($payment_type_tid);
	$payment_type_json = sirius_jsonfield_util_term_json_get($payment_type_term);
	$sign = $payment_type_json["payment_settings"]["sign"];
	$max = $payment_type_json["payment_settings"]["max"];
	$min = $payment_type_json["payment_settings"]["min"];

	$amt = $form_state['values']['payment']['dollar_amt'];
	if ($sign == 'payment' && $amt > 0) {
		form_set_error('payment][dollar_amt', 'Payments must be negative.');
	}
	if ($sign == 'charge' && $amt < 0) {
		form_set_error('payment][dollar_amt', 'Charges must be positive.');
	}

	if ($max && ($amt > $max)) { 
		form_set_error('payment][dollar_amt', 'Entries of type ' . $payment_type_term->name . ' must have a value less than ' . number_format($max, 2));
	}
	if ($min && ($amt < $min)) { 
		form_set_error('payment][dollar_amt', 'Entries of type ' . $payment_type_term->name . ' must have a value greater than ' . number_format($min, 2));
	}
}

function sirius_ledger_node_tab_payment_addedit_form_submit($form, &$form_state) {
	$result = sirius_ledger_payment_save($form_state);
	if ($result['success']) {
		drupal_set_message($result['msg']);
	} else {
		drupal_set_message($result['msg'], 'error');
	}
}

function sirius_ledger_page_payment_addedit_form_ajax_change_payment_type($form, $form_state) {
	$commands = array();
  $commands[] = ajax_command_replace("#sirius_ledger_page_payment_addedit_form_ajax_dollar_amt", render($form['payment']['dollar_amt']));
	return array('#type' => 'ajax', '#commands' => $commands);
}
