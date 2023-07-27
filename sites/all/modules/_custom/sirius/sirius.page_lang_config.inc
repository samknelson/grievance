<?php

function sirius_page_lang_config() {
	return drupal_get_form('sirius_page_lang_config_form');
}

function sirius_page_lang_config_form($form, &$form_state) {
	require_once DRUPAL_ROOT . '/includes/iso.inc';

	$form['#tree'] = TRUE;

	$def = sirius_options_to_str(sirius_lang_options());
	$form['settings']['sirius_lang_options'] = array(
		'#title' => t('Language Options'),
		'#type' => 'textarea',
		'#default_value' => $def,
		'#description' => t('Enter the list of language options in the order that they should appear. Each line should contain the name and the prompt separated by a pipe (|), i.e. en|English'),
		'#rows' => 25,
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_page_lang_config_form_submit($form, &$form_state) {
	foreach ($form_state['values']['settings'] as $name => $value) {
		if (in_array($name, array())) {
			$value = sirius_richtext_handle_input($value);
		}
		sirius_domain_variable_set($name, $value);
	}

	drupal_set_message("Settings saved.");
}
