<?php

function sirius_node_tab_event_section_settings_access($event_node) {
	// If it's disabled at the event type level, not enabled
	if (!sirius_event_section_enabled_tid($event_node->field_sirius_event_type['und'][0]['tid'])) { return FALSE; }

	// Admin user roles can access
	if (user_access('sirius admin') || user_access('sirius domain admin') || user_access('sirius event admin')) { return TRUE; }

	// Admin participant roles can access
	$participant_nid = sirius_event_participant_nid_from_user($event_node->nid);
	if (!$participant_nid) { return FALSE; }
	$participant_node = node_load($participant_nid);
	$type_json = sirius_event_type_json($event_node);
	$event_json = sirius_jsonfield_util_node_json_get($event_node);
	$prole_tid = $participant_node->field_sirius_event_prole['und'][0]['tid'];
	$admin_prole_tids = array_values($type_json['section']['admin_prole_tids']);
	if (in_array($prole_tid, $admin_prole_tids)) { return TRUE; }

	// Oh well
	return FALSE;
}

function sirius_node_tab_event_section_settings($event_node) {
  sirius_event_disclaimer_redirect($event_node);
 	return drupal_get_form('sirius_node_tab_event_section_settings_form', $event_node);
}

function sirius_node_tab_event_section_settings_form($form, &$form_state, $event_node) {
	$form_state['event_nid'] = $event_node->nid;
	$event_type_term = taxonomy_term_load($event_node->field_sirius_event_type['und'][0]['tid']);
	$prole_tids = sirius_fieldvals($event_type_term, 'field_sirius_event_proles', 'tid');
	$prole_terms = taxonomy_term_load_multiple($prole_tids);
	$prole_options = array();
	foreach ($prole_terms as $term) {
		$prole_options[$term->tid] = $term->name;
	}
	$prole_tids = array_keys($prole_terms);
	$pstatus_options = array(NULL => t('-- Select --')) + sirius_event_participant_role_status_tids($prole_tids);
	$json = sirius_jsonfield_util_node_json_get($event_node);
	$section_json = $json['section'];

	$form['#tree'] = TRUE;
	$form['section']['enabled'] = array(
		'#type' => 'select',
		'#title' => t('Sections Enabled?'),
		'#options' => array('No' => t('No'), 'Yes' => t('Yes')),
		'#description' => t('Does this event type support sections?'),
		'#default_value' => $section_json['enabled'],
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

	$form['section']['limit'] = array(
		'#type' => 'textfield',
		'#title' => t('Number of participants per section'),
		'#description' => t('Enter the maximum number of participants per section. Individual sections may set higher or lower limits.'),
		'#size' => 4,
		'#default_value' => 1,
	);
	if ($section_json['limit']) { $form['section']['limit']['#default_value'] = $section_json['limit']; }

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_node_tab_event_section_settings_form_submit($form, &$form_state) {
	$event_node = node_load($form_state['event_nid']);
	$json = sirius_jsonfield_util_node_json_get($event_node);
	$json['section'] = $form_state['values']['section'];
	sirius_jsonfield_util_node_json_set($event_node, $json);
	node_save($event_node);
	drupal_set_message("Settings saved.");
}