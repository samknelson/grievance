<?php
function sirius_node_tab_event_map_settings_access($event_node) {
	// if (!sirius_node_tab_event_map_access($event_node)) { return FALSE; } 
	if (user_access('sirius admin') || user_access('sirius domain admin') || user_access('sirius event admin')) { return TRUE; }

	$participant_nid = sirius_event_participant_nid_from_user($event_node->nid);
	if (!$participant_nid) { return FALSE; }
	$participant_node = node_load($participant_nid);
	$type_json = sirius_event_type_json($event_node);
	$prole_tid = $participant_node->field_sirius_event_prole['und'][0]['tid'];
	$admin_prole_tids = array_values($type_json['phonebank']['admin_prole_tids']);
	if (!in_array($prole_tid, $admin_prole_tids)) { return FALSE; }

	return TRUE;
}

function sirius_node_tab_event_map_settings($event_node) {
	return drupal_get_form('sirius_node_tab_event_map_settings_form', $event_node->nid);
}

function sirius_node_tab_event_map_settings_form($form, &$form_state, $event_nid) {
	$form_state['event_nid'] = $event_nid;
	$event_node = node_load($event_nid);
	$json = json_decode($event_node->field_sirius_json['und'][0]['value'], TRUE);

	$event_type_term = taxonomy_term_load($event_node->field_sirius_event_type['und'][0]['tid']);
	$prole_tids = sirius_fieldvals($event_type_term, 'field_sirius_event_proles', 'tid');

	$form['#tree'] = TRUE;

	$form['map']['caller_intro'] = array(
		'#type' => 'text_format',
		'#title' => 'Introduction for callers',
		'#default_value' => $json['map']['caller_intro'],
		'#description' => t('Callers will receive this message on the landing page of the map.'),
		'#format' => 'sirius_form_editor',
		'#allowed_formats' => array('sirius_form_editor'),
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_node_tab_event_map_settings_form_submit($form, &$form_state) {
	$event_nid = $form_state['event_nid'];
	$event_node = node_load($event_nid);
	$json = sirius_jsonfield_util_node_json_get($event_node);
	$json['map'] = $form_state['values']['map'];
	$json['map']['caller_intro'] = sirius_richtext_handle_input($form_state['values']['map']['caller_intro']);
	sirius_jsonfield_util_node_json_set($event_node, $json);
	node_save($event_node);
	drupal_set_message(t('Settings saved.'));
}