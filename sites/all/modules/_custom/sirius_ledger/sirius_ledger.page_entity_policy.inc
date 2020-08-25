<?php

function sirius_ledger_node_tab_entity_policy($entity_node) {
	return drupal_get_form('sirius_ledger_node_tab_entity_policy_form', $entity_node);
}

function sirius_ledger_node_tab_entity_policy_form($form, &$form_state, $entity_node) {
	$form_state['entity_nid'] = $entity_node->nid;

	$json = sirius_jsonfield_util_node_json_get($entity_node);

	$policies = sirius_ledger_policy_list();
	$policy_options = array(NULL => t('-- Select --'));
	foreach ($policies as $policy_nid => $policy_node) { $policy_options[$policy_nid] = $policy_node->title; }

	$form['#tree'] = TRUE;
	$form['ledger']['policy']['nid']['default'] = array(
		'#type' => 'select',
		'#options' => $policy_options,
		'#title' => 'Policy',
		'#default_value' => $json['ledger']['policy']['nid']['default'],
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_ledger_node_tab_entity_policy_form_submit($form, &$form_state) {
	$entity_node = node_load($form_state['entity_nid']);
	$json = sirius_jsonfield_util_node_json_get($entity_node);
	$json['ledger']['policy']['nid'] = $form_state['values']['ledger']['policy']['nid'];
	sirius_jsonfield_util_node_json_set($entity_node, $json);
	node_save($entity_node);
	drupal_set_message("Policy selection saved.");
}