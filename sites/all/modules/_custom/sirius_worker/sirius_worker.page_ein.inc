<?php

function sirius_worker_node_tab_ein_lookup_access($employer_node) {
	$access = Sirius_Employer_Access::getInstance();
	return $access->access('worker_list', array('employer_node' => $employer_node));
}

function sirius_worker_node_tab_ein_lookup_form($form, &$form_state, $employer_node) {
	$form_state['employer_nid'] = $employer_node->nid;
	$form['ein'] = array(
		'#type' => 'textfield',
		'#title' => 'EIN',
		'#size' => 10,
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_worker_node_tab_ein_lookup_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;

	$employer_nid = $form_state['employer_nid'];
	$ein = $form_state['values']['ein'];

	$worker_nid = sirius_worker_ein_lookup($employer_nid, $ein);

	if (!$worker_nid) {
		drupal_set_message("No worker found with the EIN $ein.", 'warning');
	} else {
		$worker_node = node_load($worker_nid);
		drupal_set_message("EIN $ein corresponds to worker $worker_node->title.");
	}
}

function sirius_worker_node_tab_ein_assign_access($employer_node) {
	$access = Sirius_Employer_Access::getInstance();
	return $access->access('worker_manage', array('employer_node' => $employer_node));
}


function sirius_worker_node_tab_ein_assign_form($form, &$form_state, $employer_node) {
	$form_state['employer_nid'] = $employer_node->nid;

	$form['ssn'] = array(
		'#type' => 'textfield',
		'#title' => 'SSN',
		'#size' => 10,
	);

	$form['ein'] = array(
		'#type' => 'textfield',
		'#title' => 'EIN',
		'#size' => 10,
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_worker_node_tab_ein_assign_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;

	$employer_nid = $form_state['employer_nid'];
	$ssn = $form_state['values']['ssn'];
	$ein = $form_state['values']['ein'];

	$result = sirius_worker_ein_set($ssn, $employer_nid, $ein);
	if ($result['success']) { drupal_set_message($result['msg']); } 
	else { drupal_set_message($result['msg'], 'warning'); }
}
