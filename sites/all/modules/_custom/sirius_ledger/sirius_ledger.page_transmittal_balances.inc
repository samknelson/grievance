<?php

function sirius_ledger_page_transmittal_balances() {
	return drupal_get_form('sirius_ledger_page_transmittal_balances_form');
}

function sirius_ledger_page_transmittal_balances_form($form, &$form_state) {
	$accounts = sirius_ledger_account_list();
	$form['account'] = array(
		'#type' => 'select',
		'#options' => array(NULL => t('-- Select --')) + $accounts,
		'#default_value' => 6,
		'#title' => 'Account',
		'#required' => TRUE,
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	if ($form_state['results']) {
		$form['results'] = array('#markup' => '<hr>' . $form_state['results']);
	}

	return $form;
}
