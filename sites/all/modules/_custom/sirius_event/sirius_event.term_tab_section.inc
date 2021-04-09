<?php

function sirius_event_term_tab_section($event_type_term) {
	return drupal_get_form('sirius_event_term_tab_section_form', $event_type_term);
}

function sirius_event_term_tab_section_form($form, &$form_state, $event_type_term) {
	$form_state['event_type_tid'] = $event_type_term->tid;
	$json = sirius_jsonfield_util_term_json_get($event_type_term);
	$section_json = $json['section'];

	$prole_tids = sirius_fieldvals($event_type_term, 'field_sirius_event_proles', 'tid');
	$prole_terms = taxonomy_term_load_multiple($prole_tids);
	$prole_options = array();
	foreach ($prole_terms as $term) {
		$prole_options[$term->tid] = $term->name;
	}
	$prole_tids = array_keys($prole_terms);
	$pstatus_options = array(NULL => t('-- Select --')) + sirius_event_participant_role_status_tids($prole_tids);

	$form['#tree'] = TRUE;
	$form['section']['enabled'] = array(
		'#type' => 'select',
		'#title' => t('sections Enabled?'),
		'#options' => array('No' => t('No'), 'Yes' => t('Yes')),
		'#description' => t('Does this event type support sections?'),
		'#default_value' => $section_json['enabled'],
	);

	$form['section']['section_alias'] = array(
		'#type' => 'textfield',
		'#title' => t('Section Alias'),
		'#description' => t('Sometimes it is useful to refer to sections by an alternative name, such as "Appointment Time".'),
		'#default_value' => $section_json['section_alias'],
	);

	$form['section']['intro'] = array(
		'#type' => 'text_format',
		'#title' => t('Introductory Text'),
		'#description' => t('Enter any introductory text that should appear to non-administrative users before the section select box.'),
		'#default_value' => $section_json['intro'],
		'#format' => 'sirius_form_editor',
	);

	$form['section']['prole_tids'] = array(
		'#type' => 'checkboxes',
		'#title' => t('Partipant Roles'),
		'#options' => $prole_options,
		'#description' => t('sections are required by these event roles'),
	);
	if ($section_json['prole_tids']) { $form['section']['prole_tids']['#default_value'] = $section_json['prole_tids']; }

	$form['section']['admin_prole_tids'] = array(
		'#type' => 'checkboxes',
		'#title' => t('Admin Roles'),
		'#options' => $prole_options,
		'#description' => t('These roles have permission to view and set sections for other event participants.'),
	);
	if ($section_json['admin_prole_tids']) { $form['section']['admin_prole_tids']['#default_value'] = $section_json['admin_prole_tids']; }

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_event_term_tab_section_form_submit($form, &$form_state) {
	$event_type_tid = $form_state['event_type_tid'];
	$event_type_term = taxonomy_term_load($event_type_tid);
	$json = json_decode($event_type_term->field_sirius_json['und'][0]['value'], TRUE);
	$json['section'] = $form_state['values']['section'];
	$json['section']['intro'] = sirius_richtext_handle_input($form_state['values']['section']['intro']);
	$event_type_term->field_sirius_json['und'][0]['value'] = json_encode($json);
	taxonomy_term_save($event_type_term);
	drupal_set_message("Settings saved.");
}

