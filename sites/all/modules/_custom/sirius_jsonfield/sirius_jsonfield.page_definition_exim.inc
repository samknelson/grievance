<?php

function sirius_json_definition_exim_form($form, &$form_state, $nid, $which_form = 'default') {
	$form_state['nid'] = $nid;
	$form_state['which_form'] = $which_form;

	$node = node_load($nid);
	$json = json_decode($node->field_sirius_json['und'][0]['value'], TRUE);

	$form['json'] = array(
		'#type' => 'textarea',
		'#default_value' => $json['form_encoded'][$which_form],
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
	$txt = $form_state['values']['json'];
	$parsed = json_decode($txt, TRUE);
	if (!$parsed) {
		drupal_set_message("Please enter a valid JSON-encoded string.", 'error');
		return;
	}
	$json = json_decode($node->field_sirius_json['und'][0]['value'], TRUE);
	$json['form_encoded'][$which_form] = $txt;
	$json['form'][$which_form] = $parsed;

	$node->field_sirius_json['und'][0]['value'] = json_encode($json);
	node_save($node);
	drupal_set_message("Form definition updated.");
}