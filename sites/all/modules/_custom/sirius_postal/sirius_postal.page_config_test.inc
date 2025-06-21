<?php

function sirius_postal_page_config_test() {
	return drupal_get_form('sirius_postal_page_config_test_form');
}

function sirius_postal_page_config_test_form($form, &$form_state) {
	$form['#tree'] = TRUE;

	$form['function'] = array(
		'#type' => 'select',
		'#title' => t('Which Test?'),
		'#options' => array(
			'postal' => t('Mail (send a test or live letter)'),
			'ping' => t('Ping (test connectivity)'),
		),
		'#required' => TRUE,
	);

	$form['postal'] = array(
		'#type' => 'fieldset',
		'#states' => array('visible' => array(array(':input[name="function"]' => array('value' => 'postal')))),
	);

	$form['postal']['mode'] = array(
		'#title' => t('Mode'),
		'#type' => 'select',
		'#options' => array(
			'test' => t('Test'),
			'live' => t('Live'),
		),
	);

	$postal_client = sirius_postal_client_instance();
	$form['postal']['options'] = array(
		'#type' => 'fieldset',
		'#title' => t('Options'),
	);
	$form['postal']['options'] += $postal_client->mail_options_form();

	$form['postal']['to'] = array(
		'#type' => 'fieldset',
		'#title' => t('Recipient'),
	);
	$form['postal']['to'] += $postal_client->addr_form();

	$form['postal']['body'] = array(
		'#title' => t('Body'),
		'#type' => 'text_format',
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

  $log = sirius_minilog_render_html();
  if ($log) { $form['results'] = array('#markup' => '<hr>' . $log); }

	return $form;
}

function sirius_postal_page_config_test_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;

	$function = $form_state['values']['function'];

	$instance = sirius_postal_client_instance();
	if (!$instance) {
		drupal_set_message("No postal instance defined.", 'error');
		return;
	}
	if ($function == 'ping') {
		$result = $instance->ping();
	} else if ($function == 'postal') {
		$params = array();
		$params['to'] = $form_state['values']['postal']['to'];
		$params['body'] = sirius_richtext_handle_input($form_state['values']['postal']['body']);
		$params['options'] = $form_state['values']['postal']['options'];
		$params['mode'] = $form_state['values']['postal']['mode'];
		$result = $instance->send($params);
	}

	if ($result['success']) {
		sirius_minilog('<pre>' . print_r($result, 1) . '</pre>');
		drupal_set_message("Server request succeeded.");
	} else {
		drupal_set_message("Request failed: " . $result['msg'], 'warning');
	}

	if ($result['comm']) {
		$path = 'node/' . $result['comm']->nid . '/sirius_log_comm';
		$msg = "The mail message can be viewed and tracked <strong>" . l('here', $path) . '</strong>';
		drupal_set_message($msg);
		sirius_minilog($msg);
	}
}
