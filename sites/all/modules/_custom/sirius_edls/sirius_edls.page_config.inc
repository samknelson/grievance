<?php

function sirius_edls_page_config() {
	return drupal_get_form('sirius_edls_page_config_form');
}

function sirius_edls_page_config_form($form, &$form_state) {
	$role_options = user_roles(TRUE, 'sirius edls employer labor supervisor');

	$variable_name = sirius_domain_variable_name('sirius_edls_supervisor_role');
	$form[$variable_name] = array(
		'#type' => 'select',
		'#title' => t('Supervisor Role'),
		'#default_value' => variable_get($variable_name, NULL),
		'#options' => array(NULL => t('-- Select --')) + $role_options,
		'#description' => t('Supervisors must have this role'),
	);

	return system_settings_form($form);
}

