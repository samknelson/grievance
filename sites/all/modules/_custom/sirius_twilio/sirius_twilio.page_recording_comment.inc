<?php

function sirius_twilio_node_tab_recording_comment_access($recording_node) {
	sirius_require('sirius_twilio', 'sirius_twilio.page_recording.inc');
	return sirius_twilio_node_tab_recording_access($recording_node);
}

function sirius_twilio_node_tab_recording_comment($recording_node) {
	sirius_backlink_add('sirius/twilio/recordings', array('name' => 'All Recordings'));
	return drupal_get_form('sirius_twilio_node_tab_recording_comment_form', $recording_node);
}

function sirius_twilio_node_tab_recording_comment_form($form, &$form_state, $recording_node) {
	$form_state['recording_nid'] = $recording_node->nid;

	$recording_json = sirius_json_get($recording_node);
	$form['#tree'] = TRUE;

	$form['comments'] = array(
		'#type' => 'textarea',
		'#title' => 'Comments',
		'#default_value' => $recording_json['comments'],
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_twilio_node_tab_recording_comment_form_submit($form, &$form_state) {
	$recording_node = node_load($form_state['recording_nid']);
	$recording_json = sirius_json_get($recording_node);
	$recording_json['comments'] = $form_state['values']['comments'];
	sirius_jsonfield_log_impl($recording_json, 'comment', 'Comment edited');
	sirius_json_set($recording_node, $recording_json);
	node_save($recording_node);
}