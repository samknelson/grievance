<?php

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

	$result = sirius_twilio_outgoing_sms($to, 'This is a test SMS message. If you are receiving this, then Sirius Twilio is properly configured.');
	if ($result['success']) {
		drupal_set_message(t('Message sent. Twilio is properly configured.'));
	} else {
		drupal_set_message(t('There was a problem sending the message by SMS: @msg Please check your settings and try again.', array('@msg' => $result['msg'])), 'warning');
		if ($result['details']) {
			drupal_set_message($result['details'], 'warning');
		}
	}
}