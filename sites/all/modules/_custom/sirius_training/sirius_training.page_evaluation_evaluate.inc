<?php

function sirius_training_evaluation_evaluate_access($worker_id) {
	// Check the parent.
	if (!sirius_training_evaluation_evaluate_launch_access()) { return FALSE; }

	// Verify the worker
	$worker_node = sirius_worker_get_by_id($worker_id);
	if (!$worker_node) { return FALSE; }
	if ($worker_node->type != 'sirius_worker') { return FALSE; }
	if (!sirius_training_evaluations_enabled_by_worker($worker_node)) { return FALSE; }

	// Verify the domain
	$domain_nid = sirius_domain_nid();
	if ($domain_nid != $worker_node->field_sirius_domain['und'][0]['target_id']) { return FALSE; }

	// @todo: Check that this user has permission to evaluate this worker, i.e. that they're actually an apprentice. But we're not doing that at the moment.

	return TRUE;
}

function sirius_training_evaluation_evaluate($worker_id) {
	return drupal_get_form('sirius_training_evaluation_evaluate_form', $worker_id);
}

function sirius_training_evaluation_evaluate_form($form, &$form_state, $worker_id) {
	$form_state['worker_id'] = $worker_id;
	$worker_node = sirius_worker_get_by_id($worker_id);
	drupal_set_title('Evaluating: ' . $worker_id . ' - ' . $worker_node->title);

	$domain_nid = sirius_domain_nid();
  $definition_nid = sirius_contact_definition_get_nid($domain_nid, TRUE);
	$contact_definition_json = sirius_contact_definition();

	$form['survey'] = json_decode($contact_definition_json['form_encoded']['default_evaluation_form'], TRUE);
	$form['#tree'] = TRUE;

	$form['!intro'] = array(
		'#markup' => '<div class="sirius_form_intro">' . '<div class="sirius-form">',
		'#weight' => -9999,
	);
	
	$intro = sirius_domain_variable_get('sirius_training_evaluations_form_intro', '');
	if ($intro['value']) {
		$form['intro'] = array(
			'#markup' => $intro['value'],
			'#weight' => -9998,
		);
	}

	$form['!outro'] = array(
		'#markup' => '</div>',
		'#weight' => 9999,
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
		'#weight' => 10000,
	);

	if ($form_state['results']) {
		$form['results'] = array('#markup' => $form_state['results']);
	}

	return $form;
}

function sirius_training_evaluation_evaluate_form_submit($form, &$form_state) {
	$worker_id = $form_state['worker_id'];
	$worker_node = sirius_worker_get_by_id($worker_id);

	$domain_nid = sirius_domain_nid();
  $definition_nid = sirius_contact_definition_get_nid($domain_nid, TRUE);
	$contact_definition_json = sirius_contact_definition();

	$log_json = array();
	$form = $contact_definition_json['form']['default_evaluation_form'];
	$values = $form_state['values']['survey'];
	$log_json['evaluation']['form_encoded'] = json_encode($form);
	$log_json['evaluation']['results_encoded'] = json_encode($values);
	sirius_log(
		'worker:evaluation', 
		'draft', 
		'Evaluation submitted', 
		NULL, 
		NULL, 
		array($worker_node->nid), 
		NULL,
		$log_json
	);

	drupal_set_message(t('Evaluation submitted: ') . $worker_id . ' - ' . $worker_node->title);
	drupal_goto('sirius/training/evaluation');
	// $form_state['rebuild'] = TRUE;
}
