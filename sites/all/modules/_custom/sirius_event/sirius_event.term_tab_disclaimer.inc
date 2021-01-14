<?php

/*
function sirius_event_term_tab_disclaimer_access($event_type_term) {
	return TRUE;
}
*/

function sirius_event_term_tab_disclaimer($event_type_term) {
	return drupal_get_form('sirius_event_term_tab_disclaimer_form', $event_type_term);
}

function sirius_event_term_tab_disclaimer_form($form, &$form_state, $event_type_term) {
	$form_state['event_type_tid'] = $event_type_term->tid;
	$json = sirius_jsonfield_util_term_json_get($event_type_term);
	$disclaimer_json = $json['disclaimer'];

	$prole_tids = sirius_fieldvals($event_type_term, 'field_sirius_event_proles', 'tid');
	$prole_terms = taxonomy_term_load_multiple($prole_tids);
	$prole_options = array();
	foreach ($prole_terms as $term) {
		$prole_options[$term->tid] = $term->name;
	}
	$prole_tids = array_keys($prole_terms);
	$pstatus_options = array(NULL => t('-- Select --')) + sirius_event_participant_role_status_tids($prole_tids);

	$form['#tree'] = TRUE;
	$form['disclaimer']['enabled'] = array(
		'#type' => 'select',
		'#title' => t('Disclaimer Enabled?'),
		'#options' => array('No' => t('No'), 'Yes' => t('Yes')),
		'#description' => t('Does this event type have a disclaimer attached?'),
		'#default_value' => $disclaimer_json['enabled'],
	);

	$form['disclaimer']['prole_tids'] = array(
		'#type' => 'checkboxes',
		'#title' => t('Partipant Roles'),
		'#options' => $prole_options,
		'#description' => t('Disclaimers are required by these event roles'),
	);
	if ($disclaimer_json['prole_tids']) { $form['disclaimer']['prole_tids']['#default_value'] = $disclaimer_json['prole_tids']; }

	$form['disclaimer']['admin_prole_tids'] = array(
		'#type' => 'checkboxes',
		'#title' => t('Admin Roles'),
		'#options' => $prole_options,
		'#description' => t('These roles have permission to view and set disclaimers for other event participants.'),
	);
	if ($disclaimer_json['admin_prole_tids']) { $form['disclaimer']['admin_prole_tids']['#default_value'] = $disclaimer_json['admin_prole_tids']; }

	$form['disclaimer']['nids'] = array(
		'#title' => 'Disclaimers',
		'#options' => sirius_disclaimer_list(),
		'#type' => 'checkboxes',
	);
	if ($disclaimer_json['nids']) { $form['disclaimer']['nids']['#default_value'] = $disclaimer_json['nids']; }

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_event_term_tab_disclaimer_form_submit($form, &$form_state) {
	$event_type_tid = $form_state['event_type_tid'];
	$event_type_term = taxonomy_term_load($event_type_tid);
	$json = json_decode($event_type_term->field_sirius_json['und'][0]['value'], TRUE);
	$json['disclaimer'] = $form_state['values']['disclaimer'];
	$event_type_term->field_sirius_json['und'][0]['value'] = json_encode($json);
	taxonomy_term_save($event_type_term);
	drupal_set_message("Settings saved.");
}

