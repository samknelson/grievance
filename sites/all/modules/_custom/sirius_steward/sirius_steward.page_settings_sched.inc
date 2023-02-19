<?php

function sirius_steward_page_settings_sched() {
	return t('No settings for steward schedule.');
	return drupal_get_form('sirius_steward_page_settings_sched_form');
}

function sirius_steward_page_settings_sched_form($form, &$form_state) {

	/*	
	$variable_name = sirius_domain_variable_name('sirius_steward_checkin_disclaimer');
	$form[$variable_name] = array(
		'#type' => 'select',
		'#title' => 'Disclaimer',
		'#default_value' => variable_get($variable_name, NULL),
		'#options' => array(NULL => t('-- Select --')) + $disclaimer_options,
	);
	*/

	return system_settings_form($form);
}
