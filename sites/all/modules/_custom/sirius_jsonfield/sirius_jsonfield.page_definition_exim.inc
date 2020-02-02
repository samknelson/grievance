<?php

function sirius_json_definition_exim_form($form, &$form_state, $nid) {
	$form_state['nid'] = $nid;
	$node = node_load($nid);
	$json = json_decode($node->field_sirius_json['und'][0]['value'], TRUE);

	$form['json'] = array(
		'#type' => 'textarea',
		'#default_value' => $node->field_sirius_json['und'][0]['value'],
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
	$node = node_load($nid);

	$txt = $form_state['values']['json'];
	$parsed = json_decode($txt, TRUE);
	if (!$parsed) {
		drupal_set_message("Please enter a valid JSON-encoded string.", 'error');
		return;
	}

	$node->field_sirius_json['und'][0]['value'] = $txt;
	node_save($node);
	drupal_set_message("Form definition updated.");
}