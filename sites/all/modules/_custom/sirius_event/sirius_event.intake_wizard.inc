<?php

function sirius_event_intake_wizard_info() {
  $items = array();
  $items['event_intake'] = array(
    'name' => 'intake',
    'title' => 'Intake',
    'description' => '',
    'callback_step_list' => 'sirius_event_intake_wizard_steps',
  );
  return $items;
}

function sirius_event_intake_wizard_steps($data, $application_node) {

	$event_node = sirius_log_handler($application_node, 'sirius_event');
	$event_json = sirius_json_get($event_node);
	$application_json = sirius_json_get($application_node);

	$event_disclaimer_json = sirius_event_disclaimer_json($event_node);
	$disclaimer_nodes = sirius_event_disclaimer_nodes($event_disclaimer_json);

  $steps = array();
  $steps['intro'] = array('label' => 'Name', 'callback' => 'sirius_event_intake_wizard_intro');
  $steps['addr'] = array('label' => 'Address', 'callback' => 'sirius_event_intake_wizard_addr');
  $steps['phone'] = array('label' => 'Phone', 'callback' => 'sirius_event_intake_wizard_phone');
  $steps['email'] = array('label' => 'Email', 'callback' => 'sirius_event_intake_wizard_email');
  if ($disclaimer_nodes) { $steps['disclaimer'] = array('label' => 'Disclaimers', 'callback' => 'sirius_event_intake_wizard_disclaimers'); }
  $steps['survey'] = array('label' => 'Intake Form', 'callback' => 'sirius_event_intake_wizard_survey');

  $required_attachments = $event_json['intake']['attachments']['required'];
  if ($required_attachments) {
  	$required_attachments = array_filter(array_values($required_attachments));
  	foreach ($required_attachments as $tid) {
  		$attachment_term = taxonomy_term_load($tid);
  		$steps['attachment_' . $tid] = array('label' => $attachment_term->name, 'callback' => 'sirius_event_intake_wizard_attachment', 'callback_args' => array('tid' => $tid));
  	}
  } else {
  	$required_attachments = array();
  }

  $optional_attachments = $event_json['intake']['attachments']['optional'];
  if ($optional_attachments) {
  	$optional_attachments = array_filter(array_values($optional_attachments));
  	$optional_attachments = array_diff($optional_attachments, $required_attachments);
  	if ($optional_attachments) {
  		if ($required_attachments) { $label = 'Other Documents'; } else { $label = 'Documents'; }
	  	$steps['optional_attachments'] = array('label' => $label, 'callback' => 'sirius_event_intake_wizard_attachments_optional');
	  }
	}

	if ($application_json['wizard']['intake']['optional_attachments']['choice']) {
		$my_optional_attachments = array_filter(array_values($application_json['wizard']['intake']['optional_attachments']['choice']));
		foreach ($my_optional_attachments as $tid) {
  		$attachment_term = taxonomy_term_load($tid);
  		$steps['attachment_' . $tid] = array('label' => $attachment_term->name, 'callback' => 'sirius_event_intake_wizard_attachment', 'callback_args' => array('tid' => $tid));
  	}
	}

  if ($event_json['intake']['anonymous']['esignature']['def']) { $steps['signature'] = array('label' => 'Signature', 'callback' => 'sirius_event_intake_wizard_signature'); }

  $steps['deliver'] = array('label' => 'Submit', 'callback' => 'sirius_event_intake_wizard_deliver');
  $steps['review'] = array('label' => 'Review', 'callback' => 'sirius_event_intake_wizard_review');

  return $steps;
}

function sirius_event_intake_wizard_locked($event_node, $application_node) {
	// Not locked if it's in draft status
	if ($application_node->field_sirius_type['und'][0]['value'] == 'draft') { return FALSE; }

	// Not locked if we're an admin, meaning we can see the applications page
	sirius_require('sirius_event', 'sirius_event.page_intake_applications.inc');
	if (sirius_node_tab_event_intake_applications_access($event_node)) { return FALSE; }

	// Oh well.
	return TRUE;
}

/************************************************************************************************
 * INTRODUCTION
 ************************************************************************************************/

function sirius_event_intake_wizard_intro($info, $data, $application_node, $current_step) { return drupal_get_form('sirius_event_intake_wizard_intro_form', $info, $data, $application_node, $current_step); }

