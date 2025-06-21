<?php

function sirius_ledger_page_stripe_idsearch() {
	return drupal_get_form('sirius_ledger_page_stripe_idsearch_form');
}

function sirius_ledger_page_stripe_idsearch_form($form, &$form_state) {
	$form['id'] = array(
		'#type' => 'textfield',
		'#title' => t('Stripe ID'),
		'#description' => t('Enter any Stripe ID. At the moment, only payment IDs (beginning "pi_...") are supported.'),
		'#required' => TRUE,
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	if ($form_state['submitted']) {
		$service = sirius_ledger_service();
		$nid = $service->fetch_by_id($form_state['values']['id'], TRUE);

		if ($nid) {
			drupal_goto("node/$nid");
		} else {
			drupal_set_message("ID not found: " . $form_state['values']['id'], 'warning');
		}
	}

	return $form;
}

function sirius_ledger_page_stripe_idsearch_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;
}