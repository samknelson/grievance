<?php

function sirius_event_term_tab_schedule($event_type_term) {
	return drupal_get_form('sirius_event_term_tab_schedule_form', $event_type_term);
}

function sirius_event_term_tab_schedule_form($form, &$form_state, $event_type_term) {
	$form_state['event_type_tid'] = $event_type_term->tid;
	$json = json_decode($event_type_term->field_sirius_json['und'][0]['value'], TRUE);

	$form['repeat'] = array(
		'#type' => 'select',
		'#title' => t('Can @name events have multiple dates and times?', array('@name' => $event_type_term->name)),
		'#options' => array(
			'Yes' => t('Yes'),
			'No' => t('No'),
		),
		'#default_value' => $json['schedule']['repeat'],
	);

	$form['duration'] = array(
		'#type' => 'select',
		'#title' => t('What duration options should be available?'),
		'#options' => sirius_event_duration_options(),
		'#multiple' => TRUE, 
		'#default_value' => $json['schedule']['duration'],
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_event_term_tab_schedule_form_submit($form, &$form_state) {
	$event_type_tid = $form_state['event_type_tid'];
	$event_type_term = taxonomy_term_load($event_type_tid);
	$json = json_decode($event_type_term->field_sirius_json['und'][0]['value'], TRUE);
	$json['schedule'] = $form_state['values'];
	$event_type_term->field_sirius_json['und'][0]['value'] = json_encode($json);
	taxonomy_term_save($event_type_term);
	drupal_set_message("Settings saved.");
}

