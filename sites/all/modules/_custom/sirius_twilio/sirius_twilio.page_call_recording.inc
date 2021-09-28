<?php

function sirius_twilio_node_tab_call_recording_access($call_node) {
	sirius_require('sirius_twilio', 'sirius_twilio.page_call.inc');
	return sirius_twilio_node_tab_call_access($call_node);
}

function sirius_twilio_node_tab_call_recording($call_node) {
	return drupal_get_form('sirius_twilio_node_tab_call_recording_form', $call_node);
}

function sirius_twilio_node_tab_call_recording_form($form, &$form_state, $call_node) {
	$form_state['call_nid'] = $call_node->nid;

	$call_json = sirius_json_get($call_node);
	if ($call_json['context']['operator_nid']) { sirius_backlink_add('node/' . $call_json['context']['operator_nid'] . '/sirius_operator_calls', array('name' => 'Incoming Calls')); }
	$form['#tree'] = TRUE;

	$recording_node = sirius_log_handler($call_node, 'sirius_log');
	if ($recording_node) {
		$html = "There is a recording for this call. ";
		$html .= l('View', 'node/' . $recording_node->nid . '/sirius_twilio_recording', array('attributes' => array('class' => array('tool-button'))));
		$form['intro'] = array('#markup' => $html);
		return $form;
	}

	$call_sid = $call_json['context']['request']['CallSid'];
	if (!$call_sid) {
		$form['intro'] = array('#markup' => "There is no local copy of the recording, and there is no SID for this call, so we cannot search for the call on the telephony provider.");
		return $form;
	}

	$result = sirius_twilio_recording_fetch_from_call($call_sid);
	if (!$result['success']) {
		$form['intro'] = array('#markup' => "There is no local copy of the recording, and we are unable to find the recording at the telephony provider. This is probably because the call was not made on a recorded line. Details: " . $result['msg']);
		return $form;
	}

	$form['recording_sid'] = array('#type' => 'value', '#value' => $result['recording']['sid']);

	$form['intro'] = array('#markup' => '<div class="sirius_form_intro">' . t('There is no local copy of the recording, but a remote recording exists on the telelphone provider. Click the button below to pull the remote copy and save it locally.') . '</div>');

	$form['comments'] = array(
		'#type' => 'textarea',
		'#title' => t('Comments'),
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Pull Recording')
	);

	return $form;
}

function sirius_twilio_node_tab_call_recording_form_submit($form, &$form_state) {
	$call_node = node_load($form_state['call_nid']);
	$recording_sid = $form_state['values']['recording_sid'];
	$result = sirius_twilio_recording_pull($recording_sid, array('comments' => $form_state['values']['comments']));
	if ($result['success']) {
		++$count_success;
	} else {
		++$count_failure;
		drupal_set_message($result['msg'], 'warning');
	}

}