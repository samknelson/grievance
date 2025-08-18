<?php 

function sirius_trust_page_election_wizard_config() {
	return drupal_get_form('sirius_trust_page_election_wizard_config_form');
}

function sirius_trust_page_election_wizard_config_form($form, &$form_state) {
	return sirius_form_error('coming soon.');
	
	$variable_name = sirius_domain_variable_name('sirius_trust_election_wizard_policy_intro');
	$form[$variable_name] = array(
		'#type' => 'text_format',
		'#title' => t('Home Employer Introduction'),
		'#default_value' => variable_get($variable_name, NULL),
		'#description' => t('Enter the text that should appear before the "home employer" '),
	);

	return system_settings_form($form);
}
