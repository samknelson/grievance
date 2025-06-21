<?php

function sirius_edls_page_config() {
	return drupal_get_form('sirius_edls_page_config_form');
}

function sirius_edls_page_config_form($form, &$form_state) {
	$role_options = user_roles(TRUE, 'sirius edls employer labor supervisor');

	$variable_name = sirius_domain_variable_name('sirius_edls_name_short');
	$form[$variable_name] = array(
		'#type' => 'textfield',
		'#title' => t('Short Name'),
		'#default_value' => variable_get($variable_name, 'EDLS'),
		'#description' => t('Enter the short name for this application; default is "EDLS".'),
	);

	$variable_name = sirius_domain_variable_name('sirius_edls_name_long');
	$form[$variable_name] = array(
		'#type' => 'textfield',
		'#title' => t('Long Name'),
		'#default_value' => variable_get($variable_name, 'Employer Daily Labor Schedule'),
		'#description' => t('Enter the short name for this application; default is "Employer Daily Labor Schedule".'),
	);

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

