<?php

function sirius_node_tab_event_disclaimer_settings_access($event_node) {
	// If it's disabled at the event type level, no disclaimer
	if (!sirius_event_disclaimer_enabled_tid($event_node->field_sirius_event_type['und'][0]['tid'])) { return FALSE; }

	// Admin user roles can access
	if (user_access('sirius admin') || user_access('sirius domain admin') || user_access('sirius event admin')) { return TRUE; }

	// Admin participant roles can access
	$participant_nid = sirius_event_participant_nid_from_user($event_node->nid);
	if (!$participant_nid) { return FALSE; }
	$participant_node = node_load($participant_nid);
	$type_json = sirius_event_type_json($event_node);
	$event_json = sirius_jsonfield_util_node_json_get($event_node);
	$prole_tid = $participant_node->field_sirius_event_prole['und'][0]['tid'];
	$admin_prole_tids = array_values($type_json['disclaimer']['admin_prole_tids']);
	if (in_array($prole_tid, $admin_prole_tids)) { return TRUE; }

	// Oh well
	return FALSE;
}

function sirius_node_tab_event_disclaimer_settings($event_node) {
  sirius_event_disclaimer_redirect($event_node);
	return drupal_get_form('sirius_node_tab_event_disclaimer_settings_form', $event_node);
}

function sirius_node_tab_event_disclaimer_settings_form($form, &$form_state, $event_node) {
	$form_state['event_nid'] = $event_node->nid;
	$event_type_term = taxonomy_term_load($event_node->field_sirius_event_type['und'][0]['tid']);
	$prole_tids = sirius_fieldvals($event_type_term, 'field_sirius_event_proles', 'tid');
	$prole_terms = taxonomy_term_load_multiple($prole_tids);
	$prole_options = array();
	foreach ($prole_terms as $term) { $prole_options[$term->tid] = $term->name; }
	$prole_tids = array_keys($prole_terms);

	$disclaimer_json = sirius_event_disclaimer_json($event_node);
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

function sirius_node_tab_event_disclaimer_settings_form_submit($form, &$form_state) {
	$event_node = node_load($form_state['event_nid']);
	$json = sirius_jsonfield_util_node_json_get($event_node);
	$json['disclaimer'] = $form_state['values']['disclaimer'];
	$json['disclaimer']['body'] = sirius_richtext_handle_input($form_state['values']['disclaimer']['body']);
	sirius_jsonfield_util_node_json_set($event_node, $json);
	node_save($event_node);
	drupal_set_message("Settings saved.");
}