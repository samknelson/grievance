<?php

function sirius_trust_page_config() {
	return drupal_get_form('sirius_trust_page_config_form');
}

function sirius_trust_page_config_form($form, &$form_state) {

	$tag_options = sirius_taxonomy_options('sirius_contact_tags');

	$variable_name = sirius_domain_variable_name('sirius_trust_member_tid');
	$form[$variable_name] = array(
		'#type' => 'select',
		'#title' => t('Member Tag'),
		'#default_value' => variable_get($variable_name, NULL),
		'#options' => array(NULL => t('-- Select --')) + $tag_options,
		'#description' => t('Select the tag that will be assigned to members.'),
	);

	$variable_name = sirius_domain_variable_name('sirius_trust_dependent_tid');
	$form[$variable_name] = array(
		'#type' => 'select',
		'#title' => t('Dependent Tag'),
		'#default_value' => variable_get($variable_name, NULL),
		'#options' => array(NULL => t('-- Select --')) + $tag_options,
		'#description' => t('Select the tag that will be assigned to dependents.'),
	);

	return system_settings_form($form);
}

