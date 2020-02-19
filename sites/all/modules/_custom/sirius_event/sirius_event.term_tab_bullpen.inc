<?php

function sirius_event_term_tab_bullpen($event_type_term) {
	return drupal_get_form('sirius_event_term_tab_bullpen_form', $event_type_term);
}

function sirius_event_term_tab_bullpen_form($form, &$form_state, $event_type_term) {
	$form_state['event_type_tid'] = $event_type_term->tid;
	$json = json_decode($event_type_term->field_sirius_json['und'][0]['value'], TRUE);
	$paccess_options = sirius_event_paccess_options($event_type_term);
	$register_as_options = sirius_event_paccess_options($event_type_term, array('exclude_non_participants' => TRUE));
	$json = json_decode($event_type_term->field_sirius_json['und'][0]['value'], TRUE);

	$form['#tree'] = TRUE;

	$form['enabled'] = array(
		'#type' => 'select',
		'#title' => t('Bullpen Enabled?'),
		'#options' => array('No' => t('No'), 'Yes' => t('Yes')),
		'#description' => t('Is this event type a bullpen?'),
		'#default_value' => $json['bullpen']['enabled'],
	);

	$form['manage']['rules'][0]['paccess'] = array(
		'#type' => 'select',
		'#title' => t('Bullpen can be managed by these users'),
		'#options' => $paccess_options,
		'#multiple' => TRUE,
	);
	if ($json['bullpen']['manage']['rules'][0]['paccess']) { $form['manage']['rules'][0]['paccess']['#default_value'] = $json['bullpen']['manage']['rules'][0]['paccess']; }

	$form["pre_dispatch"] = array(
		'#title' => t('To be dispatchable, a worker must have this participant role and status'),
		'#type' => 'select',
		'#options' => $register_as_options,
		'#default_value' => $json['bullpen']['pre_dispatch'],
	);

	$form["post_dispatch"] = array(
		'#title' => t('After being dispatched, a worker should be set to this participant role and status'),
		'#type' => 'select',
		'#options' => $register_as_options,
		'#default_value' => $json['bullpen']['post_dispatch'],
	);

	$form["checkout"] = array(
		'#title' => t('A worker who checks out of a bullpen should be set to this participant role and status'),
		'#type' => 'select',
		'#options' => $register_as_options,
		'#default_value' => $json['bullpen']['checkout'],
	);

	$form["signin_opens_self"] = array(
		'#title' => t('Self Sign-in Period'),
		'#type' => 'textfield',
		'#default_value' => $json['bullpen']['signin_opens_self'],
		'#description' => t('Enter the period before the event start date when a worker can begin signing themselves in. This should be set as a nubmer of seconds; for example, if a worker can self sign-in five days before the event, enter 60*60*24*5 = <strong>432000</strong>.'),
	);

	$form["signin_opens_others"] = array(
		'#title' => t('Steward Sign-in Period'),
		'#type' => 'textfield',
		'#default_value' => $json['bullpen']['signin_opens_others'],
		'#description' => t('Enter the period before the event start date when a steward can begin signing in workers. This should be set as a nubmer of seconds; for example, if a steward can sign-in a worker one hour before the event, enter 60*60 = <strong>3600</strong>.'),
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_event_term_tab_bullpen_form_submit($form, &$form_state) {
	$event_type_tid = $form_state['event_type_tid'];
	$event_type_term = taxonomy_term_load($event_type_tid);
	$json = json_decode($event_type_term->field_sirius_json['und'][0]['value'], TRUE);
	$json['bullpen'] = $form_state['values'];
	$event_type_term->field_sirius_json['und'][0]['value'] = json_encode($json);
	taxonomy_term_save($event_type_term);
	drupal_set_message("Settings saved.");
}

