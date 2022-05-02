<?php

function sirius_service_page_config_client_test() {
	return drupal_get_form('sirius_service_page_config_client_test_form');
}

function sirius_service_page_config_client_test_form($form, &$form_state) {
	$form['action'] = array(
		'#type' => 'select',
		'#title' => 'Action',
		'#options' => array(
			'client' => 'Client connection test',
			'proxy' => 'Proxy connection test',
		),
		'#required' => TRUE,
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

  $log = sirius_minilog_render_html();
  if ($log) { $form['results'] = array('#markup' => '<hr>' . $log); }

	return $form;
}

function sirius_service_page_config_client_test_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;

	$action = $form_state['values']['action'];
	if ($action == 'client') {
		$result = sirius_service_client_request('sirius_service_ping', 'First argument echo text');
	} else {
		$result = sirius_proxy_call('sirius_service_ping', 'First argument echo text');
	}

	if ($result['success']) {
		sirius_minilog('<pre>' . print_r($result, 1) . '</pre>');
		if ($result['is_remote']) {
			drupal_set_message("Request to REMOTE server succeeded.");
		} else {
			drupal_set_message("Request to LOCAL server succeeded.");
		}
	} else {
		drupal_set_message("Request failed: " . $result['msg'], 'warning');
	}
}