function sirius_event_intake_wizard_intro_form($form, &$form_state, $info, $data, $application_node, $current_step) {
	$form_state['info'] = $info;
	$form_state['data'] = $data;
	$form_state['application_node'] = $application_node;
	$form_state['current_step'] = $current_step;

	$event_node = sirius_log_handler($application_node, 'sirius_event');
	$event_json = sirius_json_get($event_node);
	$steps = sirius_wizard_steps($info, $data, $event_node);

	$intro_text = $event_json['intake']['anonymous']['intro'];
	if (!$intro_text) { $intro_text = $event_node->body['und'][0]['value']; }
	if (!$intro_text) { $intro_text = "Welcome to the intake wizard for <strong>" . $event_node->title . "</strong>"; }

	$form['#tree'] = TRUE;

	$form['intro'] = array('#markup' => '<div class="sirius_form_intro">' . $intro_text . '</div>');

	drupal_add_css(drupal_get_path('module', 'sirius_event') . '/css/survey.css', 'file');

	$form['survey_wrap_open'] = array('#markup' => '<div id="survey_wrap" class="survey_wrap_inpage sirius-form">');

	$form['identity']['name_first'] = array(
		'#type' => 'textfield',
		'#title' => t('First Name'),
		'#default_value' => $data['identity']['name_first'],
		'#required' => TRUE,
	);

	$form['identity']['name_last'] = array(
		'#type' => 'textfield',
		'#title' => t('Last Name'),
		'#default_value' => $data['identity']['name_last'],
		'#required' => TRUE,
	);

	$form['identity']['ssn'] = array(
		'#type' => 'textfield',
		'#title' => t('Social Security Number'),
		'#default_value' => $data['identity']['ssn'],
		'#required' => TRUE,
		'#size' => 14,
	);

	$form['survey_wrap_close'] = array('#markup' => '</div>');

	if (sirius_event_intake_wizard_locked($event_node, $application_node)) {
		$form['#disabled'] = TRUE; 
	} else {
		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit'),
		);
	}

	$prep_settings = array();
	$prep_settings['skip_required'] = TRUE;
	sirius_event_phonebank_prep_survey($form['identity'], array(), $prep_settings);

	return $form;
}

function sirius_event_intake_wizard_intro_form_validate($form, &$form_state) {
	$ssn = sirius_ssn_format($form_state['values']['identity']['ssn']);
	if (!sirius_ssn_validate($ssn)) {
		form_set_error('identity][ssn', 'Please enter a valid social security number.');
	}
	if (sirius_worker_get_by_ssn($ssn)) {
		form_set_error('identity][ssn', 'There is aleady a worker with that social security number.');
	}
}


function sirius_event_intake_wizard_intro_form_submit($form, &$form_state) {
	$info = $form_state['info'];
	$data = $form_state['data'];
	$application_node = $form_state['application_node'];

	$event_node = sirius_log_handler($application_node, 'sirius_event');
	$event_json = sirius_json_get($event_node);
	$current_step = $form_state['current_step'];

	$data['identity'] = $form_state['values']['identity'];
	$data['identity']['ssn'] = sirius_ssn_format($data['identity']['ssn']);

	sirius_event_intake_set_title($application_node);
	sirius_wizard_submit($info, $data, $application_node, $current_step, "Response saved.", array('nonsequential' => TRUE));
}

/************************************************************************************************
 * Address
 ************************************************************************************************/

function sirius_event_intake_wizard_addr($info, $data, $application_node, $current_step) { return drupal_get_form('sirius_event_intake_wizard_addr_form', $info, $data, $application_node, $current_step); }

function sirius_event_intake_wizard_addr_form($form, &$form_state, $info, $data, $application_node, $current_step) {
	$form_state['info'] = $info;
	$form_state['data'] = $data;
	$form_state['application_node'] = $application_node;
	$form_state['current_step'] = $current_step;

	$event_node = sirius_log_handler($application_node, 'sirius_event');
	$event_json = sirius_json_get($event_node);
	$steps = sirius_wizard_steps($info, $data, $event_node);

	$form['#tree'] = TRUE;

	drupal_add_css(drupal_get_path('module', 'sirius_event') . '/css/survey.css', 'file');

	$form['survey_wrap_open'] = array('#markup' => '<div id="survey_wrap" class="survey_wrap_inpage sirius-form">');

	$form['addr']['intro'] = array(
		'#markup' => '<div class="sirius_form_intro">' . t('Please enter your postal address.') . '</div>',
	);

	$form['addr']['thoroughfare'] = array(
		'#type' => 'textfield',
		'#title' => t('Address'),
		'#default_value' => $data['addr']['thoroughfare'],
		'#required' => TRUE,
	);

	$form['addr']['premise'] = array(
		'#type' => 'textfield',
		'#title' => t('Address 2'),
		'#default_value' => $data['addr']['premise'],
	);

	$form['addr']['locality'] = array(
		'#type' => 'textfield',
		'#title' => t('City'),
		'#default_value' => $data['addr']['locality'],
		'#required' => TRUE,
	);

	$form['addr']['administrative_area'] = array(
		'#type' => 'select',
		'#title' => t('State'),
		'#default_value' => $data['addr']['administrative_area'],
		'#required' => TRUE,
		'#options' => array(NULL => t('-- Select --')) + sirius_addr_states(),
	);

	$form['addr']['postal_code'] = array(
		'#type' => 'textfield',
		'#title' => t('Zip'),
		'#default_value' => $data['addr']['postal_code'],
		'#required' => TRUE,
		'#size' => 5,
	);

	if (sirius_event_intake_wizard_locked($event_node, $application_node)) {
		$form['#disabled'] = TRUE; 
	} else {
		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit'),
		);
	}

	$form['survey_wrap_close'] = array('#markup' => '</div>');

	$prep_settings = array();
	$prep_settings['skip_required'] = TRUE;
	sirius_event_phonebank_prep_survey($form['addr'], array(), $prep_settings);

	return $form;
}

function sirius_event_intake_wizard_addr_form_validate($form, &$form_state) {
	$addr = $form_state['values']['addr'];
	$result = sirius_addr_validate_full($addr);
	if (!$result['success']) {
		form_set_error('addr', $result['msg']);
	}
}

