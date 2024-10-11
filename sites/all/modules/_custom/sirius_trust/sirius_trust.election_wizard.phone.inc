<?php

function sirius_trust_election_wizard_phone($info, $data, $wizard_node, $current_step) { return drupal_get_form('sirius_trust_election_wizard_phone_form', $info, $data, $wizard_node, $current_step); }

function sirius_trust_election_wizard_phone_form($form, &$form_state, $info, $data, $wizard_node, $current_step) {
	$worker_node = sirius_trust_election_wizard_get_worker($wizard_node);

	$old_phone = $worker_node->field_sirius_phone['und'][0]['value'];
	$code = $data['phone']['code'];

	if ($old_phone) {
		$html = '<h2>Current Phone</h2>';
		$html .= '<br />';
		$form['phone']['intro'] = array('#markup' => sirius_fakerender_field('Current Phone:', $old_phone));

		$def = $data['phone']['change'];
		if (!$def) {
			if ($code) {
				$def = 'Yes';
			} else {
				$def = 'No';
			}
		}

		$form['phone']['change'] = array(
			'#type' => 'radios',
			'#options' => array(
				'No' => t('Keep my current phone'),
				'Yes' => t('Enter a new phone'),
			),
			'#default_value' => $def,
			'#required' => TRUE,
			'#id' => 'change_phone',
		);

		$fieldset_title = t('New Phone');
	} else {
		$form['phone']['change'] = array(
			'#type' => 'value',
			'#value' => 'Yes',
		);

		$fieldset_title = 'Phone';
	}

	$form['phone']['new'] = array(
		'#type' => 'fieldset',
		'#description' => $fieldset_intro,
		'#title' => $fieldset_title,
		'#states' => array('visible' => array(":input[name=\"phone[change]\"]" => array('value' => 'Yes'))),
	);

	$phone = $data['phone']['new']['phone'];

	$form['phone']['new']['phone'] = array(
		'#type' => 'textfield',
		'#title' => t('Phone Number'),
		'#default_value' => $phone,
		'#size' => 12,
	);
	if ($code) { $form['phone']['new']['phone']['#disabled'] = TRUE; }

	if ($code) {
		$form['phone']['new']['code'] = array(
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
			'#states' => array('visible' => array(":input[name=\"phone[change]\"]" => array('value' => 'Yes'))),
		);

		$form['submit_reset'] = array(
			'#type' => 'submit',
			'#value' => t('Start Over'),
			'#name' => 'submit_reset',
			'#states' => array('visible' => array(":input[name=\"phone[change]\"]" => array('value' => 'Yes'))),
		);
	} else {
		$form['submit_send_code'] = array(
			'#type' => 'submit',
			'#value' => t('Send Validation Code'),
			'#name' => 'submit_send_code',
			'#states' => array('visible' => array(":input[name=\"phone[change]\"]" => array('value' => 'Yes'))),
		);
	}

	if (user_access('sirius trust admin')) {
		$form['submit_skip'] = array(
			'#type' => 'submit',
			'#value' => t('(Admin) Skip Validation'),
			'#name' => 'submit_skip',
			'#states' => array('visible' => array(":input[name=\"phone[change]\"]" => array('value' => 'Yes'))),
		);
	}

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
		'#name' => 'submit_nochange',
		'#states' => array('visible' => array(":input[name=\"phone[change]\"]" => array('value' => 'No'))),
	);

	return $form;
}

function sirius_trust_election_wizard_phone_form_validate($form, &$form_state) {
	if ($form_state['values']['phone']['change'] == 'Yes') {
		$phone = $form_state['values']['phone']['new']['phone'];
		if (!$phone) {
			form_set_error("phone][new][phone", "Phone field is required.");
			return;
		}

		if (!sirius_phone_validate($phone)) {
			form_set_error("phone][new][phone", "Please enter a valid phone number.");
			return;
		}
	}
}

function sirius_trust_election_wizard_phone_form_submit($form, &$form_state) {
	$info = $form_state['info'];
	$data = $form_state['data'];
	$wizard_node = $form_state['wizard_node'];
	$current_step = $form_state['current_step'];

	// Don't want to change
	$data['phone']['change'] = $form_state['values']['phone']['change'];
	if ($form_state['values']['phone']['change'] != 'Yes') {
		return sirius_wizard_submit($info, $data, $wizard_node, $current_step, "Response saved.", array('nonsequential' => TRUE));
	}

	$action = $form_state['clicked_button']['#name'];
	$data['phone']['new'] = $form_state['values']['phone']['new'];
	$phone = sirius_phone_format($data['phone']['new']['phone']);

	// Administrator: save the data and skip to the next step
	if ($action == 'submit_skip') {
		return sirius_wizard_submit($info, $data, $wizard_node, $current_step, "Administrator skipped validation for $phone.", array('nonsequential' => TRUE));
	}

	// Flood control
	if (!sirius_flood_is_allowed('sirius_trust_election_wizard_phone')) {
		return drupal_set_message('Flood control: too many attempts. Please try again later.');
	}
	sirius_flood_register_event('sirius_trust_election_wizard_phone');

	// Send a validation code
	if ($action == 'submit_send_code') {
		$code = rand(100000, 999999);
		$data['phone']['code'] = $code;
		$params['sms'] = "$code - phone number verification code";
		$params['to'] = $phone;
		$result = sirius_sms_send($params);
		if ($result['success']) {
			return sirius_wizard_submit($info, $data, $wizard_node, $current_step, "A validation code has been sent to $phone.", array('incomplete' => TRUE, 'nonsequential' => TRUE));
		} else {
			return drupal_set_message("Unable to send a text message to $phone: " . $result['msg'], 'warning');
		}
	} 

	// Send a validation code
	if ($action == 'submit_reset') {
		$data['phone']['code'] = NULL;
		return sirius_wizard_submit($info, $data, $wizard_node, $current_step, "Phone number had been cleared.", array('incomplete' => TRUE, 'nonsequential' => TRUE));
	} 

	// Validate the code
	if ($action == "submit_validate") {
		$code = $data['phone']['code'];
		$submitted_code = trim($data['phone']['new']['code']);
		if ($code != $submitted_code) {
			return drupal_set_message("Incorrect code.", 'warning');
		} else {
			return sirius_wizard_submit($info, $data, $wizard_node, $current_step, "The phone number $phone has been validated.", array('nonsequential' => TRUE));
		}
	}

	// Don't know what to do.
	if ($acton) { 
		return drupal_set_message("Unknown action: $action.", 'warning');
	} else {
		return drupal_set_message("Please select one of the options below.", 'warning');
	}
}

function sirius_trust_election_wizard_phone_render($info, $data, $wizard_node, $current_step) {
	$html .= '<div class="sirius_wizard_render_section sirius_callout_basic">';
	$html .= '<h2>Phone</h2>';

	if ($data['phone']['change'] == 'Yes') {
		$html .= sirius_fakerender_field('Phone Number:', $data['phone']['new']['phone']);
	} else {
		$html .= '<p>' . t('Phone number is unchanged.') . '</p>';
	}
	$html .= '</div>';

	return $html;
}