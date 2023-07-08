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

	$clients_all = sirius_service_client_info_all();
	$options = array(NULL => t('-- Select --'));
	foreach ($clients_all as $key => $record) {
		$options[$key] = $record['name'];
	}
	$form['sirius_service_client_type'] = array(
		'#title' => 'Client Type',
		'#type' => 'select',
		'#default_value' => sirius_domain_variable_get('sirius_service_client_type'),
		'#options' => $options,
		'#states' => array(
			'visible' => array(
				array(':input[name="' . $name_active . '"]' => array('value' => 'Yes')),
			)
		),
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
	foreach (array('sirius_service_client_type', 'sirius_service_client_active', 'sirius_proxy_active') as $name) {
		sirius_domain_variable_set($name, $form_state['values'][$name]);
	}

	drupal_set_message("Settings saved.");
}