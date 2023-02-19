<?php

function grievance_imis_user_settings_access($uid) {
	// Admins only
	if (!user_access('grievance admin')) { return FALSE; }

	// Must have some brougtys with settings
  $terms = sirius_taxonomy_load('grievance_broughtby', 'tid', 'name');
  $found = FALSE;
  foreach ($terms as $tid => $term_name) {
  	$variable = sirius_domain_variable_name("grievance_workersync_imis_apikey_broughtby_$tid");
  	if (variable_get($variable)) {
  		$found = TRUE;
  		break;
  	}
  }	
  if (!$found) { return FALSE; }

  // Oh goody.
  return TRUE;
}
function grievance_imis_user_settings($uid) {
	return drupal_get_form('grievance_imis_user_settings_form', $uid);
}

function grievance_imis_user_settings_form($form, &$form_state, $settings_uid) {
	$form_state['uid'] = $settings_uid;
	$form['#tree'] = TRUE;

	$settings_user = user_load($settings_uid);
	$settings_json = sirius_json_get($settings_user);
  $terms = sirius_taxonomy_load('grievance_broughtby', 'tid', 'name');
  $form['grievance_imis']['broughtby'] = array(
  	'#title' => t("Grant access to these SePuede servers"),
  	'#options' => $terms,
  	'#type' => 'checkboxes',
  	'#checkall' => TRUE,
  );
  if ($settings_json['grievance_imis']['broughtby']) {
		$form['grievance_imis']['broughtby']['#default_value'] = $settings_json['grievance_imis']['broughtby'];
  }

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function grievance_imis_user_settings_form_submit($form, &$form_state) {
	$settings_user = user_load($form_state['uid']);
	$settings_json = sirius_json_get($settings_user);
	$settings_json['grievance_imis']['broughtby'] = $form_state['values']['grievance_imis']['broughtby'];
	sirius_json_set($settings_user, $settings_json);
	user_save($settings_user);
	drupal_set_message("Settings saved.");
}