<?php

function sirius_event_node_tab_participant_comments($participant_node) {
	return drupal_get_form('sirius_event_node_tab_participant_comments_form', $participant_node);
}

function sirius_event_node_tab_participant_comments_form($form, &$form_state, $participant_node) {
	$form_state['participant_nid'] = $participant_node->nid;

	$participant_json = sirius_jsonfield_util_node_json_get($participant_node);

	$form['#tree'] = TRUE;
	$form['participant']['comments'] = array(
		'#type' => 'textarea',
		'#title' => 'Comments',
		'#default_value' => $participant_json['comments'],
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_event_node_tab_participant_comments_form_submit($form, &$form_state) {
	$participant_node = node_load($form_state['participant_nid']);
	$participant_json = sirius_jsonfield_util_node_json_get($participant_node);
	$participant_json['comments'] = $form_state['values']['participant']['comments'];
	sirius_jsonfield_util_node_json_set($participant_node, $participant_json);
	node_save($participant_node);
}
