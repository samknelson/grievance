<?php

function sirius_service_page_config_client_test() {
	return drupal_get_form('sirius_service_page_config_client_test_form');
}

function sirius_service_page_config_client_test_form($form, &$form_state) {
	$form['#tree'] = TRUE;

	$form['action'] = array(
		'#type' => 'select',
		'#title' => t('Action'),
		'#options' => array(
			'client' => 'Client connection test',
			'proxy' => 'Proxy connection test',
		),
		'#required' => TRUE,
	);

	$form['function'] = array(
		'#type' => 'textfield',
		'#title' => t('Function'),
		'#required' => TRUE,
		'#default_value' => 'sirius_service_ping',
		'#description' => t('Enter the function to call on the remote server. Usually this should be "sirius_service_ping".'),
	);

	for ($i=0; $i<3; ++$i) {
		$form['arg'][$i] = array(
			'#type' => 'textfield',
			'#title' => t('Argument #') . ($i+1),
		);
	}
	$form['arg'][0]['#default_value'] = 'Ping Echo Test';

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

	$function = $form_state['values']['function'];

	$action = $form_state['values']['action'];
	if ($action == 'client') {
		$result = sirius_service_client_request(
			$function, 
			$form_state['values']['arg'][0],
			$form_state['values']['arg'][1],
			$form_state['values']['arg'][2],
		);
	} else {
		$result = sirius_proxy_call(
			$function, 
			$form_state['values']['arg'][0],
			$form_state['values']['arg'][1],
			$form_state['values']['arg'][2],
		);
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
