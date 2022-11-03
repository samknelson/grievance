<?php

function sirius_node_tab_customfield_tab_access($cf_node) {
	// Only JSON Definitions of type "customfield"
	if ($cf_node->field_sirius_type['und'][0]['value'] != 'customfield') { return FALSE; }

	return TRUE;
}

function sirius_node_tab_customfield_tab_form($form, &$form_state, $cf_node) {
	$form_state['cf_nid'] = $cf_node->nid;

	$cf_json = sirius_json_get($cf_node);
	$form['#tree'] = TRUE;

	$form['customfield']['tab']['title'] = array(
		'#title' => 'Title',
		'#type' => 'textfield',
		'#default_value' => $cf_json['customfield']['tab']['title'],
	);

	$form['customfield']['tab']['weight'] = array(
		'#title' => 'Weight',
		'#type' => 'textfield',
		'#default_value' => $cf_json['customfield']['tab']['weight'],
	);

	$form['customfield']['tab']['roles'] = array(
		'#title' => 'Roles',
		'#type' => 'checkboxes',
		'#default_value' => sirius_form_array_default_value($cf_json['customfield']['tab']['roles']),
		'#options' => user_roles(TRUE),
		'#checkall' => TRUE,
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;	
}

function sirius_node_tab_customfield_tab_form_submit($form, &$form_state) {
	$cf_node = node_load($form_state['cf_nid']);
	$cf_json = sirius_json_get($cf_node);
	$cf_json['customfield']['tab'] = $form_state['values']['customfield']['tab'];
	sirius_json_set($cf_node, $cf_json);
	node_save($cf_node);
	drupal_set_message('Settings saved.');
}