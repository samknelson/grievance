<?php

function sirius_node_tab_contact_json_access($contact_node) {
	if (node_access('update', $contact_node) != NODE_ACCESS_ALLOW) { return FALSE; }

	return TRUE;
}

function sirius_node_tab_contact_json($cotnact_node) {
	return drupal_get_form('sirius_node_tab_contact_json_form', $cotnact_node);
}

function sirius_node_tab_contact_json_form($form, &$form_state, $contact_node) {
	$which = 'default';

	$form_state['contact_nid'] = $contact_node->nid;
	$form_state['which'] = $which;

	$contact_definition_json = sirius_contact_definition();
	$contact_definition_form = sirius_jsonfield_util_form_get($contact_definition_json, $which);

	$contact_json = sirius_jsonfield_util_node_json_get($contact_node);
	$contact_results = sirius_jsonfield_util_results_get($contact_json, $which);
	sirius_jsonfield_util_set_defaults($contact_definition_form, $contact_results);

	$form[$which] = $contact_definition_form;
	$form['#tree'] = TRUE;

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
	$which = $form_state['which'];

	$contact_node = node_load($form_state['contact_nid']);
	$contact_json = sirius_jsonfield_util_node_json_get($contact_node);
	sirius_jsonfield_util_results_set($contact_json, $which, $form_state['values'][$which]);
	sirius_jsonfield_util_node_json_set($contact_node, $contact_json);
	sirius_jsonfield_log($contact_node, 'survey', 'Survey updated.');
	node_save($contact_node);
}