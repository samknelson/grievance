<?php

function sirius_node_tab_event_participant_definition_followup_access($survey_node) {
	if ($survey_node->field_sirius_type['und'][0]['value'] != 'node:sirius_event_participant:field_sirius_json') { return FALSE; }
	return TRUE;
}

function sirius_node_tab_event_participant_definition_followup($survey_node) {
  sirius_event_disclaimer_redirect($event_node);
	return drupal_get_form('sirius_node_tab_event_participant_definition_followup_form', $survey_node->nid);
}

function sirius_node_tab_event_participant_definition_followup_form($form, &$form_state, $survey_nid) {
  drupal_add_css(drupal_get_path('module', 'sirius_event') . '/css/event.css', 'file');

	$form_state['survey_nid'] = $survey_nid;
	$survey_node = node_load($survey_nid);
	$survey_json = sirius_jsonfield_util_node_json_get($survey_node);

	$form['#tree'] = TRUE;

	$form['followup']['enabled'] = array(
		'#title' => t('Enabled?'),
		'#type' => 'select',
		'#options' => array(
			NULL => t('-- Select --'),
			'Yes' => t('Yes'),
			'No' => t('No'),
		),
		'#default_value' => $survey_json['followup']['enabled'],
		'#description' => t('Should a "Followup" option be available to callers on a phone bank?'),
	);

	$form['followup']['enabled_sms'] = array(
		'#title' => t('Enabled for SMS?'),
		'#type' => 'select',
		'#options' => array(
			NULL => t('-- Select --'),
			'Yes' => t('Yes'),
			'No' => t('No'),
		),
		'#default_value' => $survey_json['followup']['enabled_sms'],
		'#description' => t('Allow SMS (text message) followups'),
	);

	$form['followup']['enabled_email'] = array(
		'#title' => t('Enabled for Email?'),
		'#type' => 'select',
		'#options' => array(
			NULL => t('-- Select --'),
			'Yes' => t('Yes'),
			'No' => t('No'),
		),
		'#default_value' => $survey_json['followup']['enabled_email'],
		'#description' => t('Allow email followups'),
	);

	$instructions_default = $survey_json['followup']['instructions'];
	if (!$instructions_default) { $instructions_default = 'Would you like us to send you additional information by text message?'; }
	$form['followup']['instructions'] = array(
		'#title' => t('Instructions for Volunteers'),
		'#type' => 'textarea',
		'#default_value' => $instructions_default,
		'#description' => t('What instructions should appear for volunteers? These instructions will appear at the end of the survey.'),
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_node_tab_event_participant_definition_followup_form_submit($form, &$form_state) {
	$survey_nid = $form_state['survey_nid'];
	$survey_node = node_load($survey_nid);
	$survey_json = sirius_json_get($survey_node);
	$survey_json['followup'] = $form_state['values']['followup'];
	sirius_json_set($survey_node, $survey_json);
	node_save($survey_node);
	drupal_set_message("Settings saved.");
}
