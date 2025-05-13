<?php

function sirius_ledger_node_tab_account_settings_description_form($form, &$form_state, $account_node) {
	$form_state['account_nid'] = $account_node->nid;

	$account_json = sirius_json_get($account_node);

	$form['#tree'] = TRUE;

	$form['settings']['description'] = array(
		'#title' => t('Description'),
		'#type' => 'text_format',
		'#rows' => 20,
		'#description' => t('Description of the fund, shown on the landing page for both employers and workers.'),
	);

	sirius_jsonfield_util_set_defaults($form, $account_json);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}


function sirius_ledger_node_tab_account_settings_description_form_submit($form, &$form_state) {
	$account_node = node_load($form_state['account_nid']);
	$account_json = sirius_json_get($account_node);
	$account_json['settings']['description'] = sirius_richtext_handle_input($form_state['values']['settings']['description']);
	sirius_json_set($account_node, $account_json);
	node_save($account_node);
	drupal_set_message("Settings saved");
}

