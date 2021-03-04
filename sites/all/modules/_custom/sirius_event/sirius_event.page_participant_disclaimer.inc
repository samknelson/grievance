<?php

function sirius_event_node_tab_participant_disclaimer($participant_node) {
	return drupal_get_form('sirius_event_node_tab_participant_disclaimer_form', $participant_node);
}

function sirius_event_node_tab_participant_disclaimer_form($form, &$form_state, $participant_node) {
	$event_node = node_load($participant_node->field_sirius_event['und'][0]['target_id']);
	$form_state['event_nid'] = $event_node->nid;
	$form_state['participant_nid'] = $participant_node->nid;
	$form['#tree'] = TRUE;

	$participant_json = sirius_jsonfield_util_node_json_get($participant_node);
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

function sirius_event_node_tab_participant_disclaimer_form_submit($form, &$form_state) {
	$participant_node = node_load($form_state['participant_nid']);
	$disclaimer_nodes = node_load_multiple(array_keys($form_state['values']['disclaimer']));
	sirius_disclaimer_accept($participant_node, $disclaimer_nodes, array('skip_notify' => TRUE));
	drupal_set_message("Response saved.");
}


function sirius_event_node_tab_participant_disclaimer_revoke($participant_node) {
	return drupal_get_form('sirius_event_node_tab_participant_disclaimer_revoke_form', $participant_node);
}

function sirius_event_node_tab_participant_disclaimer_revoke_form($form, &$form_state, $participant_node) {
	$event_node = node_load($participant_node->field_sirius_event['und'][0]['target_id']);
	$form_state['event_nid'] = $event_node->nid;
	$form_state['participant_nid'] = $participant_node->nid;
	$form['#tree'] = TRUE;

	$participant_json = sirius_jsonfield_util_node_json_get($participant_node);
	$event_disclaimer_json = sirius_event_disclaimer_json($event_node);
	$disclaimer_nodes = sirius_event_disclaimer_nodes($event_disclaimer_json);

	$form['confirm'] = array(
		'#type' => 'checkbox',
		'#required' => TRUE,
		'#title' => t('Yes, revoke all disclaimer acceptances for this participant.'),
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_event_node_tab_participant_disclaimer_revoke_form_submit($form, &$form_state) {
	$participant_node = node_load($form_state['participant_nid']);
	$participant_json = sirius_jsonfield_util_node_json_get($participant_node);
	unset($participant_json['disclaimer']);
	sirius_jsonfield_util_node_json_set($participant_node, $participant_json);
	node_save($participant_node);
	drupal_set_message("All disclaimers have been revoked.");
}