function sirius_event_intake_wizard_addr_form_submit($form, &$form_state) {
	$info = $form_state['info'];
	$data = $form_state['data'];
	$application_node = $form_state['application_node'];

	$event_node = sirius_log_handler($application_node, 'sirius_event');
	$event_json = sirius_json_get($event_node);
	$current_step = $form_state['current_step'];

	$data['addr'] = $form_state['values']['addr'];
	sirius_wizard_submit($info, $data, $application_node, $current_step, "Address saved.", array('nonsequential' => TRUE));

	sirius_event_intake_set_title($application_node);
}

/************************************************************************************************
 * PHONE
 ************************************************************************************************/

function sirius_event_intake_wizard_phone($info, $data, $application_node, $current_step) { return drupal_get_form('sirius_event_intake_wizard_phone_form', $info, $data, $application_node, $current_step); }

function sirius_event_intake_wizard_phone_form($form, &$form_state, $info, $data, $application_node, $current_step) {
	$form_state['info'] = $info;
	$form_state['data'] = $data;
	$form_state['application_node'] = $application_node;
	$form_state['current_step'] = $current_step;

	$event_node = sirius_log_handler($application_node, 'sirius_event');
	$event_json = sirius_json_get($event_node);
	$steps = sirius_wizard_steps($info, $data, $event_node);

	$form['#tree'] = TRUE;

	drupal_add_css(drupal_get_path('module', 'sirius_event') . '/css/survey.css', 'file');

	$form['survey_wrap_open'] = array('#markup' => '<div id="survey_wrap" class="survey_wrap_inpage sirius-form">');

	$form['validator']['intro'] = array(
		'#markup' => '<div class="sirius_form_intro">' . t('Please enter a mobile phone number. We will send a text message containing a confirmation code to this phone.') . '</div>',
	);

	$phone = $data['phone'];
	$code = $data['phone_code'];

	$form['validator']['phone'] = array(
		'#type' => 'textfield',
		'#title' => t('Phone Number'),
		'#default_value' => $phone,
		'#required' => TRUE,
		'#size' => 12,
	);
	if ($code) { $form['validator']['phone']['#disabled'] = TRUE; }

	if ($code) {
		$form['validator']['code'] = array(
			'#type' => 'textfield',
			'#title' => t('Validation Code'),
			// '#required' => TRUE,
			'#size' => 6,
		);
	}

	if ($code) {
		$form['validator']['submit_code'] = array(
			'#type' => 'submit',
			'#value' => t('Enter Code'),
			'#name' => 'code',
		);
		$form['validator']['submit_reset'] = array(
			'#type' => 'submit',
			'#value' => t('Send a New Code'),
			'#name' => 'reset',
		);

		sirius_require('sirius_event', 'sirius_event.page_intake_applications.inc');
		if (sirius_node_tab_event_intake_applications_access($event_node)) {
			$form['validator']['submit_skip'] = array(
				'#type' => 'submit',
				'#value' => t('(Admin) Skip Validation'),
				'#name' => 'skip',
			);
		}
	} else {
		$form['validator']['submit_phone'] = array(
			'#type' => 'submit',
			'#value' => t('Send Validation Code'),
			'#name' => 'phone',
		);
	}
	$form['survey_wrap_close'] = array('#markup' => '</div>');

	$prep_settings = array();
	$prep_settings['skip_required'] = TRUE;
	sirius_event_phonebank_prep_survey($form['validator'], array(), $prep_settings);

	return $form;
}

function sirius_event_intake_wizard_phone_form_validate($form, &$form_state) {
	if ($form_state['values']['validator']['phone']) {
		if (!sirius_phone_validate($form_state['values']['validator']['phone'])) {
			form_set_error('validator][phone', 'Please enter a valid phone number.');
		}
	}
}

function sirius_event_intake_wizard_phone_form_submit($form, &$form_state) {
	$info = $form_state['info'];
	$data = $form_state['data'];
	$application_node = $form_state['application_node'];

	$event_node = sirius_log_handler($application_node, 'sirius_event');
	$event_json = sirius_json_get($event_node);
	$current_step = $form_state['current_step'];

	$phone = $data['phone'];
	$action = $form_state['clicked_button']['#name'];

	if (!$phone || $action == 'phone') {
		$phone = sirius_phone_format($form_state['values']['validator']['phone']);
		$data['phone'] = $phone;

		$code = rand(100000, 999999);
		$data['phone_code'] = $code;
		$params['sms'] = "$code - phone number verification code";
		$params['to'] = $phone;
		$result = sirius_sms_send($params);
		if ($result['success']) {
			sirius_wizard_submit($info, $data, $application_node, $current_step, "A validation code has been sent to $phone.", array('incomplete' => TRUE, 'nonsequential' => TRUE));
		} else {
			drupal_set_message("Unable to send a text message to $phone: " . $result['msg'], 'warning');
		}
	} else if ($data['phone_code'] && $action == 'code') {
		$code = trim($form_state['values']['validator']['code']);
		if ($code != $data['phone_code']) {
			drupal_set_message("Incorrect code.", 'warning');
		} else {
			sirius_wizard_submit($info, $data, $application_node, $current_step, "The phone number $phone has been validated.", array('nonsequential' => TRUE));
		}
	} else if ($action == 'skip') {
		sirius_wizard_submit($info, $data, $application_node, $current_step, "Administrator skipped validation for $phone.", array('nonsequential' => TRUE));
	} else {
		$data['phone_code'] = NULL;
		sirius_wizard_submit($info, $data, $application_node, $current_step, "Please enter the phone number to receive a validation code.", array('incomplete' => TRUE, 'nonsequential' => TRUE));
	}

	sirius_event_intake_set_title($application_node);
}

