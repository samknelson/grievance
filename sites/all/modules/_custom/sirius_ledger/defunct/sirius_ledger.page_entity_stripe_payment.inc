<?php

function sirius_ledger_node_tab_entity_stripe_payment_access($entity_node) {
	$access = sirius_access_instance('ledger');
	return $access->access('entity_page_view', array('entity_node' => $entity_node));
}

function sirius_ledger_node_tab_entity_stripe_payment_form($form, &$form_state, $entity_node) {
	sirius_require('sirius_ledger', 'sirius_ledger.stripe.inc');
  drupal_add_css('sites/all/modules/_custom/sirius_ledger/css/stripe.css');

  drupal_set_message("... Under Development ...", 'warning');

	$access = sirius_access_instance('ledger');

	$customer_id = $entity_node->uuid;

	$form_state['entity_nid'] = $entity_node->nid;

	// Load the customer, or create one if it doesn't exist.
	$result = sirius_ledger_stripe_customer_retrieve($customer_id);
	if (!$result['success']) { return sirius_form_error('Unable to fetch or sync customer: ' . $result['msg']); }

	// Load the payment methods
	$result = sirius_ledger_stripe_paymentmethod_list_multitype($customer_id, array('card', 'us_bank_account'));
	if ($result['success']) { $paymentmethods_json = $result['paymentmethods']; }
	$paymentmethods_options = sirius_ledger_stripe_paymentmethods_options($entity_node, $paymentmethods_json);
	if (!$paymentmethods_options) { return sirius_form_error('Please create at least one payment method.'); }

	$form['amount'] = array(
		'#type' => 'textfield',
		'#title' => 'Amount',
		'#size' => 6,
		'#field_prefix' => '$',
	);

	$account_options = sirius_ledger_account_list();
	$form['account'] = array(
		'#type' => 'select',
		'#title' => 'Account',
		'#options' => array(NULL => t('-- Select --')) + $account_options,
	);

	$form['paymentmethod'] = array(
		'#type' => 'select',
		'#title' => 'Payment Method',
		'#options' => array(NULL => t('-- Select --')) + $paymentmethods_options,
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_ledger_node_tab_entity_stripe_payment_form_submit($form, &$form_state) {
	sirius_require('sirius_ledger', 'sirius_ledger.stripe.inc');
	$entity_node = node_load($form_state['entity_nid']);
	$customer_id = $entity_node->uuid;
	$action = $form_state['values']['action'];

	// $form_state['rebuild'] = TRUE;
	$result = sirius_ledger_stripe_paymentmethod_charge(
		$entity_node, 
		$form_state['values']['amount'],
		$form_state['values']['account'],
		$form_state['values']['paymentmethod']
	);

	if (!$result['success']) {
		drupal_set_message("Transaction failed: " . $result['msg']);
		return;
	}
	
	dpm(sirius_ledger_serivce()->object_to_json($result['paymentintent']));
}

