<?php

function sirius_node_tab_worker_contact_bu_access($worker_node) {
	return node_access('view', $worker_node);
}

function sirius_node_tab_worker_contact_bu_form($form, &$form_state, $worker_node) {
	$contact_node = sirius_worker_contact_get($worker_node);
	if (!$contact_node) { return sirius_form_error('Contact not found.'); }

	$form_state['worker_nid'] = $worker_node->nid;
	$form['#tree'] = TRUE;

	$form['worker']['bu'] = array(
		'#type' => 'select',
		'#title' => t('Bargaining Unit'),
		'#options' => array(NULL => t('-- Select --')) + sirius_employer_bu_list(),
		'#default_value' => $worker_node->field_sirius_bu['und'][0]['target_id'],
	);

	$form['worker']['title'] = array(
		'#type' => 'textfield',
		'#title' => t('Title'),
		'#default_value' => $worker_node->field_sirius_title['und'][0]['value'],
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_node_tab_worker_contact_bu_form_submit($form, &$form_state) {
	$worker_node = node_load($form_state['worker_nid']);

	if ($form_state['values']['worker']['bu']) {
		$worker_node->field_sirius_bu['und'][0]['target_id'] = $form_state['values']['worker']['bu'];
	} else {
		unset($worker_node->field_sirius_bu['und'][0]);
	}
	$worker_node->field_sirius_title['und'][0]['value'] = $form_state['values']['worker']['title'];

	node_save($worker_node);
	drupal_set_message("Record saved");
}