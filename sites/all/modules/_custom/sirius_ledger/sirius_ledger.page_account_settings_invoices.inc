<?php

function sirius_ledger_node_tab_account_settings_invoices_form($form, &$form_state, $account_node) {
	$form_state['account_nid'] = $account_node->nid;

	$account_json = sirius_json_get($account_node);

	$form['#tree'] = TRUE;

	$form['settings']['invoices']['header'] = array(
		'#title' => t('Header'),
		'#type' => 'text_format',
		'#rows' => 20,
		'#description' => t('Header to show at the top of all invoices'),
	);

	$form['settings']['invoices']['footer'] = array(
		'#title' => t('Footer'),
		'#type' => 'text_format',
		'#rows' => 20,
		'#description' => t('Footer to show at the bottom of all invoices'),
	);

	sirius_jsonfield_util_set_defaults($form, $account_json);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}


function sirius_ledger_node_tab_account_settings_invoices_form_submit($form, &$form_state) {
	$account_node = node_load($form_state['account_nid']);
	$account_json = sirius_json_get($account_node);
	$form_state['values']['settings']['invoices']['header'] = sirius_richtext_handle_input($form_state['values']['settings']['invoices']['header']);
	$form_state['values']['settings']['invoices']['footer'] = sirius_richtext_handle_input($form_state['values']['settings']['invoices']['footer']);
	$account_json['settings']['invoices'] = $form_state['values']['settings']['invoices'];
	sirius_json_set($account_node, $account_json);
	node_save($account_node);
	drupal_set_message("Settings saved");
}

