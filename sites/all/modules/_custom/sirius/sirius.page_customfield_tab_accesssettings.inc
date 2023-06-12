<?php

function sirius_node_tab_customfield_accesssettings_access($set_node) {
	// Only JSON Definitions of type "customfield"
	if ($set_node->field_sirius_type['und'][0]['value'] != 'customfield') { return FALSE; }

	$instance = sirius_customfield_access_instance($set_node);
	$form = $instance->settings_form();
	if (!$form) { return FALSE; }

	return TRUE;
}

function sirius_node_tab_customfield_accesssettings_form($form, &$form_state, $set_node) {
	$form_state['set_nid'] = $set_node->nid;
	$form['#tree'] = TRUE;
	$set_json = sirius_json_get($set_node);

	$instance = sirius_customfield_access_instance($set_node);

	$form['customfield']['access']['settings'][$instance->get_type()] = $instance->settings_form();
 	sirius_jsonfield_util_set_defaults($form, sirius_json_get($set_node));

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_node_tab_customfield_accesssettings_form_submit($form, &$form_state) {
	$set_node = node_load($form_state['set_nid']);
	$instance = sirius_customfield_access_instance($set_node);
	$json = sirius_json_get($set_node);
	$json['customfield']['access']['settings'][$instance->get_type()] = $form_state['values']['customfield']['access']['settings'][$instance->get_type()];
	sirius_json_set($set_node, $json);
	node_save($set_node);
	drupal_set_message(t('Settings saved.'));
}
