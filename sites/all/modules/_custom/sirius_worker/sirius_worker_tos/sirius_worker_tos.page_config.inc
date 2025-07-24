<?php

function sirius_worker_tos_page_config() {
	return drupal_get_form('sirius_worker_tos_page_config_form');
}

function sirius_worker_tos_page_config_form($form, &$form_state) {
	$variable_name = sirius_domain_variable_name('sirius_worker_tos_intro');

	$form['#tree'] = TRUE;

	$form['tos']['sirius_worker_tos_intro'] = array(
		'#type' => 'text_format',
		'#title' => t('Introductory Text'),
		'#default_value' => sirius_domain_variable_get('sirius_worker_tos_intro'),
		'#description' => t('Enter the introductory text for the "out sick" landing page.'),
	);

	$form['tos']['sirius_worker_tos_reminder_active'] = array(
		'#type' => 'select',
		'#title' => t('Send Reminders?'),
		'#options' => array(
			NULL => t('-- Select --'),
			'No' => t('No'),
			'Yes' => t('Yes'),
		),
		'#default_value' => sirius_domain_variable_get('sirius_worker_tos_reminder_active'),
	);

	$form['tos']['sirius_worker_tos_reminder_subject'] = array(
		'#type' => 'textfield',
		'#title' => t('Reminder Email Subject'),
		'#default_value' => sirius_domain_variable_get(
			'sirius_worker_tos_reminder_subject', 
			"Reminder: You are still absent."
		),
	);

	$form['tos']['sirius_worker_tos_reminder_body'] = array(
		'#type' => 'text_format',
		'#title' => t('Reminder Email Body'),
		'#default_value' => sirius_domain_variable_get(
			'sirius_worker_tos_reminder_body',
			"This is just a reminder. You are currently marked as absent. While you are absent, you may not be scheduled for work by your employer. To cancel, please sign in to the dispatch system and mark yourself available on the \"schedule\" tab."
		),
	);

	$form['tos']['sirius_worker_tos_reminder_sms'] = array(
		'#type' => 'textarea',
		'#title' => t('Reminder SMS'),
		'#default_value' => sirius_domain_variable_get(
			'sirius_worker_tos_reminder_sms',
			"This is just a reminder. You are currently marked as absent. While you are absent, you may not be scheduled for work by your employer. To cancel, please sign in to the dispatch system and mark yourself available on the \"schedule\" tab."
		),
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_worker_tos_page_config_form_submit($form, &$form_state) {
	foreach ($form_state['values']['tos'] as $key => $value) {
		if (in_array($key, array('sirius_worker_tos_intro', 'sirius_worker_tos_reminder_body'))) {
			$value = sirius_richtext_handle_input($value);
		}
		sirius_domain_variable_set($key, $value);
	}
}
