<?php

function sirius_json_definition_signatures_form($form, &$form_state, $nid, $which_form = 'default') {
	$form_state['nid'] = $nid;
	$form_state['which_form'] = $which_form;

	$node = node_load($nid);
	$json = sirius_jsonfield_util_node_json_get($node);
	$json_form = sirius_jsonfield_util_form_get($json, $which_form);
	$signature_definitions = $json_form['#sirius']['signature_definitions'];
	if (!$signature_definitions) { $signature_definitions = array(); }

	$count = count($signature_definitions) + 2;
	$form['#tree'] = TRUE;
	for ($i=0; $i<$count; ++$i) {
		$form[$i]['name'] = array(
			'#type' => 'textfield',
			'#title' => t('Signature line #@i', array('@i' => $i+1)),
			'#description' => t('Indicate who should sign this signature line -- i.e. instructor, student, etc.'),
			'#default_value' => $signature_definitions[$i]['name'],
		);
	}

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_json_definition_signatures_form_submit($form, &$form_state) {
	$nid = $form_state['nid'];
	$which_form = $form_state['which_form'];
	$node = node_load($nid);
	$json = sirius_jsonfield_util_node_json_get($node);

	$json_form = sirius_jsonfield_util_form_get($json, $which_form);

	$signature_definitions = array();
	$i = 0;
	while ($form_state['values'][$i]['name']) {
		$signature_definitions[] = $form_state['values'][$i];
		++$i;
	}
	$json_form['#sirius']['signature_definitions'] = $signature_definitions;

	sirius_jsonfield_util_form_set($json, $which_form, $json_form);
	sirius_jsonfield_util_node_json_set($node, $json);
	node_save($node);
	drupal_set_message("Form definition updated.");
}