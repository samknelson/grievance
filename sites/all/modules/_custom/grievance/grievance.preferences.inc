<?php

function grievance_page_preferences_access($uid) {
	// Need "grievance user"
	if (!user_access("grievance user")) { return FALSE; }

	// Need to be able to see our own account edit page.
	if (!drupal_valid_path("user/$uid/edit")) { return FALSE; }

	// For the moment, "preferences" are only for alerts. So we only allow access to this page if alerts are enabled on the site.
	if (sirius_domain_variable_get('grievance_alert_disable', NULL) == 'Yes') { return FALSE; }

	// Need the "grievances" component
	if (!sirius_domain_component_enabled('grievance')) { return FALSE; }

	return TRUE;
}

function grievance_page_preferences($uid) {
	return drupal_get_form('grievance_page_preferences_form', $uid);
}

function grievance_page_preferences_form($form, &$form_state, $uid) {
	$form_state['uid'] = $uid;
	$user_full = user_load($uid);

	$json = json_decode($user_full->field_sirius_json['und'][0]['value'], TRUE);
	$defaults = $json['sirius']['grievance']['alert']['prefs'];

	$form['#tree'] = TRUE;
	$form['sirius']['grievance']['alert']['prefs']['medium'] = array(
		'#type' => 'select',
		'#title' => t('Alert Preferences'),
		'#options' => array(
			NULL => t('-- Select --'),
			'email' => t('Email'),
			'sms' => t('SMS (Text Message)'),
			'both' => t('Both Email and SMS'),
			'none' => t('No Alerts'),
		),
		'#description' => t('When a deadline is approaching for one of your grievances, how would you like to receive reminders?'),
		'#default_value' => $json['sirius']['grievance']['alert']['prefs']['medium'],
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit')
	);

	return $form;
}

function grievance_page_preferences_form_submit($form, &$form_state) {
	$uid = $form_state['uid'];
	$user_full = user_load($uid);
	$json = json_decode($user_full->field_sirius_json['und'][0]['value'], TRUE);
	$json['sirius']['grievance']['alert']['prefs'] = $form_state['values']['sirius']['grievance']['alert']['prefs'];
	$user_full->field_sirius_json['und'][0]['value'] = json_encode($json);
	user_save($user_full);
	drupal_set_message("Preferences saved.");
}