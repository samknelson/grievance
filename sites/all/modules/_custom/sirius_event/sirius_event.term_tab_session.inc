<?php

function sirius_event_term_tab_session($event_type_term) {
	return drupal_get_form('sirius_event_term_tab_session_form', $event_type_term);
}

function sirius_event_term_tab_session_form($form, &$form_state, $event_type_term) {
	$form_state['event_type_tid'] = $event_type_term->tid;
	$json = sirius_jsonfield_util_term_json_get($event_type_term);
	$session_json = $json['session'];

	$prole_tids = sirius_fieldvals($event_type_term, 'field_sirius_event_proles', 'tid');
	$prole_terms = taxonomy_term_load_multiple($prole_tids);
	$prole_options = array();
	foreach ($prole_terms as $term) {
		$prole_options[$term->tid] = $term->name;
	}
	$prole_tids = array_keys($prole_terms);
	$pstatus_options = array(NULL => t('-- Select --')) + sirius_event_participant_role_status_tids($prole_tids);

	$form['#tree'] = TRUE;
	$form['session']['enabled'] = array(
		'#type' => 'select',
		'#title' => t('Sessions Enabled?'),
		'#options' => array('No' => t('No'), 'Yes' => t('Yes')),
		'#description' => t('Does this event type support sessions?'),
		'#default_value' => $session_json['enabled'],
	);

	$form['session']['prole_tids'] = array(
		'#type' => 'checkboxes',
		'#title' => t('Partipant Roles'),
		'#options' => $prole_options,
		'#description' => t('Sessions are required by these event roles'),
	);
	if ($session_json['prole_tids']) { $form['session']['prole_tids']['#default_value'] = $session_json['prole_tids']; }

	$form['session']['admin_prole_tids'] = array(
		'#type' => 'checkboxes',
		'#title' => t('Admin Roles'),
		'#options' => $prole_options,
		'#description' => t('These roles have permission to view and set sessions for other event participants.'),
	);
	if ($session_json['admin_prole_tids']) { $form['session']['admin_prole_tids']['#default_value'] = $session_json['admin_prole_tids']; }

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_event_term_tab_session_form_submit($form, &$form_state) {
	$event_type_tid = $form_state['event_type_tid'];
	$event_type_term = taxonomy_term_load($event_type_tid);
	$json = json_decode($event_type_term->field_sirius_json['und'][0]['value'], TRUE);
	$json['session'] = $form_state['values']['session'];
	$event_type_term->field_sirius_json['und'][0]['value'] = json_encode($json);
	taxonomy_term_save($event_type_term);
	drupal_set_message("Settings saved.");
}

