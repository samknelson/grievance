<?php

function sirius_ledger_node_tab_entity_stripe_paymentmethods_access($entity_node) {
	$access = sirius_access_instance('ledger');
	$result = $access->access('paymentmethods_view', array('entity_node' => $entity_node));
	return $result;
}

function sirius_ledger_node_tab_entity_stripe_paymentmethods_form($form, &$form_state, $entity_node) {
	$service = sirius_ledger_service();
	$access = sirius_access_instance('ledger');

  drupal_add_css('sites/all/modules/_custom/sirius_ledger/css/stripe.css');

	$customer_id = $entity_node->uuid;

	$form_state['entity_nid'] = $entity_node->nid;

	// Load the customer, or create one if it doesn't exist.
	$result = $service->customer_retrieve($customer_id);
	if (!$result['success']) { $result = $service->customer_sync($entity_node); }
	if (!$result['success']) { return sirius_form_error('Unable to fetch or sync customer: ' . $result['msg']); }

	// Load the payment methods
	$result = $service->paymentmethod_list($customer_id);
	if ($result['success']) { $paymentmethods_json = $result['paymentmethods']; }

	if ($access->access('paymentmethods_view', array('entity_node' => $entity_node))) {
		$html .= $service->paymentmethods_render($entity_node, $paymentmethods_json);
		$form['intro'] = array('#markup' => $html);
	}

	$paymentmethods_options = $service->paymentmethods_options($entity_node, $paymentmethods_json);

	$actions = array();
	$actions['paymentmethod_add'] = t('Add a Payment Method'); 

	if ($paymentmethods_options) {
		$actions['paymentmethod_set_default'] = t('Change the Default Payment Method'); 
		$actions['paymentmethod_detach'] = t('Remove a Payment Method'); 
	}

	$form['action'] = array(
		'#type' => 'select',
		'#options' => $actions,
		'#title' => 'Action',
		'#required' => TRUE,
	);

	if ($paymentmethods_options) {
		$form['paymentmethod'] = array(
			'#type' => 'select',
			'#title' => 'Payment Method',
			'#options' => array(NULL => t('-- Select --')) + $paymentmethods_options,
			'#states' => array(
				'visible' => array(
					":input[name=\"action\"]" => array(
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

function sirius_ledger_node_tab_entity_stripe_paymentmethods_form_submit($form, &$form_state) {
	$service = sirius_ledger_service();
	$entity_node = node_load($form_state['entity_nid']);
	$customer_id = $entity_node->uuid;
	$action = $form_state['values']['action'];

	if ($action == 'paymentmethod_add') {
		$result = $service->paymentmethod_add($entity_node);
	}

	if ($action == 'paymentmethod_set_default') {
		$result = $service->paymentmethod_default_set($entity_node, $form_state['values']['paymentmethod']);
	}

	if ($action == 'paymentmethod_detach') {
		$result = $service->paymentmethod_detach($entity_node, $form_state['values']['paymentmethod']);
	}

	if ($result['success']) {
		drupal_set_message("Request succeeded.");
	} else {
		drupal_set_message("Request failed: " . $result['msg'], 'error');
	}
}

