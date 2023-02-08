<?php

function sirius_steward_page_settings_checkin() {
	return drupal_get_form('sirius_steward_page_settings_checkin_form');
}

function sirius_steward_page_settings_checkin_form($form, &$form_state) {
	
	$disclaimer_options = sirius_disclaimer_list();
	$customfield_options = sirius_customfield_set_list_all();

	$variable_name = sirius_domain_variable_name('sirius_steward_checkin_disclaimer');
	$form[$variable_name] = array(
		'#type' => 'select',
		'#title' => 'Disclaimer',
		'#default_value' => variable_get($variable_name, NULL),
		'#options' => array(NULL => t('-- Select --')) + $disclaimer_options,
	);

	$variable_name = sirius_domain_variable_name('sirius_steward_checkin_in_customfield');
	$form[$variable_name] = array(
		'#type' => 'select',
		'#title' => 'Custom Field Set (IN)',
		'#default_value' => variable_get($variable_name, NULL),
		'#options' => array(NULL => t('-- Select --')) + $customfield_options,
	);

	$variable_name = sirius_domain_variable_name('sirius_steward_checkin_out_customfield');
	$form[$variable_name] = array(
		'#type' => 'select',
		'#title' => 'Custom Field Set (OUT)',
		'#default_value' => variable_get($variable_name, NULL),
		'#options' => array(NULL => t('-- Select --')) + $customfield_options,
	);

	return system_settings_form($form);
}

function sirius_steward_page_settings_checkin_form_validate($form, &$form_state) {
	$variable_name_a = sirius_domain_variable_name('sirius_steward_checkin_in_customfield');
	$variable_name_b = sirius_domain_variable_name('sirius_steward_checkin_out_customfield');
	if ($form_state['values'][$variable_name_a]) {
		if ($form_state['values'][$variable_name_a] == $form_state['values'][$variable_name_b]) {
			form_set_error($variable_name_b, 'Different fieldsets must be used for checkin and checkout.');
		}
	}
}