<?php

function sirius_json_definition_exim_form($form, &$form_state, $nid, $which_form = 'default') {
	$form_state['nid'] = $nid;
	$form_state['which_form'] = $which_form;

	$node = node_load($nid);
	$json = sirius_jsonfield_util_node_json_get($node);
	$json_form = sirius_jsonfield_util_form_get($json, $which_form);

	$form['json'] = array(
		'#type' => 'textarea',
		'#default_value' => json_encode($json_form, TRUE),
		'#title' => 'Raw Field Definition',
	);

	$form['confirm'] = array(
		'#type' => 'checkbox',
		'#title' => 'Yes, replace the ENTIRE form definition. This action cannot be undone.',
		'#required' => TRUE,
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => 'Import',
	);

	return $form;
}

function sirius_json_definition_exim_form_submit($form, &$form_state) {
	$nid = $form_state['nid'];
	$which_form = $form_state['which_form'];
	$node = node_load($nid);
	$json = sirius_jsonfield_util_node_json_get($node);
	$txt = $form_state['values']['json'];
	$json_form = json_decode($txt, TRUE);
	if (!$json_form) {
		drupal_set_message("Please enter a valid JSON-encoded string.", 'error');
		return;
	}
	sirius_jsonfield_util_form_set($json, $which_form, $json_form);
	sirius_jsonfield_util_node_json_set($node, $json);
	node_save($node);
	drupal_set_message("Form definition updated.");
}