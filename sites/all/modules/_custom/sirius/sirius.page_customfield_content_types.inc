<?php

function sirius_node_tab_customfield_content_types_access($cf_node) {
	// Only JSON Definitions of type "customfield"
	if ($cf_node->field_sirius_type['und'][0]['value'] != 'customfield') { return FALSE; }

	return TRUE;
}

function sirius_node_tab_customfield_content_types_form($form, &$form_state, $cf_node) {
	$form_state['cf_nid'] = $cf_node->nid;

	$cf_json = sirius_json_get($cf_node);
	$form['#tree'] = TRUE;

	$options = sirius_customfield_content_type_options(sirius_customfield_content_types_all());
	$form['customfield']['content_types']['types_enabled'] = array(
		'#title' => 'This custom field set is available for the following content types:',
		'#type' => 'checkboxes',
		'#options' => $options,
		'#checkall' => TRUE,
		'#default_value' => array_keys(sirius_customfield_content_types($cf_json)),
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;	
}

function sirius_node_tab_customfield_content_types_form_submit($form, &$form_state) {
	$cf_node = node_load($form_state['cf_nid']);
	$cf_json = sirius_json_get($cf_node);
	$cf_json['customfield']['content_types'] = $form_state['values']['customfield']['content_types'];
	sirius_json_set($cf_node, $cf_json);
	node_save($cf_node);
	drupal_set_message('Settings saved.');
}