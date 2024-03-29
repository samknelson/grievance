<?php

function sirius_edls_page_631twilio() {
	$result = sirius_edls_employer_switch();
	$html = $result['markup'];
	$employer_node = $result['employer_node'];
	if (!$employer_node) {
		$html .= sirius_page_error('Please select an employer.');
		return $html;
	}

	$html .= drupal_render(drupal_get_form('sirius_edls_page_631twilio_form', $employer_node));
	return $html;
}

function sirius_edls_page_631twilio_form($form, &$form_state, $employer_node) {
	$form_state['employer_nid'] = $employer_node->nid;
	$form['#tree'] = TRUE;

	$employer_json = sirius_json_get($employer_node);

	$form['edls']['teamsters631']['twilio'] = array(
		'#type' => 'fieldset',
		'#title' => 'Twilio Settings',
		'#description' => t('When sending text messages from the EDLS, the system default should be overridden, and instead the settings below should be used.'),
	);
  $form['edls']['teamsters631']['twilio']['phone'] = array(
    '#type' => 'textfield',
    '#title' => t('Outgoing Phone Number'),
    '#default_value' => $employer_json['edls']['teamsters631']['twilio']['phone'],
    '#description' => t('Phone number in E164 format (i.e. "+15551234567"). This must be a phone number provisioned at Twilio. It will be the default phone number used for outgoing calls (when no caller ID is selected) and will be used for all outgoing SMS messages.'),
  );

  $form['edls']['teamsters631']['twilio']['account_sid'] = array(
    '#type' => 'textfield',
    '#title' => t('Account SID'),
    '#default_value' => $employer_json['edls']['teamsters631']['twilio']['account_sid'],
  );

  $form['edls']['teamsters631']['twilio']['account_token'] = array(
    '#type' => 'textfield',
    '#title' => t('Account Token'),
    '#default_value' => $employer_json['edls']['teamsters631']['twilio']['account_token'],
  );

  $form['test_number'] = array(
    '#type' => 'textfield',
    '#title' => t('Send a test message to this number'),
    '#description' => t('Send a test text mesasge to the phone number listed.'),
  );

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_edls_page_631twilio_form_submit($form, &$form_state) {
	$employer_node = node_load($form_state['employer_nid']);
	$employer_json = sirius_json_get($employer_node);
	$employer_json['edls']['teamsters631']['twilio'] = $form_state['values']['edls']['teamsters631']['twilio'];
	sirius_json_set($employer_node, $employer_json);
	node_save($employer_node);
	drupal_set_message("Settings saved.");

	$test_number = $form_state['values']['test_number'];
	if ($test_number) {
		drupal_set_message("Sending a test message to: $test_number");

		$params['handler_nids'] = array();
		$medium = 'sms';
		$to = $test_number;
		$params['to'] = $test_number;
		$params['sms'] = 'This is a test message from the EDLS system on behalf of ' . $employer_node->title;

		$context = array('notify_params' => &$params, 'employer_node' => $employer_node);
		drupal_alter('sirius_edls_notify_pre_send', $context);

		$result = sirius_sms_send($params);

		$context = array('notify_params' => &$params, 'notify_result' => &$result, 'employer_node' => $employer_node);
		drupal_alter('sirius_edls_notify_post_send', $context);

		if ($result['success']) {
			drupal_set_message("Test message sent.");
		} else {
			drupal_set_message("Test message failed: " . $result['msg'] . ' - ' . $result['details'], 'error');
			$form_state['rebuild'] = TRUE;
		}

	  if ($result['comm']) {
	    $path = 'node/' . $result['comm']->nid . '/sirius_log_comm';
	    $msg = "The comm message can be viewed and tracked <strong>" . l('here', $path) . '</strong>';
	    drupal_set_message($msg);
	    sirius_minilog($msg);
	  }
	}
}
