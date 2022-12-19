<?php

function sirius_steward_page_settings() {
	return drupal_get_form('sirius_steward_page_settings_form');
}

function sirius_steward_page_settings_form($form, &$form_state) {
	
	$disclaimer_options = sirius_disclaimer_list();
	if (!$disclaimer_options) { return sirius_form_error('No disclaimers have been defined.'); }

	$form['facility_checkin'] = array(
		'#type' => 'fieldset',
		'#title' => t('Facility Checkin'),
	);
	$variable_name = sirius_domain_variable_name('sirius_steward_facility_checkin_disclaimer');
	$form['facility_checkin'][$variable_name] = array(
		'#type' => 'select',
		'#title' => 'Disclaimer',
		'#default_value' => variable_get($variable_name, NULL),
		'#options' => array(NULL => t('-- Select --')) + $disclaimer_options,
	);

	return system_settings_form($form);
}