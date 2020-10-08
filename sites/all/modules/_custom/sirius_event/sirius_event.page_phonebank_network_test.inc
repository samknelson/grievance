<?php

function sirius_node_tab_event_phonebank_network_test_access($event_node) { 
	return sirius_node_tab_event_phonebank_call_access($event_node);
}

function sirius_node_tab_event_phonebank_network_test($event_node = NULL) {
	return drupal_get_form('sirius_node_tab_event_phonebank_network_test_form', $event_node);
}

function sirius_node_tab_event_phonebank_network_test_form($form, &$form_state, $event_node = NULL) {
	$form['submit'] = array('#type' => 'submit', '#value' => t('Run Twilio Network Test'));

	if ($form_state['results']) {
		$form['results'] = array('#markup' => '<hr>' . $form_state['results']);
	}
	return $form;
}

function sirius_node_tab_event_phonebank_network_test_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;
	$form_state['results'] = '<iframe src="https://networktest.twilio.com/" width="80%" height="800px" style="border: 2px solid #999999;" allow="camera;microphone"></iframe>';
}
