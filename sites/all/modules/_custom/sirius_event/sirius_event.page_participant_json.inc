<?php

function sirius_node_tab_event_participant_json_access($participant_node) {
	$event_nid = $participant_node->field_sirius_event['und'][0]['target_id'];
	if (!$event_nid) { return FALSE; }
	$event_node = node_load($event_nid);
	$survey_nid = $event_node->field_sirius_json_definition['und'][0]['target_id'];
	$survey_node = node_load($survey_nid);
	if (!$survey_node) { return FALSE; }
	return TRUE;
}

function sirius_node_tab_event_participant_json($participant_node) {
	return drupal_get_form('sirius_node_tab_event_participant_json_form', $participant_node);
}

function sirius_node_tab_event_participant_json_form($form, &$form_state, $participant_node) {
	$form_state['participant_nid'] = $participant_node->nid;
	$event_nid = $participant_node->field_sirius_event['und'][0]['target_id'];
	$event_node = node_load($event_nid);
	$survey_nid = $event_node->field_sirius_json_definition['und'][0]['target_id'];
	$survey_node = node_load($survey_nid);
	$survey_json = sirius_jsonfield_util_node_json_get($survey_node);
	$survey_form = sirius_jsonfield_util_form_get($survey_json, 'default');

	$participant_json = sirius_jsonfield_util_node_json_get($participant_node);
	$participant_results = sirius_jsonfield_util_results_get($participant_json, 'survey');
	sirius_jsonfield_util_set_defaults($survey_form, $participant_results);

	$form['#tree'] = TRUE;
	$form['survey'] = $survey_form;

	$form['!intro'] = array(
		'#markup' => '<div class="sirius_form_intro">' . 
		'<div class="sirius-form">',
		'#weight' => -9999,
	);

	$form['!outro'] = array(
		'#markup' => '</div>',
		'#weight' => 9999,
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
		'#weight' => 10000,
	);

	return $form;
}

function sirius_node_tab_event_participant_json_form_submit($form, &$form_state) {
	$participant_node = node_load($form_state['participant_nid']);
	$participant_json = sirius_jsonfield_util_node_json_get($participant_node);
	sirius_jsonfield_util_results_set($participant_json, 'survey', $form_state['values']['survey']);
	sirius_jsonfield_util_node_json_set($participant_node, $participant_json);
	sirius_jsonfield_log($participant_node, 'survey', 'Survey updated.');
	node_save($participant_node);
}