/************************************************************************************************
 * Email
 ************************************************************************************************/

function sirius_event_intake_wizard_email($info, $data, $application_node, $current_step) { return drupal_get_form('sirius_event_intake_wizard_email_form', $info, $data, $application_node, $current_step); }

function sirius_event_intake_wizard_email_form($form, &$form_state, $info, $data, $application_node, $current_step) {
	$form_state['info'] = $info;
	$form_state['data'] = $data;
	$form_state['application_node'] = $application_node;
	$form_state['current_step'] = $current_step;

	$event_node = sirius_log_handler($application_node, 'sirius_event');
	$event_json = sirius_json_get($event_node);
	$steps = sirius_wizard_steps($info, $data, $event_node);

	$form['#tree'] = TRUE;

	drupal_add_css(drupal_get_path('module', 'sirius_event') . '/css/survey.css', 'file');

	$form['survey_wrap_open'] = array('#markup' => '<div id="survey_wrap" class="survey_wrap_inpage sirius-form">');

	$form['validator']['intro'] = array(
		'#markup' => '<div class="sirius_form_intro">' . t('Please enter a mobile email address. We will send a message containing a confirmation code to this address.') . '</div>',
	);

	$email = $data['email'];
	$code = $data['email_code'];

	$form['validator']['email'] = array(
		'#type' => 'textfield',
		'#title' => t('Email Address'),
		'#default_value' => $email,
		'#required' => TRUE,
	);
	if ($code) { $form['validator']['email']['#disabled'] = TRUE; }

	if ($code) {
		$form['validator']['code'] = array(
			'#type' => 'textfield',
			'#title' => t('Validation Code'),
			// '#required' => TRUE,
			'#size' => 6,
		);
	}

	if ($code) {
		$form['validator']['submit_code'] = array(
			'#type' => 'submit',
			'#value' => t('Enter Code'),
			'#name' => 'code',
		);
		$form['validator']['submit_reset'] = array(
			'#type' => 'submit',
			'#value' => t('Send a New Code'),
			'#name' => 'reset',
		);

		sirius_require('sirius_event', 'sirius_event.page_intake_applications.inc');
		if (sirius_node_tab_event_intake_applications_access($event_node)) {
			$form['validator']['submit_skip'] = array(
				'#type' => 'submit',
				'#value' => t('(Admin) Skip Validation'),
				'#name' => 'skip',
			);
		}
	} else {
		$form['validator']['submit_email'] = array(
			'#type' => 'submit',
			'#value' => t('Send Validation Code'),
			'#name' => 'email',
		);
	}
	$form['survey_wrap_close'] = array('#markup' => '</div>');

	$prep_settings = array();
	$prep_settings['skip_required'] = TRUE;
	sirius_event_phonebank_prep_survey($form['validator'], array(), $prep_settings);

	return $form;
}

function sirius_event_intake_wizard_email_form_validate($form, &$form_state) {
	if ($form_state['values']['validator']['email']) {
		if (!sirius_email_validate($form_state['values']['validator']['email'])) {
			form_set_error('validator][email', 'Please enter a valid email address.');
		}
	}
}

function sirius_event_intake_wizard_email_form_submit($form, &$form_state) {
	$info = $form_state['info'];
	$data = $form_state['data'];
	$application_node = $form_state['application_node'];

	$event_node = sirius_log_handler($application_node, 'sirius_event');
	$event_json = sirius_json_get($event_node);
	$current_step = $form_state['current_step'];

	$email = $data['email'];
	$action = $form_state['clicked_button']['#name'];

	if (!$email || $action == 'email') {
		$email = sirius_email_format($form_state['values']['validator']['email']);
		$data['email'] = $email;

		$code = rand(100000, 999999);
		$data['email_code'] = $code;
		$params['subject'] = "Email Validation Code";
		global $base_url;
		$url = $base_url . '/' . current_path();
		$params['body'] = "Your email validation code is <strong>$code</strong><br /><br />";
		$params['body'] .= "Please go to <a href=\"$url\">$url</a> and enter this validation code to continue with your application.";
		$params['to'] = $email;
		$result = sirius_email_send($params);
		if ($result['success']) {
			sirius_wizard_submit($info, $data, $application_node, $current_step, "A validation code has been sent to $email.", array('incomplete' => TRUE, 'nonsequential' => TRUE));
		} else {
			drupal_set_message("Unable to send a message to $email: " . $result['msg'], 'warning');
		}
	} else if ($data['email_code'] && $action == 'code') {
		$code = trim($form_state['values']['validator']['code']);
		if ($code != $data['email_code']) {
			drupal_set_message("Incorrect code.", 'warning');
		} else {
			sirius_wizard_submit($info, $data, $application_node, $current_step, "The email address $email has been validated.", array('nonsequential' => TRUE));
		}
	} else if ($action == 'skip') {
		sirius_wizard_submit($info, $data, $application_node, $current_step, "Administrator skipped validation for $email.", array('nonsequential' => TRUE));
	} else {
		$data['email_code'] = NULL;
		sirius_wizard_submit($info, $data, $application_node, $current_step, "Please enter the email address to receive a validation code.", array('incomplete' => TRUE, 'nonsequential' => TRUE));
	}

	sirius_event_intake_set_title($application_node);
}

