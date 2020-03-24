<?php

function sirius_node_tab_contact_json_access($contact_node) {
	if (node_access('update', $contact_node) != NODE_ACCESS_ALLOW) { return FALSE; }

	return TRUE;
}

function sirius_node_tab_contact_json($cotnact_node) {
	return drupal_get_form('sirius_node_tab_contact_json_form', $cotnact_node);
}

function sirius_node_tab_contact_json_form($form, &$form_state, $contact_node) {
	$form_state['contact_nid'] = $contact_node->nid;
	$contact_definition_json = sirius_contact_definition();
	$form['survey'] = $contact_definition_json['form']['default'];
	$form['#tree'] = TRUE;

	$json_defaults = json_decode($contact_node->field_sirius_json['und'][0]['value'], TRUE);
	sirius_jsonfield_util_set_defaults($form['survey'], $json_defaults['survey']);

	$form['!intro'] = array(
		'#markup' => '<div class="sirius_form_intro">' . 
		'<div class="sirius-form">',
		'#weight' => -9999,
	);

	$form['!outro'] = array(
		'#markup' => '</div>',
		'#weight' => 9999,
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
		'#weight' => 10000,
	);

	return $form;
}

function sirius_node_tab_contact_json_form_submit($form, &$form_state) {
	$contact_node = node_load($form_state['contact_nid']);
	$json = json_decode($contact_node->field_sirius_json['und'][0]['value'], TRUE);
	$json['survey'] = $form_state['values']['survey'];
	sirius_jsonfield_log($contact_node, 'survey', 'Survey updated.');
	$contact_node->field_sirius_json['und'][0]['value'] = json_encode($json);
	node_save($contact_node);
}