<?php

function sirius_node_tab_event_participant_definition_followup_access($definition_node) {
	if ($definition_node->field_sirius_type['und'][0]['value'] != 'node:sirius_event_participant:field_sirius_json') { return FALSE; }
	return TRUE;
}

function sirius_node_tab_event_participant_definition_followup($definition_node) {
	return drupal_get_form('sirius_node_tab_event_participant_definition_followup_form', $definition_node->nid);
}

function sirius_node_tab_event_participant_definition_followup_form($form, &$form_state, $definition_nid) {
	$form_state['definition_nid'] = $definition_nid;
	$definition_node = node_load($definition_nid);
	$definition_json = sirius_jsonfield_util_node_json_get($definition_node);

	$form['#tree'] = TRUE;

	$form['followup']['enabled'] = array(
		'#title' => t('Enabled?'),
		'#type' => 'select',
		'#options' => array(
			NULL => t('-- Select --'),
			'Yes' => t('Yes'),
			'No' => t('No'),
		),
		'#default_value' => $definition_json['followup']['enabled'],
		'#description' => t('Should a "Followup" section be available to callers on a phone bank?'),
	);

	$instructions_default = $definition_json['followup']['instructions'];
	if (!$instructions_default) { $instructions_default = 'Would you like us to send you additional information by text message?'; }
	$form['followup']['instructions'] = array(
		'#title' => t('Instructions for Volunteers'),
		'#type' => 'textarea',
		'#default_value' => $instructions_default,
		'#description' => t('What instructions should appear for volunteers? These instructions will appear at the end of the survey.'),
	);

	$form['followup']['editable'] = array(
		'#title' => t('Editable?'),
		'#type' => 'select',
		'#options' => array(
			NULL => t('-- Select --'),
			'Yes' => t('Yes'),
			'No' => t('No'),
		),
		'#default_value' => $definition_json['followup']['editable'],
		'#description' => t('Should the caller have the ability to edit or personalize the message?'),
	);

	$form['followup']['options'][0]['sms'] = array(
		'#title' => t('Text Message'),
		'#type' => 'textarea',
		'#default_value' => $definition_json['followup']['options'][0]['sms'],
		'#description' => t('Enter the text message that will be sent to the person being called. Links may be included and will be rewritten into tracking links. Outgoing messages will come from the phone number <strong>@phone</strong>', array('@phone' => sirius_domain_variable_get('sirius_twilio_phone', ''))),
	);


	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_node_tab_event_participant_definition_followup_form_submit($form, &$form_state) {
	$definition_nid = $form_state['definition_nid'];
	$definition_node = node_load($definition_nid);
	$definition_json = sirius_jsonfield_util_node_json_get($definition_node);

	$definition_json['followup'] = $form_state['values']['followup'];

	sirius_jsonfield_util_node_json_set($definition_node, $definition_json);

	node_save($definition_node);

	drupal_set_message("Settings saved.");
}
