<?php

function sirius_ledger_page_payment_create() {
	return drupal_get_form('sirius_ledger_node_tab_payment_addedit_form');
}

/*
function sirius_ledger_page_payment_create_form($form, &$form_state) {
	$form['#tree'] = TRUE;

	$employers = sirius_employer_list(FALSE);
	$form['payment']['payer_type'] = array(
		'#title' => t('Payment From'),
		'#type' => 'radios',
		'#options' => array(
			'worker' => 'Worker',
			'employer' => 'Employer',
		),
		'#required' => TRUE,
	);

	$form['payment']['payer_employer'] = array(
		'#type' => 'select',
		'#title' => t('Employer'),
		'#options' => $employers,
		'#states' => array(
			'visible' => array(
				":input[name=\"payment[payer_type]\"]" => array('value' => 'employer')
			),
		),
	);
	$form['payment']['payer_worker'] = array(
		'#type' => 'textfield',
		'#title' => t('Worker ID'),
		'#states' => array(
			'visible' => array(
				":input[name=\"payment[payer_type]\"]" => array('value' => 'worker')
			),
		),
	);

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

	$accounts = sirius_ledger_account_list();
	$form['payment']['ledger_account'] = array(
		'#title' => t('To'),
		'#type' => 'select',
		'#required' => TRUE,
		'#options' => $accounts,
	);

	$payment_types = sirius_taxonomy_load('sirius_payment_type', 'tid', 'name');
  $form['payment']['payment_type'] = array(
    '#title' => t('Type'),
		'#type' => 'select',
		'#options' => $payment_types,
		'#required' => TRUE,
		'#ajax' => array(
			'callback' => 'sirius_ledger_page_payment_create_form_ajax_change_payment_type',
			'wrapper' => 'sirius_ledger_page_payment_create_form_ajax_dollar_amt',
			'method' => 'replace',
		),
  );

  $description = '';
  $payment_type_tid = $form_state['values']['payment']['payment_type'];
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
		'#prefix' => '<div id="sirius_ledger_page_payment_create_form_ajax_dollar_amt">',
		'#suffix' => '</div>',
		'#description' => $description,
  );

  $form['payment']['merchant_name'] = array(
    '#title' => t('Merchant'),
		'#type' => 'textfield',
  );

  $form['payment']['check_number'] = array(
    '#title' => t('Check / Txn #'),
		'#type' => 'textfield',
  );

  $form['payment']['notes'] = array(
    '#title' => t('Memo'),
		'#type' => 'textfield',
  );

  $form['payment']['datetime_created'] = array(
    '#title' => t('Received'),
		'#type' => 'date_popup',
		'#required' => FALSE,
		'#default_value' => NULL,
		'#attributes' => array('autocomplete' => 'off'),
		'#required' => TRUE,
  );

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Create Payment'),
	);

	return $form;
}

function sirius_ledger_page_payment_create_form_validate($form, &$form_state) {
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
	if ($max && $amt > $amt) { 
		form_set_error('payment][dollar_amt', 'Entries of type ' . $payment_type_term->name . ' must have a value less than ' . number_format($max, 2));
	}
	if ($min && $amt < $min) { 
		form_set_error('payment][dollar_amt', 'Entries of type ' . $payment_type_term->name . ' must have a value greater than ' . number_format($min, 2));
	}
}

function sirius_ledger_page_payment_create_form_submit($form, &$form_state) {
	global $user;
	$domain_nid = sirius_domain_nid();

	// Create the event node
	$payment_node = new stdClass();
	$payment_node->type = 'sirius_payment';
	$payment_node->no_recurse = TRUE;
	$payment_node->language = $user->language;
	$payment_node->uid = $user->uid;
	$payment_node->status = 1;
	$payment_node->promote = 0;
	$payment_node->comment = 0;
	if ($form_state['values']['payment']['payer_type'] == 'employer') {
		$payment_node->field_sirius_payer['und'][0]['target_id'] = $form_state['values']['payment']['payer_employer'];
	} else {
		$worker_node = sirius_worker_get_by_id($form_state['values']['payment']['payer_worker']);
		if (!$worker_node) { drupal_set_message("Worker not found.", 'error'); return; }
		$payment_node->field_sirius_payer['und'][0]['target_id'] = $worker_node->field_sirius_contact['und'][0]['target_id'];
	}
	$payment_node->field_sirius_ledger_account['und'][0]['target_id'] = $form_state['values']['payment']['ledger_account'];
	$payment_node->field_sirius_payment_type['und'][0]['tid'] = $form_state['values']['payment']['payment_type'];
	$payment_node->field_sirius_dollar_amt['und'][0]['value'] = $form_state['values']['payment']['dollar_amt'];
	$payment_node->field_sirius_merchant_name['und'][0]['value'] = $form_state['values']['payment']['merchant_name'];
	$payment_node->field_sirius_check_number['und'][0]['value'] = $form_state['values']['payment']['check_number'];
	$payment_node->field_sirius_notes['und'][0]['value'] = $form_state['values']['payment']['notes'];
	if ($form_state['values']['payment']['datetime_created']) {
		$payment_node->field_sirius_datetime_created['und'][0]['value'] = gmdate('Y-m-d H:i:s', strtotime($form_state['values']['payment']['datetime_created']));
	}

	$payment_node->field_sirius_ledger_allocated['und'][0]['value'] = 'No';
	$payment_node->field_sirius_payment_status['und'][0]['value'] = 'Received';

	sirius_ledger_payment_set_title($payment_node);

	if ($domain_nid) { $payment_node->field_sirius_domain['und'][0]['target_id'] = $domain_nid; }
	node_object_prepare($payment_node);
	$payment_node = node_submit($payment_node);
	node_save($payment_node);
	drupal_set_message(t("Created a new payment: ") . $payment_node->title);

	drupal_goto('node/' . $payment_node->nid . '');
}

function sirius_ledger_page_payment_create_form_ajax_change_payment_type($form, $form_state) {
	$commands = array();
  $commands[] = ajax_command_replace("#sirius_ledger_page_payment_create_form_ajax_dollar_amt", render($form['payment']['dollar_amt']));
	return array('#type' => 'ajax', '#commands' => $commands);
}
*/
