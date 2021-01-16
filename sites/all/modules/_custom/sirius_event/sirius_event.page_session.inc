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
	return drupal_get_form('sirius_node_tab_event_session_form', $event_node);
}

function sirius_node_tab_event_session_form($form, &$form_state, $event_node) {
	$form_state['event_nid'] = $event_node->nid;
	$evemt_json = sirius_jsonfield_util_node_json_get($event_node);
	$counts = sirius_event_session_counts($event_node);	

	$participant_nid = sirius_event_participant_nid_from_user($event_node->nid);
	$participant_node = node_load($participant_nid);
	$participant_json = sirius_jsonfield_util_node_json_get($participant_node);

	foreach ($counts as $delta => $ts) {
		$options[$ts] = date('l, F j, g:i A', $ts);
	}

	$form['#tree'] = TRUE;

	$form['session']['ts'] = array(
		'#type' => 'select',
		'#options' => $options,
		'#title' => t('Session'),
		'#default_value' => $participant_json['session']['ts'],
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_node_tab_event_session_form_submit($form, &$form_state) {
	$event_node = node_load($form_state['event_nid']);
	$participant_nid = sirius_event_participant_nid_from_user($event_node->nid);
	$participant_node = node_load($participant_nid);
	if (!$participant_node) { 
		drupal_set_message("You are not registered for this event.", 'error'); 
		return;
	}
	$participant_json = sirius_jsonfield_util_node_json_get($participant_node);
	$participant_json['session'] = $form_state['values']['session'];
	sirius_jsonfield_util_node_json_set($participant_node, $participant_json);
	node_save($participant_node);
	drupal_set_message("Your selection has been saved.");
}