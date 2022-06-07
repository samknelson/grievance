<?php

/**
 * Implements hook_sirius_wizard_submit_alter()
 * 
 * If there is a member ID, automatically accept the application
 */

function sirius_bsh_sirius_wizard_submit_alter(&$alterable) {
	$info = $alterable['info'];
	$data = $alterable['data'];
	$application_node = $alterable['node'];
	$current_step = $alterable['current_step'];
	$msg = $alterable['msg'];
	$options = $alterable['options'];

	// We're only interested in the "deliver" step of event intake wizards
	if ($info['name'] != 'intake') { return; }
	if ($current_step != 'deliver') { return; }

	// Must have a BSH member ID
	$id2 = $data['bsh_worker']['id'];
	if (!$id2) { return; }

	// Set up our "accept options"
	$event_node = sirius_log_handler($application_node, 'sirius_event');
	$event_json = sirius_json_get($event_node);

	$accept_options = array();
	$accept_options['id2'] = $id2;
	$accept_options['accept']['notification']['medium'] = 'both';
	$accept_options['accept']['notification']['email_subject'] = $event_json['intake']['notifications']['accept']['email_subject'];
  $accept_options['accept']['notification']['email_body'] = $event_json['intake']['notifications']['accept']['email_body'];
  $accept_options['accept']['notification']['sms'] = $event_json['intake']['notifications']['accept']['sms'];
  $accept_options['accept']['notification']['silent'] = TRUE;
	$result = sirius_event_intake_application_accept($application_node, $accept_options);
	if ($result['success']) {
		drupal_set_message("Login instructions have been sent to you by email and text message.");
	} else {
		drupal_set_message("We were not able to automatically create your user: " . $result['msg'] . " A site administrator will review your application will send you login instructions.");
	}
}

/**
 * Implements hook_sirius_wizard_steps_alter()
 * 
 * Add a step to the event intake wizard to run a check against the worker database
 */
function sirius_bsh_sirius_wizard_steps_alter(&$steps, $context) {
	$info = $context['info'];
	$data = $context['data'];
	$node = $context['node'];

	if ($info['name'] != 'intake') { return; }
	if ($steps['bsh_lookup']) { return; }

	$new_step = array(
		'label' => 'Local 5 Member Lookup',
		'callback' => 'sirius_bsh_event_intake_worker_lookup'
	);

	$new_steps = array();
	foreach ($steps as $key => $step) {
		if ($key == 'deliver') {
			$new_steps['bsh_lookup'] = $new_step;
		}
		$new_steps[$key] = $step;
	}
	$steps = $new_steps;
}

/************************************************************************************************
 * Worker Lookup wizard step
 ************************************************************************************************/

function sirius_bsh_event_intake_worker_lookup($info, $data, $application_node, $current_step) { return drupal_get_form('sirius_bsh_event_intake_worker_lookup_form', $info, $data, $application_node, $current_step); }

