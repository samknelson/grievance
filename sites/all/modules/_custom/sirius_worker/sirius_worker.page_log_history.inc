<?php

function sirius_node_tab_worker_log_history_access($log_node) {
	$history_info = sirius_worker_history_info($log_node->field_sirius_category['und'][0]['value']);
	if (!$history_info) { return FALSE; }

	return TRUE;
}

function sirius_node_tab_worker_log_history($log_node) {
	return drupal_get_form('sirius_node_tab_worker_log_history_form', $log_node);
}

function sirius_node_tab_worker_log_history_form($form, &$form_state, $log_node) {
	$form_state['log_nid'] = $log_node->nid;

	$log_json = sirius_json_get($log_node);

	$ts = $log_json['change']['ts'];
	if (!$ts) { $ts = $node->created; }

	// Commenting this out, because I think we don't want to allow you to change a work status history entry. 
	// Instead you should delete it and create a new one with a different effective date.
	/*
	if ($log_node->field_sirius_category['und'][0]['value'] == 'worker:work_status') {
		$ws_options = sirius_taxonomy_load('sirius_work_status', 'tid', 'name');
		$form['new_value'] = array(
			'#type' => 'select',
			'#title' => 'Work Status',
			'#options' => array(NULL => t('-- Select --')) + $ws_options,
			'#default_value' => $log_json['change']['new_value'],
			'#required' => TRUE,
		);
	}
	*/

	$form['view'] = array('#markup' => drupal_render(node_view($log_node), 'full'));

	$form['date'] = array(
		'#title' => t('Effective Date'),
		'#type' => 'date_popup',
		'#value' => t('Submit'),
		'#default_value' => date('Y-m-d H:i:s', $ts),
		'#date_format' => 'Y-m-d H:i:s',
    '#attributes' => array(
			'autocomplete' => 'off',
    ),
    '#required' => TRUE,
	);

	$active = $log_json['change']['active'];
	if (!$active) { $active = 'Yes'; }
	$form['active'] = array(
		'#type' => 'select',
		'#title' => 'Active?',
		'#options' => array(
			'Yes' => 'Yes',
			'No' => 'No',
		),
		'#default_value' => $active,
		'#description' => 'Select "No" to cancel this history entry.',
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_node_tab_worker_log_history_form_submit($form, &$form_state) {
	$log_node = node_load($form_state['log_nid']);
	$log_json = sirius_json_get($log_node);
	$log_json['change']['ts'] = strtotime($form_state['values']['date']);
	$log_json['change']['active'] = $form_state['values']['active'];
	sirius_json_set($log_node, $log_json);

	/*
	if ($log_node->field_sirius_category['und'][0]['value'] == 'worker:work_status') {
		$ws_term = taxonomy_term_load($form_state['values']['new_value']);
		$worker_node = sirius_log_handler($log_node, 'sirius_worker');
		$msg = $worker_node->title . ' has been assigned the new work status ' . $ws_term->name;
		$log_node->field_sirius_message['und'][0]['value'] = $msg;
	}
	*/

	node_save($log_node);

	drupal_set_message("History updated.");
}