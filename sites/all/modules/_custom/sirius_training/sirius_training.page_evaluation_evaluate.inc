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

	$contact_definition_json = sirius_contact_definition();
	$form['#tree'] = TRUE;
	$form['survey'] = sirius_jsonfield_util_form_get($contact_definition_json, 'default_evaluation_form');

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
	$contact_definition_json = sirius_contact_definition();

	// Save the evaluation.
	$json = array();
	$json_form = sirius_jsonfield_util_form_get($contact_definition_json, 'default_evaluation_form');
	$json_results = $form_state['values']['survey'];
	sirius_jsonfield_util_form_set($json, 'evaluation', $json_form);
	sirius_jsonfield_util_results_set($json, 'evaluation', $json_results);
	sirius_log(
		'worker:evaluation', 
		'draft', 
		'Evaluation submitted', 
		NULL, 
		NULL, 
		array($worker_node->nid), 
		NULL,
		$json
	);

	// Notify the worker
	global $user;
	$user_full = user_load($user->uid);
	if ($user_full->field_sirius_worker['und'][0]['target_id']) {
		$author_worker_node = node_load($user_full->field_sirius_worker['und'][0]['target_id']);
		$author_name = $author_worker_node->title;
	} else {
		$author_name = $user->name;
	}

	$subject = "An evaulation has been submitted on your behalf";
	$body = "An evaluation on your behalf has been submitted to the training center by $author_name.";
	$sms = "An evaluation on your behalf has been submitted to the training center by $author_name.";
	sirius_worker_notify(
		$worker_node, 
		array(
			'subject' => $subject,
			'body' => $body,
			'sms' => $sms,
		)
	);

	drupal_set_message(t('Evaluation submitted: ') . $worker_id . ' - ' . $worker_node->title);
	drupal_goto('sirius/training/evaluation');
	// $form_state['rebuild'] = TRUE;
}
