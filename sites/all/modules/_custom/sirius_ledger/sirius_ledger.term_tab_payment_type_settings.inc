<?php

function sirius_ledger_term_tab_payment_type_settings($event_type_term) {
	return drupal_get_form('sirius_ledger_term_tab_payment_type_settings_form', $event_type_term);
}

function sirius_ledger_term_tab_payment_type_settings_form($form, &$form_state, $payment_type_term) {
	$form_state['payment_type_tid'] = $payment_type_term->tid;
	$json = sirius_jsonfield_util_term_json_get($payment_type_term);

	$form['#tree'] = TRUE;

	$form["payment_settings"]["sign"] = array(
		'#title' => t('Payment or Charge?'),
		'#type' => 'select',
		'#options' => array(
			NULL => t('-- Select --'),
			'payment' => t('Payment - Must be negative (credit to the payer)'),
			'charge' => t('Charge - Must be positive (credit to the fund)'),
		),
		'#default_value' => $json['payment_settings']['sign'],
	);

	$form["payment_settings"]["max"] = array(
		'#title' => t('Maximum Permitted Value'),
		'#type' => 'textfield',
		'#size' => 5,
		'#description' => 'Max and min should always be positive values.',
		'#default_value' => $json['payment_settings']['max'],
	);

	$form["payment_settings"]["min"] = array(
		'#title' => t('Minimum Permitted Value'),
		'#type' => 'textfield',
		'#size' => 5,
		'#default_value' => $json['payment_settings']['min'],
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_ledger_term_tab_payment_type_settings_form_submit($form, &$form_state) {
	$payment_type_tid = $form_state['payment_type_tid'];
	$payment_type_term = taxonomy_term_load($payment_type_tid);
	$json = sirius_jsonfield_util_term_json_get($payment_type_term);
	$json['payment_settings'] = $form_state['values']['payment_settings'];
	sirius_jsonfield_util_term_json_set($payment_type_term, $json);
	taxonomy_term_save($payment_type_term);
	drupal_set_message("Settings saved.");
}
