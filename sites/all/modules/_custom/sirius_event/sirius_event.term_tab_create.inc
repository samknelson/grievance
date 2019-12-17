<?php

function sirius_event_term_tab_policy($event_type_term) {
	drupal_goto("taxonomy/term/" . $event_type_term->tid . "/sirius_event_type_policy/create");
}

function sirius_event_term_tab_policy_create($event_type_term) {
	return drupal_get_form('sirius_event_term_tab_policy_create_form', $event_type_term);
}

function sirius_event_term_tab_policy_create_form($form, &$form_state, $event_type_term) {
	$form_state['event_type_tid'] = $event_type_term->tid;
	$json = json_decode($event_type_term->field_sirius_json['und'][0]['value'], TRUE);
	$paccess_options = sirius_event_paccess_options($event_type_term, array('exclude_participants' => TRUE));
	$paccess_options_register = sirius_event_paccess_options($event_type_term, array('exclude_non_participants' => TRUE, 'null' => t('None, the event creator should not be registered.')));

	if ($json['create']['rules']) {
		$max = count($json['create']['rules']) + 2;
	} else {
		$max = 2;
	}

	$form['#tree'] = TRUE;

	for ($i=0; $i<$max; ++$i) {
		$form["create_$i"] = array(
			'#type' => 'fieldset',
			'#title' => t('Creation rule #@i', array('@i' => $i+1)),
		);

		$form["create_$i"]['paccess'] = array(
			'#type' => 'select',
			'#title' => t('Events may be created by these users.'),
			'#options' => $paccess_options,
			'#default_value' => $json['create']['rules'][$i]['paccess'],
			'#multiple' => TRUE,
		);

		$form["create_$i"]['register_as'] = array(
			'#type' => 'select',
			'#title' => t('When a new event is created, the user should be registered with this participant role and participant status'),
			'#options' => $paccess_options_register,
			'#default_value' => $json['create']['rules'][$i]['register_as'],
		);
	}

	$form["create"]['notify']['email'] = array(
		'#type' => 'textarea',
		'#title' => 'Email Notifications',
		'#description' => t('When a new event is created, send an email notification to these addresses (one per line)'),
	);
	if ($json['create']['notify']['email']) {
		$form["create"]['notify']['email']['#default_value'] = join("\n", $json['create']['notify']['email']);
	}

	$form["create"]['notify']['sms'] = array(
		'#type' => 'textarea',
		'#title' => 'SMS Notifications',
		'#description' => t('When a new event is created, send an SMS notification to these phone numbers (one per line)'),
	);
	if ($json['create']['notify']['sms']) {
		$form["create"]['notify']['sms']['#default_value'] = join("\n", $json['create']['notify']['sms']);
	}

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_event_term_tab_policy_create_form_submit($form, &$form_state) {
	$event_type_tid = $form_state['event_type_tid'];
	$event_type_term = taxonomy_term_load($event_type_tid);

	$json = json_decode($event_type_term->field_sirius_json['und'][0]['value'], TRUE);
	$i = 0;
	$result = array();
	while ($form_state['values']["create_$i"]['paccess']) {
		$result[] = $form_state['values']["create_$i"];
		++$i;
	}

	$notify_sms = preg_split('/[\n\r]+/', $form_state['values']['create']['notify']['sms']);
	$json['create']['notify']['sms'] = array();
	foreach ($notify_sms as $phone) {
		$phone = sirius_phone_format($phone);
		if (!sirius_phone_validate($phone)) {
			drupal_set_message("Doesn't look like a valid phone number: $phone", 'warning');
			continue;
		}
		$json['create']['notify']['sms'][] = $phone;
	}

	$notify_email = preg_split('/[\n\r]+/', $form_state['values']['create']['notify']['email']);
	$json['create']['notify']['email'] = array();
	foreach ($notify_email as $email) {
		$email = sirius_email_format($email);
		if (!sirius_email_validate($email)) {
			drupal_set_message("Doesn't look like a valid email address: $email", 'warning');
			continue;
		}
		$json['create']['notify']['email'][] = $email;
	}

	$json['create']['rules'] = $result;
	$event_type_term->field_sirius_json['und'][0]['value'] = json_encode($json);
	taxonomy_term_save($event_type_term);
	drupal_set_message("Settings saved.");
}

