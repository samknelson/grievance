<?php

function sirius_ledger_node_tab_auth_accounts_access($auth_node) {
	if ($auth_node->field_sirius_category['und'][0]['value'] != 'ledger:auth') { return FALSE; }
	return TRUE;
}

function sirius_ledger_node_tab_auth_accounts($auth_node) {
	return drupal_get_form('sirius_ledger_node_tab_auth_accounts_form', $auth_node);
}

function sirius_ledger_node_tab_auth_accounts_form($form, &$form_state, $auth_node) {
	$form_state['auth_nid'] = $auth_node->nid;

  $esignature_status = sirius_esignature_entity_status($auth_node);
  if ($esignature_status['status'] != 'unsigned') {
    $form['intro'] = array('#markup' => '<p><strong>Because this authorization has already been signed, no changes can be made. (Signature status: ' . $esignature_status['status_msg'] . '.)</strong></p>');
  }

	$default = array_keys(sirius_ledger_auth_entities($auth_node, 'sirius_ledger_account'));
	$options = sirius_ledger_account_list();

	$form['accounts'] = array(
		'#type' => 'checkboxes',
		'#options' => $options,
		'#default_value' => $default,
		'#title' => 'Accounts',
	);

	if ($esignature_status['status'] == 'unsigned') {
		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit')
		);
	}

	return $form;
}

function sirius_ledger_node_tab_auth_accounts_form_submit($form, &$form_state) {
	$auth_node = node_load($form_state['auth_nid']);

	$account_nids = array_filter(array_values($form_state['values']['accounts']));
	sirius_ledger_auth_entities_set($auth_node, 'sirius_ledger_account', $account_nids);

	sirius_ledger_auth_set_title($auth_node);
}