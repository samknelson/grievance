<?php

function sirius_node_tab_event_participant_json_access($participant_node) {
	$event_nid = $participant_node->field_sirius_event['und'][0]['target_id'];
	if (!$event_nid) { drupal_set_message("No event."); return FALSE; }
	$event_node = node_load($event_nid);
	$survey_nid = $event_node->field_sirius_json_definition['und'][0]['target_id'];
	$survey_node = node_load($survey_nid);
	if (!$survey_node) { drupal_set_message("No survey."); return FALSE; }
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
	$form['survey'] = json_decode($survey_node->field_sirius_json['und'][0]['value'], TRUE)['form']['default'];
	$form['#tree'] = TRUE;

	$json_defaults = json_decode($participant_node->field_sirius_json['und'][0]['value'], TRUE);
	sirius_jsonfield_util_set_defaults($form['survey'], $json_defaults['survey']);

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
	$json = json_decode($participant_node->field_sirius_json['und'][0]['value'], TRUE);
	$json['survey'] = $form_state['values']['survey'];
	sirius_jsonfield_log($participant_node, 'survey', 'Survey updated.');
	$participant_node->field_sirius_json['und'][0]['value'] = json_encode($json);
	node_save($participant_node);
}