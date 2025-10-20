<?php

function sirius_worker_rating_page_config() {
	return drupal_get_form('sirius_worker_rating_page_config_form');
}

function sirius_worker_rating_page_config_form($form, &$form_state) {
	$form['#tree'] = TRUE;

	$form['rating']['sirius_worker_rating_intro'] = array(
		'#type' => 'text_format',
		'#title' => t('Introductory Text'),
		'#default_value' => sirius_domain_variable_get('sirius_worker_rating_intro'),
		'#description' => t('Enter the introductory text for the "out sick" landing page.'),
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_worker_rating_page_config_form_submit($form, &$form_state) {
	foreach ($form_state['values']['rating'] as $key => $value) {
		if (in_array($key, array('sirius_worker_rating_intro', 'sirius_worker_rating_reminder_body'))) {
			$value = sirius_richtext_handle_input($value);
		}
		sirius_domain_variable_set($key, $value);
	}
}