/************************************************************************************************
 * DISCLAIMERS
 ************************************************************************************************/

function sirius_event_intake_wizard_disclaimers($info, $data, $application_node, $current_step) { return drupal_get_form('sirius_event_intake_wizard_disclaimers_form', $info, $data, $application_node, $current_step); }

function sirius_event_intake_wizard_disclaimers_form($form, &$form_state, $info, $data, $application_node, $current_step) {
	$form_state['info'] = $info;
	$form_state['data'] = $data;
	$form_state['application_node'] = $application_node;
	$form_state['current_step'] = $current_step;

	$event_node = sirius_log_handler($application_node, 'sirius_event');
	$event_json = sirius_json_get($event_node);
	$steps = sirius_wizard_steps($info, $data, $event_node);

	$event_disclaimer_json = sirius_event_disclaimer_json($event_node);
	$disclaimer_nodes = sirius_event_disclaimer_nodes($event_disclaimer_json);
	$application_json = sirius_json_get($application_node);

	$form['#tree'] = TRUE;

	if (!$disclaimer_nodes) {
		drupal_set_message("No disclaimers have been configured for this event.");
		return array();
	}

	$accepted_all = TRUE;
	foreach ($disclaimer_nodes as $disclaimer_nid => $disclaimer_node) {
		$disclaimer_json = sirius_json_get($disclaimer_node)['disclaimer'];
		$acceptance_json = $application_json['disclaimer'][$disclaimer_nid]['acceptances'][0];
		if (!$acceptance_json) { $accepted_all = FALSE; }
		$form['disclaimer'][$disclaimer_nid] = sirius_disclaimer_render_form($disclaimer_json, $acceptance_json);			
	}

	if (sirius_event_intake_wizard_locked($event_node, $application_node)) {
		$form['#disabled'] = TRUE; 
	} else {
		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit'),
		);
	}

	return $form;
}

function sirius_event_intake_wizard_disclaimers_form_submit($form, &$form_state) {
	$info = $form_state['info'];
	$data = $form_state['data'];
	$application_node = $form_state['application_node'];
	$event_node = sirius_log_handler($application_node, 'sirius_event');
	$event_json = sirius_json_get($event_node);
	$current_step = $form_state['current_step'];

	$disclaimer_nodes = node_load_multiple(array_keys($form_state['values']['disclaimer']));
	sirius_disclaimer_accept($application_node, $disclaimer_nodes, array());
	
	sirius_event_intake_set_title($application_node);
	sirius_wizard_submit($info, $data, $application_node, $current_step, "Response saved.", array('nonsequential' => TRUE));
}

/************************************************************************************************
 * SURVEY
 ************************************************************************************************/

function sirius_event_intake_wizard_survey($info, $data, $application_node, $current_step) { return drupal_get_form('sirius_event_intake_wizard_survey_form', $info, $data, $application_node, $current_step); }

function sirius_event_intake_wizard_survey_form($form, &$form_state, $info, $data, $application_node, $current_step) {
	$form_state['info'] = $info;
	$form_state['data'] = $data;
	$form_state['application_node'] = $application_node;
	$form_state['current_step'] = $current_step;

	$event_node = sirius_log_handler($application_node, 'sirius_event');
	$event_json = sirius_json_get($event_node);
	$steps = sirius_wizard_steps($info, $data, $event_node);

	$survey_node = node_load($event_json['intake']['survey']);
	$survey_json = sirius_json_get($survey_node);
	$survey_form = sirius_jsonfield_util_form_get($survey_json, 'default');

	$application_json = sirius_json_get($application_node);
	$application_results = sirius_jsonfield_util_results_get($application_json, 'survey');

	$prep_settings = array();
	$prep_settings['skip_required'] = TRUE;
	sirius_event_phonebank_prep_survey($survey_form, $application_results, $prep_settings);
	drupal_add_css(drupal_get_path('module', 'sirius_event') . '/css/survey.css', 'file');

	$form['#tree'] = TRUE;

	$form['survey_wrap_open'] = array('#markup' => '<div id="survey_wrap" class="survey_wrap_inpage sirius-form">');

	$form['survey'] = $survey_form;

	$form['survey_wrap_close'] = array('#markup' => '</div>');

	if (sirius_event_intake_wizard_locked($event_node, $application_node)) {
		$form['#disabled'] = TRUE; 
	} else {
		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit'),
		);
	}

	return $form;
}

