<?php

function sirius_node_tab_entity_restrict_access($entity_node) {
	return node_access('update', $entity_node);
}
function sirius_node_tab_entity_restrict($entity_node) {
	return drupal_get_form('sirius_node_tab_entity_restrict_form', $entity_node);
}

function sirius_node_tab_entity_restrict_form($form, &$form_state, $entity_node) {
	$form_state['entity_nid'] = $entity_node->nid;

	$entity_json = sirius_json_get($entity_node);

	$form['#tree'] = TRUE;

	$result = sirius_proxy_call('sirius_taxonomy_load', 'sirius_member_status', 'tid', 'name');
	if (!$result['success']) { return sirius_form_error($result['msg']); }
	$ms_options = $result['data'];
	if ($ms_options) {
		$form['restrict']['ms'] = array(
			'#title' => t('Retrict by Member Status'),
			'#type' => 'checkboxes',
			'#options' => $ms_options,
			'#checkall' => TRUE,
			'#description' => t('Leave blank to allow access to all member statuses.'),
			'#default_value' => sirius_form_array_default_value($entity_json['restrict']['ms']),
		);
	}

	$result = sirius_proxy_call('sirius_taxonomy_load', 'sirius_work_status', 'tid', 'name');
	if (!$result['success']) { return sirius_form_error($result['msg']); }
	$ws_options = $result['data'];
	if ($ws_options) {
		$form['restrict']['ws'] = array(
			'#title' => t('Retrict by Work Status'),
			'#type' => 'checkboxes',
			'#options' => $ws_options,
			'#checkall' => TRUE,
			'#description' => t('Leave blank to allow access to all work statuses.'),
			'#default_value' => sirius_form_array_default_value($entity_json['restrict']['ws']),
		);
	}

	$roles =  user_roles(TRUE);
	$form['restrict']['role'] = array(
		'#title' => t('Retrict by Role'),
		'#type' => 'checkboxes',
		'#options' => $roles,
		'#checkall' => TRUE,
		'#description' => t('Leave blank to allow access to all roles.'),
		'#default_value' => sirius_form_array_default_value($entity_json['restrict']['role']),
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_node_tab_entity_restrict_form_submit($form, &$form_state) {
	$entity_node = node_load($form_state['entity_nid']);
	$entity_json = sirius_json_get($entity_node);
	$entity_json['restrict'] = $form_state['values']['restrict'];
	sirius_json_set($entity_node, $entity_json);
	node_save($entity_node);
	drupal_set_message("Settings saved.");
}