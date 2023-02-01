<?php

function sirius_steward_node_tab_log_sched_access($sched_node) {
	if ($sched_node->field_sirius_category['und'][0]['value'] != 'steward:sched') { return FALSE; }
	return TRUE;
}

function sirius_steward_node_tab_log_sched_form($form, &$form_state, $sched_node) {
	$form['#tree'] = TRUE;
	$form_state['sched_nid'] = $sched_node->nid;

	$json = sirius_json_get($sched_node);

	$form['sched']['whatever'] = array(
		'#type' => 'textfield',
		'#title' => t('Whatever'),
		'#default_value' => $json['sched']['whatever'],
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Save'),
	);

	return $form;
}

function sirius_steward_node_tab_log_sched_form_submit($form, &$form_state) {
	$sched_nid = $form_state['sched_nid'];
	$sched_node = node_load($sched_nid);
	$json = sirius_json_get($sched_node);
	$json['sched'] = $form_state['values']['sched'];
	sirius_json_set($sched_node, $json);
	node_save($sched_node);
	drupal_set_message("Schedule saved.");
}