function sirius_event_intake_wizard_survey_form_submit($form, &$form_state) {
	$info = $form_state['info'];
	$data = $form_state['data'];
	$application_node = $form_state['application_node'];
  $application_json = sirius_json_get($application_node);
	$event_node = sirius_log_handler($application_node, 'sirius_event');
	$event_json = sirius_json_get($event_node);
	$current_step = $form_state['current_step'];

	$survey_node = node_load($event_json['intake']['survey']);
	$survey_json = sirius_json_get($survey_node);
	$survey_form = sirius_jsonfield_util_form_get($survey_json, 'default');

  // Set the results
	sirius_jsonfield_util_results_set($application_json, 'survey', $form_state['values']['survey']);
	sirius_json_set($application_node, $application_json);
	node_save($application_node);

	sirius_event_intake_set_title($application_node);
	sirius_wizard_submit($info, $data, $application_node, $current_step, "Survey responses saved.", array('nonsequential' => TRUE));
}

/************************************************************************************************
 * SIGNATURE
 ************************************************************************************************/

function sirius_event_intake_wizard_signature($info, $data, $application_node, $current_step) { return drupal_get_form('sirius_event_intake_wizard_signature_form', $info, $data, $application_node, $current_step); }

function sirius_event_intake_wizard_signature_form($form, &$form_state, $info, $data, $application_node, $current_step) {
	sirius_esignature_init();
	drupal_add_css(drupal_get_path('module', 'sirius_training') . '/css/training.css');
	sirius_require('sirius', 'sirius.page_entity_signatures.inc');

	$form_state['info'] = $info;
	$form_state['data'] = $data;
	$form_state['application_node'] = $application_node;
	$form_state['current_step'] = $current_step;

	$event_node = sirius_log_handler($application_node, 'sirius_event');
	$event_json = sirius_json_get($event_node);
	$steps = sirius_wizard_steps($info, $data, $event_node);

	$all_defs = json_decode(sirius_domain_variable_get('sirius_esignature_defs'), TRUE);
	$application_json = sirius_json_get($application_node);

	$which = $event_json['intake']['anonymous']['esignature']['def'];
	$statement = $event_json['intake']['anonymous']['esignature']['statement'];
	if (!$statement) { $statement = $application_node->title; }

	$esignature = sirius_jsonfield_util_esignature_get($application_json, $which);
	if ($esignature) {
		$form['signature'] = array('#markup' => sirius_esignature_render($esignature, 'full', $application_node->nid, $which));
		$form['action'] = array(
			'#type' => 'value', 
			'#value' => 'revoke'
		);
		if (sirius_event_intake_wizard_locked($event_node, $application_node)) {
			$form['#disabled'] = TRUE; 
		} else {
			$form['submit'] = array(
				'#type' => 'submit', 
				'#value' => 'Revoke'
			);
		}
	} else {
		sirius_esignature_form_prepare($form, $which, "", $statement, array('show_name' => FALSE, 'show_org' => FALSE));
	}

	return $form;
}

function sirius_event_intake_wizard_signature_form_submit($form, &$form_state) {
	$info = $form_state['info'];
	$data = $form_state['data'];
	$application_node = $form_state['application_node'];
	$application_json = sirius_json_get($application_node);

	$event_node = sirius_log_handler($application_node, 'sirius_event');
	$event_json = sirius_json_get($event_node);
	$current_step = $form_state['current_step'];

	$which = $event_json['intake']['anonymous']['esignature']['def'];
	$action = $form_state['values']['action'];
	if ($action == 'revoke') {
		drupal_set_message("Revoking $which...");
		sirius_jsonfield_util_esignature_set($application_json, $which, NULL);
		sirius_json_set($application_node, $application_json);
		sirius_event_intake_set_title($application_node);
		sirius_wizard_submit($info, $data, $application_node, $current_step, "Signature revoked.", array('incomplete' => TRUE, 'nonsequential' => TRUE));
	} else {
		$signature = sirius_esignature_form_get_json($form, $form_state, $which);
		sirius_jsonfield_util_esignature_set($application_json, $which, $signature);
		sirius_json_set($application_node, $application_json);
		sirius_event_intake_set_title($application_node);
		sirius_wizard_submit($info, $data, $application_node, $current_step, "Response saved.", array('nonsequential' => TRUE));
	}
}

/************************************************************************************************
 * OPTIONAL ATTACHMENTS
 ************************************************************************************************/

function sirius_event_intake_wizard_attachments_optional($info, $data, $application_node, $current_step) { return drupal_get_form('sirius_event_intake_wizard_attachments_optional_form', $info, $data, $application_node, $current_step); }

