<?php

function sirius_service_page_config_client() {
	return drupal_get_form('sirius_service_page_config_client_form');
}

function sirius_service_page_config_client_form($form, &$form_state) {
	$name_active = 'sirius_service_client_active';

  $form[$name_active] = array(
  	'#title' => t('Client Active'),
  	'#type' => 'select',
  	'#options' => array(
  		'No' => 'No',
  		'Yes' => 'Yes',
  	),
    '#default_value' => sirius_domain_variable_get($name_active, ''),
  );

  $form['tokens'] = array(
  	'#type' => 'fieldset',
  	'#title' => 'Access Tokens',
		'#collapsible' => TRUE, 
		'#collapsed' => TRUE,
		'#states' => array(
			'visible' => array(
				array(':input[name="' . $name_active . '"]' => array('value' => 'Yes')),
			)
		),
  );

  $form['tokens']['client_ip'] = array(
  	'#markup' => '<p>' . 'This server\'s IP is listed as <strong>' . $_SERVER['SERVER_ADDR'] . '</strong>. That address must be allowed on the main server. Note that because this server may connect through a proxy, the listed IP may not be the same as the IP that is used for requests.' . '</p>'
  );

	$form['tokens']['sirius_service_client_host'] = array(
		'#type' => 'textfield',
		'#title' => 'Remote Host',
		'#default_value' => sirius_domain_variable_get('sirius_service_client_host'),
	);

	$form['tokens']['sirius_service_client_account'] = array(
		'#type' => 'textfield',
		'#title' => 'Account ID',
		'#default_value' => sirius_domain_variable_get('sirius_service_client_account'),
	);

	$form['tokens']['sirius_service_client_access'] = array(
		'#type' => 'textfield',
		'#title' => 'Access Token',
		'#default_value' => sirius_domain_variable_get('sirius_service_client_access'),
	);

  $form['sirius_proxy_active'] = array(
  	'#title' => t('Proxy Active'),
  	'#type' => 'select',
  	'#options' => array(
  		'No' => 'No',
  		'Yes' => 'Yes',
  	),
    '#default_value' => sirius_domain_variable_get('sirius_proxy_active', ''),
    '#description' => 'Is this domain configured as a proxy client, meaning that service calls should be made where possible to the remote server?'
  );

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_service_page_config_client_form_submit($form, &$form_state) {
	foreach (array('sirius_service_client_active', 'sirius_proxy_active', 'sirius_service_client_host', 'sirius_service_client_account', 'sirius_service_client_access') as $name) {
		sirius_domain_variable_set($name, $form_state['values'][$name]);
	}

	drupal_set_message("Settings saved.");
}