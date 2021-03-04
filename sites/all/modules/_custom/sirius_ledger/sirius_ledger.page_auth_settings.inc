<?php

function sirius_ledger_node_tab_auth_settings_access($auth_node) {
	if ($auth_node->field_sirius_category['und'][0]['value'] != 'ledger:auth') { return FALSE; }
	return TRUE;
}

function sirius_ledger_node_tab_auth_settings($auth_node) {
	return drupal_get_form('sirius_ledger_node_tab_auth_settings_form', $auth_node);
}

function sirius_ledger_node_tab_auth_settings_form($form, &$form_state, $auth_node) {
	$form_state['auth_nid'] = $auth_node->nid;
	$form['#tree'] = TRUE;
	$auth_json = sirius_jsonfield_util_node_json_get($auth_node);

  $esignature_status = sirius_esignature_entity_status($auth_node);
  if ($esignature_status['status'] != 'unsigned') {
    $form['intro'] = array('#markup' => '<p><strong>Because this authorization has already been signed, no changes can be made. (Signature status: ' . $esignature_status['status_msg'] . '.)</strong></p>');
  }

	$form['settings']['title'] = array(
		'#title' => 'Title',
		'#type' => 'textfield',
		'#default_value' => $auth_json['auth']['settings']['title'],
	);

	$form['settings']['comments'] = array(
		'#title' => 'Comments',
		'#type' => 'textarea',
		'#default_value' => $auth_json['auth']['settings']['comments'],
	);

	if ($esignature_status['status'] == 'unsigned') {
		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit')
		);
	}

	return $form;
}

function sirius_ledger_node_tab_auth_settings_form_submit($form, &$form_state) {
	$auth_node = node_load($form_state['auth_nid']);
	$auth_json = sirius_jsonfield_util_node_json_get($auth_node);
	$auth_json['auth']['settings'] = $form_state['values']['settings'];
	sirius_jsonfield_util_node_json_set($auth_node, $auth_json);
	sirius_ledger_auth_set_title($auth_node);
	drupal_set_message("Settings saved.");
}