function sirius_event_intake_wizard_attachments_optional_form($form, &$form_state, $info, $data, $application_node, $current_step) {
	$form_state['info'] = $info;
	$form_state['data'] = $data;
	$form_state['application_node'] = $application_node;
	$form_state['current_step'] = $current_step;

	$event_node = sirius_log_handler($application_node, 'sirius_event');
	$event_json = sirius_json_get($event_node);
	$steps = sirius_wizard_steps($info, $data, $event_node);

 	$required_attachments = $event_json['intake']['attachments']['required'];
  if ($required_attachments) {
  	$required_attachments = array_filter(array_values($required_attachments));
  } else {
  	$required_attachments = array();
  }

  $optional_attachments = $event_json['intake']['attachments']['optional'];
  if ($optional_attachments) {
  	$optional_attachments = array_filter(array_values($optional_attachments));
  	$optional_attachments = array_diff($optional_attachments, $required_attachments);
	}
	if (!$optional_attachments) { $optional_attachments = array(); }

	$form['#tree'] = TRUE;

	drupal_add_css(drupal_get_path('module', 'sirius_event') . '/css/survey.css', 'file');

	$form['survey_wrap_open'] = array('#markup' => '<div id="survey_wrap" class="survey_wrap_inpage sirius-form">');

	$form['optional_attachments']['intro'] = array(
		'#markup' => '<div class="sirius_form_intro">' . t('Please select the documents that you wish to include with this application.') . '</div>',
	);

	$options = array();
	foreach ($optional_attachments as $tid) {
		$term = taxonomy_term_load($tid);
		$options[$tid] = $term->name;
	}

	$form['optional_attachments']['choice'] = array(
		'#type' => 'checkboxes',
		'#title' => t('Documents'),
		'#options' => $options,
	);
	if ($data['optional_attachments']['choice']) { $form['optional_attachments']['choice']['#default_value'] = $data['optional_attachments']['choice']; }

	if (sirius_event_intake_wizard_locked($event_node, $application_node)) {
		$form['#disabled'] = TRUE; 
	} else {
		$form['submit'] = array(
			'#type' => 'submit', 
			'#value' => 'Submit'
		);
	}

	$form['survey_wrap_close'] = array('#markup' => '</div>');

	$prep_settings = array();
	$prep_settings['skip_required'] = TRUE;
	sirius_event_phonebank_prep_survey($form['optional_attachments'], array(), $prep_settings);

	return $form;
}

function sirius_event_intake_wizard_attachments_optional_form_submit($form, &$form_state) {
	$info = $form_state['info'];
	$data = $form_state['data'];
	$application_node = $form_state['application_node'];

	$event_node = sirius_log_handler($application_node, 'sirius_event');
	$event_json = sirius_json_get($event_node);
	$current_step = $form_state['current_step'];

	$data['optional_attachments'] = $form_state['values']['optional_attachments'];

	sirius_wizard_submit($info, $data, $application_node, $current_step, "Please enter the email address to receive a validation code.", array('nonsequential' => TRUE));
	sirius_event_intake_set_title($application_node);
}

/************************************************************************************************
 * ATTACHMENT
 ************************************************************************************************/

function sirius_event_intake_wizard_attachment($info, $data, $application_node, $current_step, $args) { return drupal_get_form('sirius_event_intake_wizard_attachment_form', $info, $data, $application_node, $current_step, $args); }

function sirius_event_intake_wizard_attachment_form($form, &$form_state, $info, $data, $application_node, $current_step, $args) {
	$form_state['info'] = $info;
	$form_state['data'] = $data;
	$form_state['application_node'] = $application_node;
	$form_state['current_step'] = $current_step;

	$event_node = sirius_log_handler($application_node, 'sirius_event');
	$event_json = sirius_json_get($event_node);
	$steps = sirius_wizard_steps($info, $data, $event_node);

	$attachment_tid = $args['tid'];
	$form_state['attachment_tid'] = $attachment_tid;
	$attachment_term = taxonomy_term_load($attachment_tid);
	if (!$attachment_term) {
		drupal_set_message("Unable to find the attachment definition (uuid=$attachment_tid). Please check the event settings.", 'error');
		return array();
	}

	$attachment = $data['attachments'][$attachment_tid];

	$form['#tree'] = TRUE;

	$intro_text = $attachment_term->description;
	if ($intro_text) {
		$form['intro'] = array('#markup' => '<div class="sirius_form_intro">' . $intro_text . '<h2>' . $attachment_term->name . '</h2></div>');
	}

	sirius_require('sirius_event', 'sirius_event.page_intake_applications.inc');
	$is_admin = sirius_node_tab_event_intake_applications_access($event_node);

	if ($attachment) {
		$fid = $attachment['fid'];
		$file = (array) file_load($fid);
		$details = '<strong>Current File:</strong> ';
		if ($is_admin) {
			$url = file_create_url($file['uri']);
			$details .= "<a href=\"$url\">";
		}
		$details .= $attachment['origname'];
		$details .= ' (';
		$details .= format_size($file['filesize']);
		$details .= ')';
		if ($is_admin) {
			$details .= '</a>';
		}

		$form['current'] = array('#markup' => $details);
		$upload_title = "Replace With";
	} else {
		$upload_title = "Upload";
	}


	$form["file"] = array(
		'#type' => 'file',
		'#title' => $upload_title,
		// '#required' => TRUE,
	);

	if (sirius_event_intake_wizard_locked($event_node, $application_node)) {
		$form['#disabled'] = TRUE; 
	} else {
		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit'),
		);
	}

	return $form;
}

function sirius_event_intake_wizard_attachment_form_validate($form, &$form_state) {
	$info = field_info_instance('node', 'field_sirius_attachments', 'sirius_log');
	$extensions = $info['settings']['file_extensions'];
  $file = file_save_upload("file", array(
    'file_validate_extensions' => array($extensions),
  ));
  if ($file) {
    $name = rand();
    if ($file = file_move($file, 'private://attach_' . rand())) {
      $form_state['storage']["file"] = $file;
    }
    else {
      form_set_error("file", t("Failed to write the uploaded file to the site's file folder."));
    }
  } else {
     form_set_error("file", t("Please upload a file."));
  }
}

