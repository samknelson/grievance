<?php

function sirius_training_certification_add_access($worker_nid, $definition_nid, $participant_nid = NULL) {
	// certifications must be enabled
	if (sirius_domain_variable_get('sirius_training_certifications_enabled', '') != 'Yes') { return FALSE; }

	// Verify the participant
	if ($participant_nid) {
		$participant_node = node_load($participant_nid);
		if ($participant_node->type != 'sirius_event_participant') { return FALSE; }
	}
	/*
	// Must have one of the roles permitted to submit evaluations
	$roles = sirius_domain_variable_get('sirius_training_evaluations_roles');
	$evaluation_rids = array();
	foreach ($roles as $rid => $value) { if ($value) { $evaluation_rids[] = $value; } }
	global $user;
	$my_rids = array_keys($user->roles);
	if (array_intersect($my_rids, $evaluation_rids)) { return TRUE; }
	*/

	// Verify the worker
	$worker_node = node_load($worker_nid);
	if (!$worker_node) { return FALSE; }
	if ($worker_node->type != 'sirius_worker') { return FALSE; }

	// Verify the worker
	$definition_node = node_load($definition_nid);
	if (!$definition_node) { return FALSE; }
	if ($definition_node->type != 'sirius_json_definition') { return FALSE; }
	if ($definition_node->field_sirius_type['und'][0]['value'] != 'sirius_training:certification') { return FALSE; }

	// Verify the domain
	$domain_nid = sirius_domain_nid();
	if ($domain_nid != $worker_node->field_sirius_domain['und'][0]['target_id']) { return FALSE; }
	if ($domain_nid != $definition_node->field_sirius_domain['und'][0]['target_id']) { return FALSE; }

	return TRUE;
}

function sirius_training_certification_add($worker_nid, $definition_nid, $participant_nid = NULL) {
	return drupal_get_form('sirius_training_certification_add_form', $worker_nid, $definition_nid, $participant_nid);
}

function sirius_training_certification_add_form($form, &$form_state, $worker_nid, $definition_nid, $participant_nid = NULL) {
	$form_state['worker_nid'] = $worker_nid;
	$form_state['definition_nid'] = $definition_nid;
	$form_state['participant_nid'] = $participant_nid;

	$worker_node = node_load($worker_nid);
	$definition_node = node_load($definition_nid);

	drupal_set_title('Submit ' . $definition_node->title . ' For: ' . $worker_node->field_sirius_id['und'][0]['value'] . ' - ' . $worker_node->title);

	$definition_json = sirius_jsonfield_util_node_json_get($definition_node);
	$form['#tree'] = TRUE;
	$form['survey'] = sirius_jsonfield_util_form_get($definition_json, 'default');

	$form['!intro'] = array(
		'#markup' => '<div class="sirius_form_intro">' . '<div class="sirius-form">',
		'#weight' => -9999,
	);
	
	$intro = sirius_domain_variable_get('sirius_training_certifications_form_intro', '');
	if ($intro && $intro['value']) {
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

function sirius_training_certification_add_form_submit($form, &$form_state) {
	$worker_nid = $form_state['worker_nid'];
	$worker_node = node_load($worker_nid);
	$definition_nid = $form_state['definition_nid'];
	$definition_node = node_load($definition_nid);
	$definition_json = sirius_jsonfield_util_node_json_get($definition_node);
	$participant_nid = $form_state['participant_nid'];
	$participant_node = node_load($participant_nid);
	if ($participant_node) {
		$event_nid = $participant_node->field_sirius_event['und'][0]['target_id'];
		$event_node = node_load($event_nid);
	}

	// Save the evaluation.
	$json = array();
	$json_form = sirius_jsonfield_util_form_get($definition_json, 'default');
	$json_results = $form_state['values']['survey'];
	sirius_jsonfield_util_form_set($json, 'certification', $json_form);
	sirius_jsonfield_util_results_set($json, 'certification', $json_results);
	$handlers = array($worker_nid);
	$handlers[] = $definition_nid;
	if ($participant_nid) { $handlers[] = $participant_nid; }
	$certification_node = sirius_log(
		'worker:certification', 
		'draft', 
		'Certification submitted', 
		NULL, 
		NULL, 
		$handlers, 
		NULL,
		$json
	);

	// Notify the worker
	/*
	global $user;
	$user_full = user_load($user->uid);
	if ($user_full->field_sirius_worker['und'][0]['target_id']) {
		$author_worker_node = node_load($user_full->field_sirius_worker['und'][0]['target_id']);
		$author_name = $author_worker_node->title;
	} else {
		$author_name = $user->name;
	}

	$subject = "A certification has been submitted on your behalf";
	$body = "A certification on your behalf has been submitted to the training center by $author_name.";
	$sms = "A certification on your behalf has been submitted to the training center by $author_name.";
	sirius_worker_notify(
		$worker_node, 
		array(
			'subject' => $subject,
			'body' => $body,
			'sms' => $sms,
		)
	);
	*/

	drupal_set_message(t('Certification submitted: ') . $worker_node->field_sirius_id['und'][0]['value'] . ' - ' . $worker_node->title);
	drupal_goto('node/' . $certification_node->nid . '/sirius_log_tc/certification_signatures');
}
