<?php

function sirius_postal_page_config() {
	return drupal_get_form('sirius_postal_page_config_client_form');
}

function sirius_postal_page_config_client_form($form, &$form_state) {
	$options = array(NULL => t('-- Select --'));

	if (module_exists('sirius_postal_lob')) {
		$options['postal_lob'] = 'Lob';
	}

	$form['#tree'] = TRUE;
	$form['sirius_postal_client_type'] = array(
		'#title' => t('Client Type'),
		'#type' => 'select',
		'#default_value' => sirius_domain_variable_get('sirius_postal_client_type'),
		'#options' => $options,
		'#states' => array(
			'visible' => array(
				array(':input[name="' . $name_active . '"]' => array('value' => 'Yes')),
			)
		),
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_postal_page_config_client_form_submit($form, &$form_state) {
	foreach (array('sirius_postal_client_type') as $name) {
		sirius_domain_variable_set($name, $form_state['values'][$name]);
	}
}