<?php

function sirius_event_term_tab_pstatus($pstatus_term) {
	return drupal_get_form('sirius_event_term_tab_pstatus_form', $pstatus_term);
}

function sirius_event_term_tab_pstatus_form($form, &$form_state, $pstatus_term) {
	$form_state['pstatus_tid'] = $pstatus_term->tid;
	$json = sirius_jsonfield_util_term_json_get($pstatus_term);

	$form['#tree'] = TRUE;

	$form['schedule']['display'] = array(
		'#type' => 'fieldset',
		'#title' => t('Schedule Display'),
		'#description' => t('The settings below indicate how participants with this status should be rendered when their schedule is displayed. For instance, if a worker has status "registered" for a class, you might want to show them on the schedule as "In Class"; but if their participant status is "canceled", you might prefer for that event not to appear on the schedule.'),
	);

	$form['schedule']['display']['active'] = array(
		'#title' => t('Display on the contact\'s schedule page?'),
		'#type' => 'select',
		'#options' => array(
			NULL => t('-- Select --'),
			'Yes' => t('Yes'),
			'No' => t('No'),
		),
		'#id' => 'schedule_display_active',
	);
	if (isset($json['schedule']['display']['active'])) { $form['schedule']['display']['active']['#default_value'] = $json['schedule']['display']['active']; }

	$form['schedule']['display']['title'] = array(
		'#title' => t('Display As (Full)'),
		'#type' => 'textfield',
 		'#states' => array('visible' => array(":input[id=\"schedule_display_active\"]" => array('value' => 'Yes'))),
	);
	if (isset($json['schedule']['display']['title'])) { $form['schedule']['display']['title']['#default_value'] = $json['schedule']['display']['title']; } 
	else { $form['schedule']['display']['title']['#default_value'] = $pstatus_term->name; }

	$form['schedule']['display']['title_short'] = array(
		'#title' => t('Display As (Short)'),
		'#type' => 'textfield',
		'#size' => 4,
 		'#states' => array('visible' => array(":input[id=\"schedule_display_active\"]" => array('value' => 'Yes'))),
	);
	if (isset($json['schedule']['display']['title_short'])) { $form['schedule']['display']['title_short']['#default_value'] = $json['schedule']['display']['title_short']; } 
	else { $form['schedule']['display']['title_short']['#default_value'] = substr($pstatus_term->name, 0, 4); }

	$form['phonebank'] = array(
		'#type' => 'fieldset',
		'#title' => t('Phonebank Display'),
		'#description' => t('The settings below indicate how this participant status should be handled in phonebanks.'),
	);

	$form['phonebank']['conceal_during_call'] = array(
		'#title' => t('Conceal During Call?'),
		'#type' => 'select',
		'#options' => array(
			NULL => t('-- Select --'),
			'Yes' => t('Yes'),
			'No' => t('No'),
		),
		'#description' => t('Select "Yes" to conceal this status from phone banker users who are making a call.'),
	);
	if (isset($json['phonebank']['conceal_during_call'])) { $form['phonebank']['conceal_during_call']['#default_value'] = $json['phonebank']['conceal_during_call']; }


	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => 'Submit',
	);

	return $form;
}

function sirius_event_term_tab_pstatus_form_submit($form, &$form_state) {
	$pstatus_term = taxonomy_term_load($form_state['pstatus_tid']);
	$json = sirius_jsonfield_util_term_json_get($pstatus_term);
	$json['schedule'] = $form_state['values']['schedule'];
	$json['phonebank'] = $form_state['values']['phonebank'];
	sirius_jsonfield_util_term_json_set($pstatus_term, $json);
	taxonomy_term_save($pstatus_term);

	drupal_set_message("Settings saved.");
}