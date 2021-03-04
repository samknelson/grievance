<?php
function sirius_node_tab_entity_machine_name_access($entity_node) {
	if ($entity_node->type == 'sirius_json_definition') {
		if ($entity_node->field_sirius_type['und'][0]['value'] != 'customfield') { return FALSE; }
	}

	return TRUE;
}

function sirius_node_tab_entity_machine_name($entity_node) {
	return drupal_get_form('sirius_node_tab_entity_machine_name_form', $entity_node);
}

function sirius_node_tab_entity_machine_name_form($form, &$form_state, $entity_node) {
	$form_state['entity_nid'] = $entity_node->nid;

	$json = sirius_jsonfield_util_node_json_get($entity_node);

	$form['machine_name'] = array(
		'#type' => 'textfield',
		'#title' => t('Machine Name'),
		'#default_value' => $json['machine_name'],
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_node_tab_entity_machine_name_form_submit($form, &$form_state) {
	$entity_node = node_load($form_state['entity_nid']);
	$json = sirius_jsonfield_util_node_json_get($entity_node);
	$machine_name = $form_state['values']['machine_name'];
	$machine_name = trim($machine_name);
	$machine_name = strtolower($machine_name);
	$machine_name = preg_replace('/[^a-z0-9_]/', '_', $machine_name);
	$machine_name = preg_replace('/_+/', '_', $machine_name);
	$json['machine_name'] = $machine_name;
	sirius_jsonfield_util_node_json_set($entity_node, $json);
	node_save($entity_node);
	drupal_set_message("Machine name saved.");
}