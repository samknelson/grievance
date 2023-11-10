<?php

function sirius_help_page_config() {
	return drupal_get_form('sirius_help_page_config_form');
}

function sirius_help_page_config_form($form, &$form_state) {
	$form['sirius_help_show_create_links'] = array(
		'#type' => 'select',
		'#title' => t('Show "Create Help" Links?'),
		'#options' => array(
			'No' => t('No'),
			'Yes' => t('Yes'),
		),
		'#default_value' => variable_get('sirius_help_show_create_links', 'No'),
		'#description' => t('Should "Create Help Text" links be displayed on every page? This should generally only be enabled for development servers. The links will only be visible to users with "sirius help admin" permission.'),
	);

	return system_settings_form($form);
}