function sirius_bsh_event_intake_worker_lookup_form($form, &$form_state, $info, $data, $application_node, $current_step) {
	$form_state['info'] = $info;
	$form_state['data'] = $data;
	$form_state['application_node'] = $application_node;
	$form_state['current_step'] = $current_step;

	$event_node = sirius_log_handler($application_node, 'sirius_event');
	$event_json = sirius_json_get($event_node);
	$steps = sirius_wizard_steps($info, $data, $event_node);

	$intro_text = "<p>";
	$intro_text .= '<strong>Local 5 Members:</strong> ';
	$intro_text .= "Please enter your Local 5 member ID. This is a 4, 5, or 6-digit number. Validating your ID will allow us to approve your application and create your account immediately.";
	$intro_text .= "</p>";

	$intro_text .= "<p>";
	$intro_text .= '<strong>All other users:</strong> ';
	$intro_text .= "Please leave the ID field blank. Your application will be approved by an administrator within one business day.";
	$intro_text .= "</p>";

	$form['#tree'] = TRUE;

	$form['intro'] = array('#markup' => '<div class="sirius_form_intro">' . $intro_text . '</div>');

	drupal_add_css(drupal_get_path('module', 'sirius_event') . '/css/survey.css', 'file');

	$form['survey_wrap_open'] = array('#markup' => '<div id="survey_wrap" class="survey_wrap_inpage sirius-form">');

	$form['bsh_worker']['id'] = array(
		'#type' => 'textfield',
		'#title' => t('Local 5 Member ID (OPTIONAL)'),
		'#default_value' => $data['bsh_worker']['id'],
		'#required' => FALSE,
		'#size' => 6,
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

function _sirius_bsh_event_intake_loosematch($str1, $str2) {
	if (!$str1 || !$str2) { return FALSE; }

	return trim(strtoupper($str1)) == trim(strtoupper($str2));
}

function sirius_bsh_event_intake_worker_lookup_form_validate($form, &$form_state) {
	$info = $form_state['info'];
	$data = $form_state['data'];
	$application_node = $form_state['application_node'];
	$event_node = sirius_log_handler($application_node, 'sirius_event');
	$event_json = sirius_json_get($event_node);

	$id = $form_state['values']['bsh_worker']['id'];
	$id = trim($id);

	// If there's no ID, we skip this step.
	if (!$id) { 
		return;
	}

	// ID must be an integer
	if (! ($id > 0)) {
		form_set_error('bsh_worker][id', 'Please enter a valid worker ID.');
		return;
	}

	// Flood control: 250 events in an hour
	if (!sirius_flood_is_allowed('sirius_bsh_worker_lookup')) {
		form_set_error('bsh_worker][id', 'Flood control: too many lookups. Please try again later.');
		return; 
	}

	// Load the workersync plugin
	$plugin = grievance_workersync_plugin_fetch();
	if (!$plugin) {
		form_set_error('bsh_worker][id', 'System error: no worker plugin has been configured.');
		return;
	}
	$callback = $plugin['lookup_callback'];
	$result = $callback($id, TRUE);

	$match = 0;
	if ($result) {
		if (_sirius_bsh_event_intake_loosematch($data['identity']['name_first'], $result['first_name'])) { $match++; }
		if (_sirius_bsh_event_intake_loosematch($data['identity']['name_last'], $result['last_name'])) { $match++; }
		if (_sirius_bsh_event_intake_loosematch($data['addr']['thoroughfare'], $result['street'])) { $match++; }
		if (_sirius_bsh_event_intake_loosematch(sirius_phone_format($data['phone']), sirius_phone_format($result['phone']))) { $match++; }
		if (_sirius_bsh_event_intake_loosematch($data['email'], $result['email'])) { $match++; }
	}

	if (!$match) {
		sirius_flood_register_event('sirius_bsh_worker_lookup');
		form_set_error('bsh_worker][id', 'The data in your application does not match the data in our records. Please make sure to use the same name, phone number, postal address, OR email address as appears in the Local 5 database.');
		return;
	}

	drupal_set_message("Local 5 lookup successful. Your membership has been validated.");
}


function sirius_bsh_event_intake_worker_lookup_form_submit($form, &$form_state) {
	$info = $form_state['info'];
	$data = $form_state['data'];
	$application_node = $form_state['application_node'];
	$event_node = sirius_log_handler($application_node, 'sirius_event');
	$event_json = sirius_json_get($event_node);
	$current_step = $form_state['current_step'];
	$data['bsh_worker'] = $form_state['values']['bsh_worker'];
	sirius_event_intake_set_title($application_node);
	sirius_wizard_submit($info, $data, $application_node, $current_step, "Response saved.", array('nonsequential' => TRUE));
}


/**
 * Implements hook_sirius_flood_info
 */

function sirius_bsh_sirius_flood_info() {
  $info['sirius_bsh_worker_lookup'] = array(
    'title' => 'BSH Worker Lookup',
    'threshold_default' => 15,
    'window_default' => 5*60,
    'description' => 'A user attempted to lookup a worker on the BSH intake screen',
  );

  return $info;
}
