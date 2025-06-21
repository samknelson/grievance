<?php

function sirius_ledger_node_tab_entity_stripe_customer_access($entity_node) {
	$access = sirius_access_instance('ledger');
	return $access->access('admin', array('entity_node' => $entity_node));
}

function sirius_ledger_node_tab_entity_stripe_customer_form($form, &$form_state, $entity_node) {
	$service = sirius_ledger_service();
  drupal_add_css('sites/all/modules/_custom/sirius_ledger/css/stripe.css');

	$customer_id = $entity_node->uuid;

	$form_state['entity_nid'] = $entity_node->nid;

	// Load the customer, or create one if it doesn't exist.
	$result = $service->customer_retrieve($customer_id);
	if (!$result['success']) { $result = $service->customer_sync($entity_node); }
	if (!$result['success']) { return sirius_form_error('Unable to fetch or sync customer: ' . $result['msg']); }

	$html .= $service->customer_render($service->object_to_json($result['customer']));
	$form['intro'] = array('#markup' => $html);

	$actions = array();
	$actions['customer_sync'] = t('Create or Update Stripe Customer');

	$form['action'] = array(
		'#type' => 'select',
		'#options' => $actions,
		'#title' => 'Action',
		'#required' => TRUE,
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_ledger_node_tab_entity_stripe_customer_form_submit($form, &$form_state) {
	$service = sirius_ledger_service();
	$entity_node = node_load($form_state['entity_nid']);
	$customer_id = $entity_node->uuid;
	$action = $form_state['values']['action'];

	if ($action == 'customer_sync') {
		$result = $service->customer_sync($entity_node);
	}

	if ($result['success']) {
		drupal_set_message("Request succeeded.");
	} else {
		drupal_set_message("Stripe request failed: " . $result['msg'], 'error');
	}
}

