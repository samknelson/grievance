<?php

function sirius_training_evaluation_evaluate_launch_access() {
	// Evaluations must be enabled
	if (sirius_domain_variable_get('sirius_training_evaluations_enabled', '') != 'Yes') { return FALSE; }

	// Must have one of the roles permitted to submit evaluations
	$roles = sirius_domain_variable_get('sirius_training_evaluations_roles');
	$evaluation_rids = array();
	foreach ($roles as $rid => $value) { if ($value) { $evaluation_rids[] = $value; } }
	global $user;
	$my_rids = array_keys($user->roles);
	if (array_intersect($my_rids, $evaluation_rids)) { return TRUE; }

	// Oh well.
	return FALSE;
}

function sirius_training_evaluation_evaluate_launch() {
	return drupal_get_form('sirius_training_evaluation_evaluate_launch_form');
}

function sirius_training_evaluation_evaluate_launch_form($form, &$form_state) {
	$form['worker_id'] = array(
		'#type' => 'textfield',
		'#size' => 10,
		'#title' => t('Worker ID'),
		'#description' => t('Enter the badge number of the worker for whom you are submitting an evaluation.'),
		'#required' => TRUE
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_training_evaluation_evaluate_launch_form_validate($form, &$form_state) {
	$worker_id = trim($form_state['values']['worker_id']);
	if (!$worker_id) {
		form_set_error('worker_id', t('Please enter a valid numeric id.'));
		return;
	}

	$worker_node = sirius_worker_get_by_id($worker_id);
	if (!$worker_node) {
		form_set_error('worker_id', t('The worker with ID @worker_id was not found.', array('@worker_id' => $worker_id)));
		return;
	}

	if (!sirius_training_evaluation_evaluate_access($worker_id)) {
		form_set_error('worker_id', t('You do not have permission to submit an evaluation for the worker with ID @worker_id.', array('@worker_id' => $worker_id)));
		return;
	}
}

function sirius_training_evaluation_evaluate_launch_form_submit($form, &$form_state) {
	$worker_id = trim($form_state['values']['worker_id']); 
	drupal_goto(current_path() . '/' . $worker_id);
}