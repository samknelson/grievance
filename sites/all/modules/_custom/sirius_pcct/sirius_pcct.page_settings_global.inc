<?php

function sirius_pcct_page_settings_global() {
	return drupal_get_form('sirius_pcct_page_settings_global_form');
}

function sirius_pcct_page_settings_global_form($form, &$form_state) {
	$form['#tree'] = TRUE;

	$form['settings']['sirius_pcct_domain_list_header'] = array(
		'#title' => t('Domain List Page Header'),
		'#type' => 'text_format',
		'#default_value' => variable_get('sirius_pcct_domain_list_header'),
		'#description' => t('Enter the text that should appear at the top of the landing page (above the "select a plan" dropdown).'),
	);

	$form['settings']['sirius_pcct_domain_list_footer'] = array(
		'#title' => t('Landing Page Footer'),
		'#type' => 'text_format',
		'#default_value' => variable_get('sirius_pcct_domain_list_footer'),
		'#description' => t('Enter the text that should appear at the bottom of the landing page (below the "select a plan" dropdown).'),
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_pcct_page_settings_global_form_submit($form, &$form_state) {
	foreach ($form_state['values']['settings'] as $name => $value) {
		if (in_array($name, array('sirius_pcct_domain_list_header', 'sirius_pcct_domain_list_footer'))) {
			$value = sirius_richtext_handle_input($value);
		}
		variable_set($name, $value);
	}

	drupal_set_message("Settings saved.");
}