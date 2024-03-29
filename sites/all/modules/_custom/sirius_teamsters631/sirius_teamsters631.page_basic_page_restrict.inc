<?php

function sirius_teamsters631_node_tab_basic_page_restrict($basic_page_node) {
	return drupal_get_form('sirius_teamsters631_node_tab_basic_page_restrict_form', $basic_page_node);
}

function sirius_teamsters631_node_tab_basic_page_restrict_form($form, &$form_state, $basic_page_node) {
	$form_state['basic_page_nid'] = $basic_page_node->nid;

	$basic_page_json = sirius_json_get($basic_page_node);

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
			'#default_value' => sirius_form_array_default_value($basic_page_json['restrict']['ms']),
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
			'#default_value' => sirius_form_array_default_value($basic_page_json['restrict']['ws']),
		);
	}

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_teamsters631_node_tab_basic_page_restrict_form_submit($form, &$form_state) {
	$basic_page_node = node_load($form_state['basic_page_nid']);
	$basic_page_json = sirius_json_get($basic_page_node);
	$basic_page_json['restrict'] = $form_state['values']['restrict'];
	sirius_json_set($basic_page_node, $basic_page_json);
	node_save($basic_page_node);
	drupal_set_message("Settings saved.");
}