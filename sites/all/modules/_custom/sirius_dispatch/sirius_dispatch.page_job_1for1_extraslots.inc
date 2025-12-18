<?php

function sirius_node_tab_dispatch_job_1for1_extraslots_access($job_node) {
	// 1for1 plugin must be active
	if (!sirius_dispatch_elig_plugin_active($job_node, '1for1')) { return FALSE; }

	// Managers can access
	if (!sirius_node_tab_dispatch_job_manage_access($job_node)) { return FALSE; }

	return TRUE;
}

function sirius_node_tab_dispatch_job_1for1_extraslots_form($form, &$form_state, $job_node) {
	drupal_add_css(drupal_get_path('module', 'sirius') . '/css/table.css');

	$form_state['job_nid'] = $job_node->nid;

	$tos = Sirius_Worker_Tos::getInstance();
	$for1 = sirius_dispatch_elig_instance('1for1');

	$form['total'] = array(
		'#type' => 'textfield',
		'#size' => '4',
		'#title' => t('Extra Slots'),
		'#description' => t('How many extra slots are available for this job?'),
		'#default_value' => $for1->extraslots_get_total($job_node),
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_node_tab_dispatch_job_1for1_extraslots_form_submit($form, &$form_state) {
	$job_node = node_load($form_state['job_nid']);

	$tos = Sirius_Worker_Tos::getInstance();
	$for1 = sirius_dispatch_elig_instance('1for1');

	$result =  SiriusResult::create($for1->extraslots_set_total($job_node, $form_state['values']['total']));
	$result->drupalSetMessage();
}

