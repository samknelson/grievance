<?php

function sirius_ledger_node_tab_entity_stripe($entity_node) {
	return drupal_get_form('sirius_ledger_node_tab_entity_stripe_form', $entity_node);
}

function sirius_ledger_node_tab_entity_stripe_form($form, &$form_state, $entity_node) {
	sirius_require('sirius_ledger', 'sirius_ledger.stripe.inc');
  drupal_add_css('sites/all/modules/_custom/sirius_ledger/css/stripe.css');

	$customer_id = $entity_node->uuid;

	$form_state['entity_nid'] = $entity_node->nid;

	// Load the customer
	$result = sirius_ledger_stripe_customer_retrieve($customer_id);
	if ($result['success']) { $customer_json = sirius_ledger_stripe_object_to_json($result['customer']); }

	// Load the payment methods
	if ($customer_json) {
		$result = sirius_ledger_stripe_paymentmethod_list_multitype($customer_id, array('card', 'us_bank_account'));
		if ($result['success']) { $paymentmethods_json = $result['paymentmethods']; }
	}

	$html .= sirius_ledger_stripe_customer_render($customer_json);
	$html .= sirius_ledger_stripe_paymentmethods_render($entity_node, $paymentmethods_json);
	$form['intro'] = array('#markup' => $html);

	$paymentmethods_options = sirius_ledger_stripe_paymentmethods_options($entity_node, $paymentmethods_json);

	$actions = array();
	$actions['customer_sync'] = t('Create or Update Stripe Customer');
	$actions['paymentmethod_add'] = t('Add a Payment Method'); 

	if ($paymentmethods_options) {
		$actions['paymentmethod_charge'] = t('Make a payment'); 
		$actions['paymentmethod_set_default'] = t('Change the Default Payment Method'); 
		$actions['paymentmethod_detach'] = t('Remove a Payment Method'); 
	}

	$form['action'] = array(
		'#type' => 'select',
		'#options' => $actions,
		'#title' => 'Action',
		'#required' => TRUE,
	);

	$form['amount'] = array(
		'#type' => 'textfield',
		'#title' => 'Amount',
		'#size' => 6,
		'#field_prefix' => '$',
		'#states' => array('visible' => array(":input[name=\"action\"]" => array('value' => 'paymentmethod_charge'))),
	);

	$account_options = sirius_ledger_account_list();
	$form['account'] = array(
		'#type' => 'select',
		'#title' => 'Account',
		'#options' => array(NULL => t('-- Select --')) + $account_options,
		'#states' => array('visible' => array(":input[name=\"action\"]" => array('value' => 'paymentmethod_charge'))),
	);

	if ($paymentmethods_options) {
		$form['paymentmethod'] = array(
			'#type' => 'select',
			'#title' => 'Payment Method',
			'#options' => array(NULL => t('-- Select --')) + $paymentmethods_options,
			'#states' => array(
				'visible' => array(
					":input[name=\"action\"]" => array(
						array('value' => 'paymentmethod_charge'),
						array('value' => 'paymentmethod_set_default'),
						array('value' => 'paymentmethod_detach'),
					)
				)
			),
		);
	}

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_ledger_node_tab_entity_stripe_form_submit($form, &$form_state) {
	sirius_require('sirius_ledger', 'sirius_ledger.stripe.inc');
	$entity_node = node_load($form_state['entity_nid']);
	$customer_id = $entity_node->uuid;
	$action = $form_state['values']['action'];

	if ($action == 'customer_sync') {
		$result = sirius_ledger_stripe_customer_sync($entity_node);
	}

	if ($action == 'paymentmethod_add') {
		$result = sirius_ledger_stripe_paymentmethod_add($entity_node);
	}

	if ($action == 'paymentmethod_charge') {
		// $form_state['rebuild'] = TRUE;
		$result = sirius_ledger_stripe_paymentmethod_charge(
			$entity_node, 
			$form_state['values']['amount'],
			$form_state['values']['account'],
			$form_state['values']['paymentmethod']
		);
		if ($result['success']) {
			dpm(sirius_ledger_stripe_object_to_json($result['paymentintent']));
		}
	}

	if ($action == 'paymentmethod_set_default') {
		$result = sirius_ledger_stripe_paymentmethod_default_set($entity_node, $form_state['values']['paymentmethod']);
	}

	if ($action == 'paymentmethod_detach') {
		$result = sirius_ledger_stripe_paymentmethod_detach($entity_node, $form_state['values']['paymentmethod']);
	}

	if ($result['success']) {
		drupal_set_message("Request succeeded.");
	} else {
		drupal_set_message("Stripe request failed: " . $result['msg'], 'error');
	}
}

