<?php

function sirius_event_page_intake_anonymous_landing() {
	return drupal_get_form('sirius_event_page_intake_anonymous_landing_form');
}

function sirius_event_page_intake_anonymous_landing_form($form, &$form_state) {
	$form['intro'] = array('#markup' => '<div class="sirius_form_intro">' . t('Your administrator should have provided you with an access code. Please enter it below.') . '</div>');

	$form['access_code'] = array(
		'#type' => 'textfield',
		'#title' => t('Access Code'),
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_event_page_intake_anonymous_landing_form_submit($form, &$form_state) {
	$access_code = sirius_event_intake_access_code_anonicalize($form_state['values']['access_code']);
	drupal_goto('intake/' . $access_code);
}
