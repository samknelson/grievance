<?php

function sirius_twilio_page_test() {
	return drupal_get_form('sirius_twilio_page_test_form');
}

function sirius_twilio_page_test_form($form, &$form_state) {
	$intro_html = '<div class="sirius_form_intro">';

	$result = sirius_twilio_account_fetch();
	if (!$result['success']) {
		$account_render = "<em>Twilio account not found: " . $result['msg'] . "</em>";
	} else {
		$account_arr = $result['account']->toArray();
		$account_render = $account_arr['friendlyName'];
	}
	$intro_html .= sirius_fakerender_field('Twilio Account:', $account_render);

	$result = sirius_twilio_app_fetch();
	if (!$result['success']) {
		$app_render = "<em>Twilio app not found: " . $result['msg'] . "</em>";
	} else {
		$app_arr = $result['app']->toArray();
		$app_render = $app_arr['friendlyName'];
	}
	$intro_html .= sirius_fakerender_field('Twilio App:', $app_render);


	$intro_html .= '</div>';

	$form['intro'] = array('#markup' => $intro_html);

	$form['to'] = array(
		'#type' => 'textfield',
		'#title' => t('To'),
		'#description' => t('Enter the number which should reveive the SMS message.'),
		'#required' => TRUE,
	);

	$form['medium'] = array(
		'#type' => 'select',
		'#title' => t('Medium'),
		'#required' => TRUE,
		'#options' => array(
			'sms' => t('SMS (Text Message)'),
			'voice' => t('Voice'),
		),
	);

	$form['callerid_nid'] = array(
		'#type' => 'select',
		'#title' => t('Caller ID'),
		'#options' => array(NULL => t('-- Select --')) + sirius_callerid_options(),
		'#description' => t('Caller ID to use. Applies only for voice calls.'),
		'#states' => array('visible' => array(":input[name=\"medium\"]" => array('value' => 'voice'))),
	);

  $result = sirius_twilio_incoming_phone_numbers_list();
  if (!$result['success']) { return sirius_form_error($result['msg']); }
  $ipns = $result['records'];
  if (!$ipns) { return sirius_form_error(t('No phone numbers were found in the account. Please create at least one SMS-capable phone number in your Twilio account.')); }
  $ipn_options = array();
  foreach ($ipns as $ipn) {
  	$ipn_options[sirius_phone_format($ipn['phoneNumber'])] = sirius_phone_format($ipn['phoneNumber']) . ' - ' . $ipn['friendlyName'];
  }
	$ipn_default = sirius_phone_format(sirius_domain_variable_get('sirius_twilio_phone', ''));
	$form['sms_from'] = array(
		'#title' => t('Outgoing Phone Number'),
		'#type' => 'select',
		'#required' => TRUE,
		'#options' => $ipn_options,
		'#default_value' => $ipn_default,
		'#states' => array('visible' => array(":input[name=\"medium\"]" => array('value' => 'sms'))),
	);

	$form['msg'] = array(
		'#type' => 'textarea',
		'#title' => t('Message'),
		'#required' => TRUE,
		'#default_value' => 'This is a test message from the Sirius system. If you are receiving this, then Sirius Twilio is properly configured.',
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Send Test Message'),
	);

	return $form;
}

function sirius_twilio_page_test_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;
	$to = $form_state['values']['to'];
	$medium = $form_state['values']['medium'];
	$outgoing_msg = $form_state['values']['msg'];
	$callerid_nid = $form_state['values']['callerid_nid'];

	$params = array();
	$params['handler_nids'] = array();
	$params['to'] = $to;
	// $params['subject'] = $bulk_node->title;
	// $params['body'] = $bulk_node->body['und'][0]['value'];
	$params['sms'] = $outgoing_msg;
	$params['voice'] = $outgoing_msg;
	$params['callerid_nid'] = $callerid_nid;
	// $params['phonetree']['play']['url'] = file_create_url($bulk_node->field_sirius_audio_public['und'][0]['uri']);
	$params['phonetree']['disable'] = TRUE;
	$params['amd']['MachineDetection'] = 'DetectMessageEnd';
	$params['sms_from'] = $form_state['values']['sms_from'];

	if ($medium == 'voice') {
		$result = sirius_voice_send($params);
	} else {
		$result = sirius_sms_send($params);
	}

	if ($result['success']) {
		drupal_set_message("Message sent successfully.");
		if ($result['msg']) { drupal_set_message($result['msg']); }
	} else {
		drupal_set_message("Message failed: " . $result['msg'], 'warning');
		if ($result['details']) {
			drupal_set_message("Details: " . $result['details'], 'warning');
		}
	}

  if ($result['comm']) {
    $path = 'node/' . $result['comm']->nid . '/sirius_log_comm';
    $msg = "The comm message can be viewed and tracked <strong>" . l('here', $path) . '</strong>';
    drupal_set_message($msg);
    sirius_minilog($msg);
  }
}
