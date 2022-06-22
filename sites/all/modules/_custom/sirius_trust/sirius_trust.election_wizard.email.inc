<?php

function sirius_trust_election_wizard_emia($info, $data, $wizard_node, $current_step) { return drupal_get_form('sirius_trust_election_wizard_email_form', $info, $data, $wizard_node, $current_step); }

function sirius_trust_election_wizard_email_form($form, &$form_state, $info, $data, $wizard_node, $current_step) {
	$result = sirius_trust_election_wizard_get_worker($wizard_node);
	if (!$result['success']) { return sirius_form_error($result['msg']); }
	$worker_node = $result['data'];

	$old_email = $worker_node->field_sirius_email['und'][0]['value'];
	$code = $data['email']['code'];

	if ($old_email) {
		$html = '<h2>Current email</h2>';
		$html .= '<br />';
		$form['email']['intro'] = array('#markup' => sirius_fakerender_field('Current email:', $old_email));

		$def = $data['email']['change'];
		if (!$def) {
			if ($code) {
				$def = 'Yes';
			} else {
				$def = 'No';
			}
		}

		$form['email']['change'] = array(
			'#type' => 'radios',
			'#options' => array(
				'No' => t('Keep my current email'),
				'Yes' => t('Enter a new email'),
			),
			'#default_value' => $def,
			'#required' => TRUE,
			'#id' => 'change_email',
		);

		$fieldset_title = t('New email');
	} else {
		$form['email']['change'] = array(
			'#type' => 'value',
			'#value' => 'Yes',
		);

		$fieldset_title = 'email';
	}

	$form['email']['new'] = array(
		'#type' => 'fieldset',
		'#description' => $fieldset_intro,
		'#title' => $fieldset_title,
		'#states' => array('visible' => array(":input[name=\"email[change]\"]" => array('value' => 'Yes'))),
	);

	$email = $data['email']['new']['email'];

	$form['email']['new']['email'] = array(
		'#type' => 'textfield',
		'#title' => t('Email Address'),
		'#default_value' => $email,
		'#size' => 12,
	);
	if ($code) { $form['email']['new']['email']['#disabled'] = TRUE; }

	if ($code) {
		$form['email']['new']['code'] = array(
			'#type' => 'textfield',
			'#title' => t('Validation Code'),
			'#size' => 6,
		);
	}

	$form = sirius_trust_election_wizard_form_prep($form, $form_state, $info, $data, $wizard_node, $current_step);

	if ($code) {
		$form['submit_validate'] = array(
			'#type' => 'submit',
			'#value' => t('Validate'),
			'#name' => 'submit_validate',
			'#states' => array('visible' => array(":input[name=\"email[change]\"]" => array('value' => 'Yes'))),
		);

		$form['submit_reset'] = array(
			'#type' => 'submit',
			'#value' => t('Start Over'),
			'#name' => 'submit_reset',
			'#states' => array('visible' => array(":input[name=\"email[change]\"]" => array('value' => 'Yes'))),
		);
	} else {
		$form['submit_send_code'] = array(
			'#type' => 'submit',
			'#value' => t('Send Validation Code'),
			'#name' => 'submit_send_code',
			'#states' => array('visible' => array(":input[name=\"email[change]\"]" => array('value' => 'Yes'))),
		);
	}

	if (user_access('sirius trust admin')) {
		$form['submit_skip'] = array(
			'#type' => 'submit',
			'#value' => t('(Admin) Skip Validation'),
			'#name' => 'submit_skip',
			'#states' => array('visible' => array(":input[name=\"email[change]\"]" => array('value' => 'Yes'))),
		);
	}

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
		'#name' => 'submit_nochange',
		'#states' => array('visible' => array(":input[name=\"email[change]\"]" => array('value' => 'No'))),
	);

	return $form;
}

function sirius_trust_election_wizard_email_form_validate($form, &$form_state) {
	if ($form_state['values']['email']['change'] == 'Yes') {
		$email = $form_state['values']['email']['new']['email'];
		if (!$email) {
			form_set_error("email][new][email", "email field is required.");
			return;
		}

		if (!sirius_email_validate($email)) {
			form_set_error("email][new][email", "Please enter a valid email address.");
			return;
		}
	}
}

function sirius_trust_election_wizard_email_form_submit($form, &$form_state) {
	$info = $form_state['info'];
	$data = $form_state['data'];
	$wizard_node = $form_state['wizard_node'];
	$current_step = $form_state['current_step'];


	// Don't want to change
	$data['email']['change'] = $form_state['values']['email']['change'];
	if ($form_state['values']['email']['change'] != 'Yes') {
		return sirius_wizard_submit($info, $data, $wizard_node, $current_step, "Response saved.", array('nonsequential' => TRUE));
	}

	$action = $form_state['clicked_button']['#name'];
	$data['email']['new'] = $form_state['values']['email']['new'];
	$email = sirius_email_format($data['email']['new']['email']);

	// Administrator: save the data and skip to the next step
	if ($action == 'submit_skip') {
		return sirius_wizard_submit($info, $data, $wizard_node, $current_step, "Administrator skipped validation for $email.", array('nonsequential' => TRUE));
	}

	// Flood control
	if (!sirius_flood_is_allowed('sirius_trust_election_wizard_email')) {
		return drupal_set_message('Flood control: too many attempts. Please try again later.');
	}
	sirius_flood_register_event('sirius_trust_election_wizard_email');

	// Send a validation code
	if ($action == 'submit_send_code') {
		$code = rand(100000, 999999);
		$data['email']['code'] = $code;
		$params['subject'] = "Email Validation Code";
		global $base_url;
		$url = $base_url . '/' . current_path();
		$params['body'] = "Your email validation code is <strong>$code</strong><br /><br />";
		$params['body'] .= "Please go to <a href=\"$url\">$url</a> and enter this validation code to continue with your application.";
		$params['to'] = $email;
		$result = sirius_email_send($params);
		if ($result['success']) {
			return sirius_wizard_submit($info, $data, $wizard_node, $current_step, "A validation code has been sent to $email.", array('incomplete' => TRUE, 'nonsequential' => TRUE));
		} else {
			return drupal_set_message("Unable to send a text message to $email: " . $result['msg'], 'warning');
		}
	} 

	// Send a validation code
	if ($action == 'submit_reset') {
		$data['email']['code'] = NULL;
		return sirius_wizard_submit($info, $data, $wizard_node, $current_step, "The email address had been cleared.", array('incomplete' => TRUE, 'nonsequential' => TRUE));
	} 

	// Validate the code
	if ($action == "submit_validate") {
		$code = $data['email']['code'];
		$submitted_code = trim($data['email']['new']['code']);
		if ($code != $submitted_code) {
			return drupal_set_message("Incorrect code.", 'warning');
		} else {
			return sirius_wizard_submit($info, $data, $wizard_node, $current_step, "The email address $email has been validated.", array('nonsequential' => TRUE));
		}
	}

	// Don't know what to do.
	if ($acton) { 
		return drupal_set_message("Unknown action: $action.", 'warning');
	} else {
		return drupal_set_message("Please select one of the options below.", 'warning');
	}
}
