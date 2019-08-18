<?php

use Twilio\Rest\Client;

function sirius_twilio_page_test() {
	return drupal_get_form('sirius_twilio_page_test_form');
}

function sirius_twilio_page_test_form($form, &$form_state) {
	$form['intro'] = array(
		'#type' => 'markup',
		'#markup' => '<div class="sirius_form_intro">' .
			t('Use the form below to verify that the Twilio library is properly configured by sending an SMS message.') .
			'</div>'
	);
	$form['to'] = array(
		'#type' => 'textfield',
		'#title' => t('To'),
		'#description' => t('Enter the number which should reveive the SMS message.'),
		'#required' => TRUE,
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => 'Send SMS message now'
	);

	return $form;
}

function sirius_twilio_page_test_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;
	$to = $form_state['values']['to'];

	try {
		$client = sirius_twilio_client_get();
		$client->messages->create(
	    $to,
	    array(
	      'from' => variable_get('sirius_twilio_phone', ''),
	      'body' => 'This is a test SMS message. If you are receiving this, then Sirius Twilio is properly configured.'
	    )
		);
	} catch (Exception $e) {
		drupal_set_message('Twilio error: ' . $e->getCode() . ' - ' . $e->getMessage(), 'error');
		return;
	}

	drupal_set_message(t('Message sent. Twilio is properly configured.'));
}