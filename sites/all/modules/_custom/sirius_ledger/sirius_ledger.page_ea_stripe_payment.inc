<?php

function sirius_ledger_node_tab_ea_stripe_payment_access($ea_node) {
	if ($ea_node->field_sirius_category['und'][0]['value'] != 'ledger:ea') { return FALSE; }

	$access = sirius_access_instance('ledger');
	$ea = Sirius_Ledger_EA::getInstance();
	$entity_node = $ea->entity_node($ea_node);

	// Must have access to charge payment methods.
	return $access->access('paymentmethods_charge', array('entity_node' => $entity_node));
}

function sirius_ledger_node_tab_ea_stripe_payment_form($form, &$form_state, $ea_node) {
	sirius_require('sirius_ledger', 'sirius_ledger.stripe.inc');
  drupal_add_css('sites/all/modules/_custom/sirius_ledger/css/stripe.css');

	$ea = Sirius_Ledger_EA::getInstance();
	$entity_node = $ea->entity_node($ea_node);
	$account_node = $ea->account_node($ea_node);

	$service = sirius_ledger_service();
	$access = sirius_access_instance('ledger');
	$customer_id = $entity_node->uuid;

	// Load the customer, or create one if it doesn't exist.
	$result = $service->customer_retrieve($customer_id);
	if (!$result['success']) { $result = $service->customer_sync($entity_node); }
	if (!$result['success']) { return sirius_form_error('Unable to fetch or sync customer: ' . $result['msg']); }

	$form_state['ea_nid'] = $ea_node->nid;

	// Load the payment methods
	$result = $service->paymentmethod_list($customer_id);
	if ($result['success']) { $paymentmethods_json = $result['paymentmethods']; }
	$paymentmethods_options = $service-> paymentmethods_options($entity_node, $paymentmethods_json);
	if (!$paymentmethods_options) { return sirius_form_error('Please create at least one payment method.'); }

	$balance = $ea->balance($ea_node);
	$currency = sirius_ledger_currency_from_account_nid($account_nid);
	$balance_render = $currency->render_amt($balance);

	$form['intro'] = array(
		'#markup' => sirius_fakerender_field('Balance:', $balance_render),
	);

	$form['which'] = array(
		'#type' => 'select',
		'#title' => t('I want to:'),
		'#options' => array(
			NULL => t('-- Select --'),
			'full' => t('Pay full balance'),
			'partial' => t('Pay partial amount'),
		),
	);

	$form['amt_full'] = array('#type' => 'value', '#value' => $balance);

	$form['amt_partial'] = array(
		'#type' => 'textfield',
		'#title' => 'Payment Amount',
		'#size' => 6,
		'#field_prefix' => '$',
 		'#states' => array(
 			'visible' => array(
 				":input[name=\"which\"]" => array(
 					array('value' => 'partial'),
 				),
 			)
 		),
	);

	$form['paymentmethod'] = array(
		'#type' => 'select',
		'#title' => 'Payment Method',
		'#options' => array(NULL => t('-- Select --')) + $paymentmethods_options,
		'#required' => TRUE,
	);

	$form['confirm'] = array(
		'#type' => 'checkbox',
		'#title' => 'Yes, please charge the amount entered above to the selected payment method.',
		'#required' => TRUE,
		'#prefix' => '<br />',
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}





function sirius_ledger_node_tab_ea_stripe_payment_form_validate($form, &$form_state) {
	sirius_require('sirius_ledger', 'sirius_ledger.stripe.inc');
	$ea_node = node_load($form_state['ea_nid']);

	$ea = Sirius_Ledger_EA::getInstance();
	$entity_node = $ea->entity_node($ea_node);
	$account_node = $ea->account_node($ea_node);
	$currency = sirius_ledger_currency_from_account_nid($account_node->nid);

	$which = $form_state['values']['which'];
	if ($which == 'full') {
		$amt = $form_state['values']['amt_full'];
	} else {
		$amt = $form_state['values']['amt_partial'];
	}

	$result = $currency->validate_amt($amt);
	if (!$result['success']) {
		form_set_error("amt_partial", $result['msg']);
	}
}

function sirius_ledger_node_tab_ea_stripe_payment_form_submit($form, &$form_state) {
	sirius_require('sirius_ledger', 'sirius_ledger.stripe.inc');
	$ea_node = node_load($form_state['ea_nid']);

	$ea = Sirius_Ledger_EA::getInstance();
	$entity_node = $ea->entity_node($ea_node);
	$account_node = $ea->account_node($ea_node);
	$currency = sirius_ledger_currency_from_account_nid($account_node->nid);

	$service = sirius_ledger_service();
	$access = sirius_access_instance('ledger');
	$customer_id = $entity_node->uuid;

	$result = $service->paymentmethod_list($customer_id);
	if (!$result['success']) {
		drupal_set_message("Unable to generate a list of payment methods: " . $result['msg'], 'warning');
		return;
	}
	$paymentmethods_json = $result['paymentmethods'];

	$paymentmethods_options = $service->paymentmethods_options($entity_node, $paymentmethods_json);
	if (!$paymentmethods_options) {
		drupal_set_message('In order to make an online payment, please create at least one payment method.', 'warning');
		return;
	}

	$which = $form_state['values']['which'];
	if ($which == 'full') {
		$amt = $form_state['values']['amt_full'];
	} else {
		$amt = $form_state['values']['amt_partial'];
	}
	$result = $currency->validate_amt($amt);
	if (!$result['success']) {
		drupal_set_message($result['msg'], 'error');
		return;
	}
	$amt = $result['amt'];

	$paymentmethod_id = $form_state['values']['paymentmethod'];
	
	$paymentmethod_json = array();
	foreach ($paymentmethods_json['data'] as $idx => $tmp) {
		if ($idx != $paymentmethod_id) { continue; }
		$paymentmethod_json = $service->object_to_json($tmp);
		break;
	}

	$result = $service->paymentmethod_charge(
		$entity_node, 
		$amt,
		$account_node->nid,
		$paymentmethod_id
	);

	if (!$result['success']) {
		drupal_set_message("Transaction failed: " . $result['msg'], 'error');
		return $result;
	}

	drupal_set_message("A charge of " . $currency->render_amt($amt) . " has been applied to " . $paymentmethods_options[$paymentmethod_id] . ". When it clears, the account balance will be updated to reflect the payment.");

	return $result;
}

