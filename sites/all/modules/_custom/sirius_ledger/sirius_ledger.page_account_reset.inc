<?php

function sirius_ledger_node_tab_account_reset_form($form, &$form_state, $account_node) {
	$form_state['account_nid'] = $account_node->nid;

	$form['date'] = array(
	 	'#type' => 'textfield',
	 	'#title' => t('Date'),
	 	'#description' => 'Reset entries as of the specified date (i.e. the balance will be "0" as of the date given.)',
	 	'#required' => TRUE,
	);

	$form['memo'] = array(
	 	'#type' => 'textfield',
	 	'#title' => t('Memo'),
	 	'#description' => 'Enter the memo that should be shown to accompany the offsetting A/R entry.',
	 	'#required' => TRUE,
	 );

	$form['confirm'] = array(
	 	'#type' => 'checkbox',
	 	'#title' => t('I understand that his will permanently and completely reset the ledger. It will create offsetting ledger entries which cause the balance for each participant to reset to 0. This action cannot be undone.'),
	 	'#required' => TRUE,
	 );

	 $form['submit'] = array(
	 	'#type' => 'submit',
	 	'#value' => t('Submit'),
	 );

	 return $form;
}

function sirius_ledger_node_tab_account_reset_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;
	$account_node = node_load($form_state['account_nid']);

	$form_state['rebuild'] = TRUE;
	$account_nid = $account_node->nid;
	if (!$account_nid) {
		drupal_set_message("Please select an account.");
		return;
	}

	$params = array();
	$params['ledger_account'] = $account_nid;
	$count = sirius_ledger_ar_reset_all($account_nid, $form_state['values']['date'], $form_state['values']['memo']);

	drupal_set_message("Account reset. Up to $count participants have been reset.");
}
