<?php

function sirius_node_tab_event_session_access($event_node) {
	// If we can access the settings page, we're good
	if (sirius_node_tab_event_session_settings_access($event_node)) { return TRUE; }

	// If it's disabled, deny access
	if (!sirius_event_session_enabled($event_node)) { return FALSE; }

	// Get our participant role
	$participant_nid = sirius_event_participant_nid_from_user($event_node->nid);
	if (!$participant_nid) { return FALSE; }
	$participant_node = node_load($participant_nid);
	$prole_tid = $participant_node->field_sirius_event_prole['und'][0]['tid'];
	if (!$prole_tid) { return FALSE; }

	// Get available participant roles
	$session_json = sirius_event_session_json($event_node);
	if (!$session_json['prole_tids']) { return FALSE; }
	$prole_tids = array_filter(array_values($session_json['prole_tids']));

	// Are we one of the available participant roles?
	if (in_array($prole_tid, $prole_tids)) { return TRUE; }

	// Oh well
	return FALSE;
}

function sirius_node_tab_event_session($event_node) {
	return 'Pick a session...';
	return drupal_get_form('sirius_node_tab_event_session_form', $event_node);
}

function sirius_node_tab_event_session_form($form, &$form_state, $event_node) {
	$form_state['event_nid'] = $event_node->nid;
	$json = sirius_jsonfield_util_node_json_get($event_node);
	dpm($json);

	$form['#tree'] = TRUE;

	$form['session']['enabled'] = array(
		'#type' => 'select',
		'#title' => 'Enabled?',
		'#options' => array(
			'Yes' => t('Yes'),
			'No' => t('No'),
		),
		'#description' => t('Are sessions enabled for this event? If so, then participants should be assigned to a specific session, and should only attend that session.'),
		'#default_value' => $json['session']['enabled'],
	);

	$form['session']['max_per_session'] = array(
		'#type' => 'textfield',
		'#size' => 4,
		'#title' => 'Maximum Participants Per Session',
		'#default_value' => $json['session']['max_per_session'],
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_node_tab_event_session_form_submit($form, &$form_state) {
	$event_node = node_load($form_state['event_nid']);
	$json = sirius_jsonfield_util_node_json_get($event_node);
	$json['session'] = $form_state['values']['session'];
	sirius_jsonfield_util_node_json_set($event_node, $json);
	node_save($event_node);
	drupal_set_message("Sessions settings saved.");
}