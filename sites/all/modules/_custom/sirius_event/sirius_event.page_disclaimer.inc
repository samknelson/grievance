<?php

function sirius_node_tab_event_disclaimer_view_access($event_node) {
	// If it's disabled at the event type level, no disclaimer
	if (!sirius_event_disclaimer_enabled_tid($event_node->field_sirius_event_type['und'][0]['tid'])) { return FALSE; }

	// Admins can get in even if the disclaimer is disabled at the event level
	if (sirius_node_tab_event_disclaimer_settings_access($event_node)) { return TRUE; }

	// Everybody else is excluded if the disclaimer is disabled at the event level
	if (!sirius_event_disclaimer_enabled($event_node)) { return FALSE; } 

	// Otherwise, you can access if you're in one of the suitable event roles
	$participant_nid = sirius_event_participant_nid_from_user($event_node->nid);
	if (!$participant_nid) { return FALSE; }
	$participant_node = node_load($participant_nid);
	$type_json = sirius_event_type_json($event_node);
	$event_json = sirius_jsonfield_util_node_json_get($event_node);
	$prole_tid = $participant_node->field_sirius_event_prole['und'][0]['tid'];
	$prole_tids = array_values($type_json['disclaimer']['prole_tids']);
	if (in_array($prole_tid, $prole_tids)) { return TRUE; }

	// Oh well
	return FALSE;
}

function sirius_node_tab_event_disclaimer_view($event_node) {
	return drupal_get_form('sirius_node_tab_event_disclaimer_view_form', $event_node);
}

function sirius_node_tab_event_disclaimer_view_form($form, &$form_state, $event_node) {
	$form_state['event_nid'] = $event_node->nid;
	$form['#tree'] = TRUE;

	$participant_nid = sirius_event_participant_nid_from_user($event_node->nid);
	$participant_node = node_load($participant_nid);
	$participant_json = sirius_jsonfield_util_node_json_get($participant_node);

	// Back door to revoke all acceptances. I'm using this basically just for dev purposes and it can go away any time.
	if (user_access('sirius domain admin') && $_REQUEST['clear']) { 
		unset($participant_json['disclaimer']);
		sirius_jsonfield_util_node_json_set($participant_node, $participant_json);
		node_save($participant_node);
	}

	$event_disclaimer_json = sirius_event_disclaimer_json($event_node);
	$disclaimer_nodes = sirius_event_disclaimer_nodes($event_disclaimer_json);

	if (!$disclaimer_nodes) {
		drupal_set_message("No disclaimers have been configured for this event.");
		return array();
	}

	$accepted_all = TRUE;
	foreach ($disclaimer_nodes as $disclaimer_nid => $disclaimer_node) {
		$disclaimer_json = sirius_jsonfield_util_node_json_get($disclaimer_node)['disclaimer'];
		$acceptance_json = $participant_json['disclaimer'][$disclaimer_nid]['acceptances'][0];
		if (!$acceptance_json) { $accepted_all = FALSE; }
		$form['disclaimer'][$disclaimer_nid] = sirius_disclaimer_render_form($disclaimer_json, $acceptance_json);			
	}

	if (!$accepted_all) {
		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit'),
		);
	}

	return $form;
}

function sirius_node_tab_event_disclaimer_view_form_submit($form, &$form_state) {
	$participant_nid = sirius_event_participant_nid_from_user($form_state['event_nid']);
	$participant_node = node_load($participant_nid);
	if (!$participant_node) {
		drupal_set_message("You are not registered for this event.", 'error');
		return;
	}

	$disclaimer_nodes = node_load_multiple(array_keys($form_state['values']['disclaimer']));
	sirius_disclaimer_accept($participant_node, $disclaimer_nodes, array());
	
	drupal_set_message("Response saved.");
}