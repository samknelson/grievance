<?php

function sirius_event_term_tab_pstatus($pstatus_term) {
	return drupal_get_form('sirius_event_term_tab_pstatus_form', $pstatus_term);
}

function sirius_event_term_tab_pstatus_form($form, &$form_state, $pstatus_term) {
	$form_state['pstatus_tid'] = $pstatus_term->tid;
	$json = sirius_jsonfield_util_term_json_get($pstatus_term);
	$form['#tree'] = TRUE;

	$form['notification']['register'] = array(
		'#type' => 'fieldset',
		'#title' => t('Registration Notification'),
		'#description' => t('Indicate the default notification that should be sent to a participant when they are registered for this event.', array('@name' => $pstatus_term->name)),
		'#collapsible' => TRUE,
		'#collapsed' => TRUE,
	);

	$medium_options = array(
    NULL => t('No notification'),
    'email' => t('Notify by Email'),
    'sms' => t('Notify by SMS'),
    'both' => t('Notify by both Email and SMS'),
	);

	$form['notification']['register']['medium'] = array(
		'#title' => t('Default Medium'),
		'#type' => 'select',
		'#options' => $medium_options,
		'#default_value' => $json['notification']['register']['medium'],
	);

	$form['notification']['register']['email_subject'] = array(
		'#type' => 'textfield',
		'#title' => t('Email Subject'),
		'#default_value' => $json['notification']['register']['email_subject'],
	);

	$form['notification']['register']['email_body'] = array(
		'#type' => 'text_format',
		'#title' => 'Email Body',
		'#default_value' => $json['notification']['register']['email_body'],
		'#format' => 'sirius_form_editor',
		'#allowed_formats' => array('sirius_form_editor'),
	);

	$form['notification']['register']['sms'] = array(
		'#type' => 'textarea',
		'#title' => t('SMS'),
		'#default_value' => $json['notification']['register']['sms'],
	);

	$form['mypstatus'] = array(
		'#type' => 'fieldset',
		'#title' => t('Self-Update Options'),
		'#description' => t('If a participant has the status <strong>@name</strong>, indicate which statuses they can opt to put themselves into. For example, if the event is a class, a participant with status "enrolled" might be able to transition to the status "withdrawn", but would not be able to transition themselves to the status "passed".', array('@name' => $pstatus_term->name)),
		'#collapsible' => TRUE,
		'#collapsed' => TRUE,
	);

	$prole_tids = sirius_fieldvals($pstatus_term, 'field_sirius_event_proles', 'tid');
	$pstatus_options = sirius_event_participant_role_status_tids($prole_tids);
	if ($pstatus_options) {
		$form['mypstatus']['options'] = array(
			'#title' => t('Available Self-Update Options'),
			'#type' => 'checkboxes',
			'#options' => $pstatus_options,
		);
		if (isset($json['mypstatus']['options'])) { $form['mypstatus']['options']['#default_value'] = $json['mypstatus']['options']; }
	}

	$form['schedule']['display'] = array(
		'#type' => 'fieldset',
		'#title' => t('Schedule Display'),
		'#description' => t('The settings below indicate how participants with this status should be rendered when their schedule is displayed. For instance, if a worker has status "registered" for a class, you might want to show them on the schedule as "In Class"; but if their participant status is "canceled", you might prefer for that event not to appear on the schedule.'),
		'#collapsible' => TRUE,
		'#collapsed' => TRUE,
	);

	$form['schedule']['display']['active'] = array(
		'#title' => t('Display on the contact\'s schedule page?'),
		'#type' => 'select',
		'#options' => array(
			NULL => t('-- Select --'),
			'Yes' => t('Yes'),
			'No' => t('No'),
		),
		'#id' => 'schedule_display_active',
	);
	if (isset($json['schedule']['display']['active'])) { $form['schedule']['display']['active']['#default_value'] = $json['schedule']['display']['active']; }

	$form['schedule']['display']['title'] = array(
		'#title' => t('Display As (Full)'),
		'#type' => 'textfield',
 		'#states' => array('visible' => array(":input[id=\"schedule_display_active\"]" => array('value' => 'Yes'))),
	);
	if (isset($json['schedule']['display']['title'])) { $form['schedule']['display']['title']['#default_value'] = $json['schedule']['display']['title']; } 
	else { $form['schedule']['display']['title']['#default_value'] = $pstatus_term->name; }

	$form['schedule']['display']['title_short'] = array(
		'#title' => t('Display As (Short)'),
		'#type' => 'textfield',
		'#size' => 4,
 		'#states' => array('visible' => array(":input[id=\"schedule_display_active\"]" => array('value' => 'Yes'))),
	);
	if (isset($json['schedule']['display']['title_short'])) { $form['schedule']['display']['title_short']['#default_value'] = $json['schedule']['display']['title_short']; } 
	else { $form['schedule']['display']['title_short']['#default_value'] = substr($pstatus_term->name, 0, 4); }

	$form['phonebank'] = array(
		'#type' => 'fieldset',
		'#title' => t('Phonebank Display'),
		'#description' => t('The settings below indicate how this participant status should be handled in phonebanks.'),
		'#collapsible' => TRUE,
		'#collapsed' => TRUE,
	);

	$form['phonebank']['conceal_during_call'] = array(
		'#title' => t('Conceal During Call?'),
		'#type' => 'select',
		'#options' => array(
			NULL => t('-- Select --'),
			'Yes' => t('Yes'),
			'No' => t('No'),
		),
		'#description' => t('Select "Yes" to conceal this status from phone banker users who are making a call.'),
	);
	if (isset($json['phonebank']['conceal_during_call'])) { $form['phonebank']['conceal_during_call']['#default_value'] = $json['phonebank']['conceal_during_call']; }

	$form['map'] = array(
		'#type' => 'fieldset',
		'#title' => t('Map Display'),
		'#description' => t('The settings below indicate how this participant status should be displayed on a map.'),
		'#collapsible' => TRUE,
		'#collapsed' => TRUE,
	);

	/*
	$form['map']['icon'] = array(
		'#title' => t('Icon'),
		'#type' => 'select',
		'#options' => array(NULL => t('-- Select --')) + sirius_map_icons(),
		'#description' => t('Select the marker to use to render participants with this status.'),
	);
	if (isset($json['map']['icon'])) { $form['map']['icon']['#default_value'] = $json['map']['icon']; }
	*/

	$form['map']['pin'] = sirius_map_pin_picker($json['map']['pin']);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => 'Submit',
	);

	return $form;
}

function sirius_event_term_tab_pstatus_form_submit($form, &$form_state) {
	$pstatus_term = taxonomy_term_load($form_state['pstatus_tid']);
	$json = sirius_jsonfield_util_term_json_get($pstatus_term);
	$json['schedule'] = $form_state['values']['schedule'];
	$json['phonebank'] = $form_state['values']['phonebank'];
	$json['map'] = $form_state['values']['map'];
	$json['mypstatus'] = $form_state['values']['mypstatus'];
	$json['notification'] = $form_state['values']['notification'];
	$json['notification']['register']['email_body'] = sirius_richtext_handle_input($form_state['values']['notification']['register']['email_body']);
	sirius_jsonfield_util_term_json_set($pstatus_term, $json);
	taxonomy_term_save($pstatus_term);

	drupal_set_message("Settings saved.");
}