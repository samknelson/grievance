<?php

function sirius_ledger_page_stripe_config_webhook() {
	return drupal_get_form('sirius_ledger_page_stripe_config_webhook_form');
}

function sirius_ledger_page_stripe_config_webhook_form($form, &$form_state) {
	sirius_require('sirius_ledger', 'sirius_ledger.stripe.inc');
	$webhookendpoint_id = sirius_domain_variable_get('sirius_ledger_stripe_webhookendpoint_id');
	if ($webhookendpoint_id) {
		$result = sirius_ledger_stripe_webhookendpoint_retrieve($webhookendpoint_id);
		if ($result['success']) {
			$form['msg'] = array('#markup' => sirius_fakerender_field('Current Endpoint:', $webhookendpoint_id));
		} else {
			drupal_set_message("Can't retrieve current endpoint: " . $result['msg'], 'warning');
		}
	}

	if ($result['webhookendpoint']) {
		$webhookendpoint_json = sirius_ledger_stripe_object_to_json($result['webhookendpoint']);
		$url = sirius_ledger_stripe_object_url($webhookendpoint_json);
		$form['link'] = array('#markup' => "<a href=\"$url\" target=\"_blank\" class=\"tool-button\">" . t('View in Stripe') . '</a>');
	}

	$actions = array();
	if (!$webhookendpoint_id || !$result['success']) {
		$actions['create'] = 'Create a webhook endpoint';
	}
	
	if ($webhookendpoint_id && $result['success']) {
		$actions['delete'] = 'Delete the webhook endpoint';
		$actions['update'] = 'Update the webhook endpoint';
	}

	$form['action'] = array(
		'#title' => t('Action'),
		'#type' => 'select',
		'#options' => array(NULL => t('-- Select --')) + $actions,
		'#required' => TRUE,
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_ledger_page_stripe_config_webhook_form_submit($form, &$form_state) {
	sirius_require('sirius_ledger', 'sirius_ledger.stripe.inc');
	$action = $form_state['values']['action'];

	if ($action == 'create') {
		$result = sirius_ledger_stripe_webhookendpoint_create();
		if ($result['success']) {
			$webhookendpoint_json = sirius_ledger_stripe_object_to_json($result['webhookendpoint']);
			sirius_domain_variable_set('sirius_ledger_stripe_webhookendpoint_id', $webhookendpoint_json['id']);
			sirius_domain_variable_set('sirius_ledger_stripe_webhookendpoint_secret', $webhookendpoint_json['secret']);
		} 
	}

	if ($action == 'delete') {
		$result = sirius_ledger_stripe_webhookendpoint_delete(sirius_domain_variable_get('sirius_ledger_stripe_webhookendpoint_id'));
		if ($result['success']) {
			sirius_domain_variable_set('sirius_ledger_stripe_webhookendpoint_id', NULL);
			sirius_domain_variable_set('sirius_ledger_stripe_webhookendpoint_secret', NULL);
		}
	}

	if ($action == 'update') {
		$result = sirius_ledger_stripe_webhookendpoint_update(sirius_domain_variable_get('sirius_ledger_stripe_webhookendpoint_id'));
	}

	if ($result['success']) {
		drupal_set_message("$action succeeded.");
	} else {
		drupal_set_message("$action failed: " . $result['msg'], 'error');
	}
}