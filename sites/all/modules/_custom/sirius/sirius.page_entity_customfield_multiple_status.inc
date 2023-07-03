<?php

function sirius_node_tab_entity_customfield_multiple_status_access($data_node) {
	// Only log entries of category "customfield"
	if ($data_node->field_sirius_category['und'][0]['value'] != 'customfield') { return FALSE; }

	// Must have "edit" access
	$context = sirius_customfield_multiple_context($data_node);
	if (!sirius_customfield_access('edit', $context)) { return FALSE; }

	// We must have a workflow that supports statuses, with at least one status available to this user
	$instance = sirius_customfield_access_instance($context['set_node']);
	if (method_exists($instance, 'status_options')) { $status_options = $instance->status_options($context); }
	if (!$status_options) { return FALSE; }

	return TRUE;
}

function sirius_node_tab_entity_customfield_multiple_status_form($form, &$form_state, $data_node) {
	$context = sirius_customfield_multiple_context($data_node);
	$form_state['data_nid'] = $data_node->nid;
	$form['#tree'] = TRUE;

	$instance = sirius_customfield_access_instance($context['set_node']);
	$status_options = $instance->status_options($context);

	$form['status'] = array(
		'#type' => 'select',
		'#title' => t('Status'),
		'#options' => $status_options,
		'#default_value' => sirius_customfield_status($context),
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_node_tab_entity_customfield_multiple_status_form_submit($form, &$form_state) {
	$data_node = node_load($form_state['data_nid']);
	$context = sirius_customfield_multiple_context($data_node);
	sirius_customfield_status_set($context, $form_state['values']['status']);
	drupal_set_message(t('Status updated.'));
	// If we no longer have any status options, redirect
	if (!sirius_node_tab_entity_customfield_multiple_status_access($data_node)) {
		drupal_goto('node/' . $context['entity_node']->nid . '/sirius_dispatch_job_group_customfield_tab/' . $context['set_node']->nid);
	}
}