function sirius_event_intake_wizard_attachment_form_submit($form, &$form_state) {
	$info = $form_state['info'];
	$data = $form_state['data'];
	$application_node = $form_state['application_node'];

	$event_node = sirius_log_handler($application_node, 'sirius_event');
	$event_json = sirius_json_get($event_node);
	$current_step = $form_state['current_step'];

	$attachment_tid = $form_state['attachment_tid'];

  $file = $form_state['storage']["file"];
  if ($file) {
		$file = (array) $file;
		$file['display'] = 1;
		unset($form_state['storage']["attach_$delta"]);
		unset($file['destination']);
		unset($file['uri']);

		// Remove the previous file
		$old_fid = $data['attachments'][$attachment_tid]['fid'];
		if ($old_fid) {
			if ($application_node->field_sirius_attachments['und']) {
				foreach ($application_node->field_sirius_attachments['und'] as $delta => $entry) {
					if ($entry['fid'] == $old_fid) { 
						unset($application_node->field_sirius_attachments['und'][$delta]);
					}
				}
			}
		}

		// Add the new
		$application_node->field_sirius_attachments['und'][] = $file;
		$data['attachments'][$attachment_tid] = $file;
	}

	sirius_event_intake_set_title($application_node);
	sirius_wizard_submit($info, $data, $application_node, $current_step, "Attachment saved.", array('nonsequential' => TRUE));
}

/************************************************************************************************
 * DELIVER / SUBMIT
 ************************************************************************************************/

function sirius_event_intake_wizard_deliver($info, $data, $application_node, $current_step) { return drupal_get_form('sirius_event_intake_wizard_deliver_form', $info, $data, $application_node, $current_step); }

function sirius_event_intake_wizard_deliver_form($form, &$form_state, $info, $data, $application_node, $current_step) {
	$form_state['info'] = $info;
	$form_state['data'] = $data;
	$form_state['application_node'] = $application_node;
	$form_state['current_step'] = $current_step;

	$event_node = sirius_log_handler($application_node, 'sirius_event');
	$event_json = sirius_json_get($event_node);
	$steps = sirius_wizard_steps($info, $data, $event_node);

	$intro_text = $event_json['intake']['anonymous']['submit'];
	if (!$intro_text) {
		$intro_text = "Click 'Submit' below to submit your application. After your application is submitted, it will be locked and no additional changes will be permitted.";
	}

	$form['#tree'] = TRUE;

	$form['intro'] = array('#markup' => '<div class="sirius_form_intro">' . $intro_text . '</div>');

	if (sirius_event_intake_wizard_locked($event_node, $application_node)) {
		$form['#disabled'] = TRUE; 
	} else {
		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit and Lock Application'),
		);
	}

	return $form;
}

function sirius_event_intake_wizard_deliver_form_submit($form, &$form_state) {
	$info = $form_state['info'];
	$data = $form_state['data'];
	$application_node = $form_state['application_node'];

	$event_node = sirius_log_handler($application_node, 'sirius_event');
	$event_json = sirius_json_get($event_node);
	$current_step = $form_state['current_step'];

	$data['submitted_ts'] = time();

	$application_node->field_sirius_type['und'][0]['value'] = 'submitted';
	sirius_jsonfield_log($application_node, "submitted", "Application submitted.");
	sirius_event_intake_set_title($application_node);
	sirius_wizard_submit($info, $data, $application_node, $current_step, "Response saved.", array('nonsequential' => TRUE));
}

/************************************************************************************************
 * REVIEW
 ************************************************************************************************/

function sirius_event_intake_wizard_review($info, $data, $application_node, $current_step) { return drupal_get_form('sirius_event_intake_wizard_review_form', $info, $data, $application_node, $current_step); }

function sirius_event_intake_wizard_review_form($form, &$form_state, $info, $data, $application_node, $current_step) {
	$form_state['info'] = $info;
	$form_state['data'] = $data;
	$form_state['application_node'] = $application_node;
	$form_state['current_step'] = $current_step;

	$event_node = sirius_log_handler($application_node, 'sirius_event');
	$event_json = sirius_json_get($event_node);
	$steps = sirius_wizard_steps($info, $data, $event_node);

	$survey_node = node_load($event_json['intake']['survey']);
	$survey_json = sirius_json_get($survey_node);
	$survey_form = sirius_jsonfield_util_form_get($survey_json, 'default');

	$application_json = sirius_json_get($application_node);
	$application_results = sirius_jsonfield_util_results_get($application_json, 'survey');

	$prep_settings = $event_json;
	$prep_settings['skip_required'] = TRUE;
	sirius_event_phonebank_prep_survey($survey_form, $application_results, $prep_settings);
	drupal_add_css(drupal_get_path('module', 'sirius_event') . '/css/survey.css', 'file');

	$form['#tree'] = TRUE;

	$submitted_ts = $data['submitted_ts'];
	if ($submitted_ts) {
		$form['status'] = array('#markup' => '<p>Application submitted on <strong>' . date('l, F d Y - g:ia', $submitted_ts) . '</strong></p>');
	}

	$form['survey_wrap_open'] = array('#markup' => '<div id="survey_wrap" class="survey_wrap_inpage sirius-form">');

	$form['survey'] = $survey_form;

	$form['survey_wrap_close'] = array('#markup' => '</div>');
	$form['#disabled'] = TRUE;

	return $form;
}