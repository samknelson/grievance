<?php

function sirius_smf_page_register() {
	return drupal_get_form('sirius_smf_page_register_form');
}

function sirius_smf_page_register_form($form, &$form_state) {
	$form['intro'] = array('#markup' => "User registration form coming soon. To register a user manually, go to the Worker's page. Under Contact > User, sync the user. This will create a Drupal user and, as a side effect, an Okta user.");

	$form['ssn'] = array(
		'#type' => 'textfield',
		'#title' => 'Social Securtity Number',
		'#size' => 11,
		'#required' => TRUE,
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_smf_page_register_form_validate($form, &$form_state) {
	$ssn = sirius_ssn_format($form_state['values']['ssn']);

	if (!sirius_ssn_validate($ssn)) {
		form_set_error('ssn', 'Please enter a valid SSN.');
	} else {
		if (!sirius_flood_is_allowed('sirius_smf_register_ssn', $ssn)) {
			form_set_error('ssn', 'Too many registration attempts with this social security number. Please try again later.');
		}
	}

	if (!sirius_flood_is_allowed('sirius_smf_register_ip')) {
		form_set_error('ssn', 'Too many registration attempts from this address. Please try again later.');
	}
}

function sirius_smf_page_register_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;

	$ssn = sirius_ssn_format($form_state['values']['ssn']);

	// Failure
	sirius_flood_register_event('sirius_smf_register_ip');
	sirius_flood_register_event('sirius_smf_register_ssn', $ssn);
	drupal_set_message("Registration failed: [under development]", 'warning');
}