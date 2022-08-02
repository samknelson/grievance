<?php

function sirius_dispatch_page_jwa_jobs() {
	return drupal_get_form('sirius_dispatch_page_jwa_jobs_form');
}

function sirius_dispatch_page_jwa_jobs_form($form, &$form_state) {
	$form['#tree'] = TRUE; 
	global $user;
	$user_full = user_load($user->uid);
	$json = sirius_json_get($user_full);
	$defaults = $json['sirius']['dispatch']['jwa']['prefs'];

	$job_type_options = sirius_taxonomy_load('sirius_dispatch_job_type', 'tid', 'name');
	$form['jobs']['job_type'] = array(
		'#type' => 'checkboxes',
		'#title' => t('Job Type'),
		'#options' => $job_type_options,
		'#default_value' => sirius_form_array_default_value($defaults['jobs']['job_type']),
		'#checkall' => TRUE,
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Save')
	);

	$form['submit_assign'] = array(
		'#type' => 'submit',
		'#value' => t('Save and Assign Workers'),
		'#name' => 'submit_assign',
		'#prefix' => '&nbsp;',
	);

	return $form;
}

function sirius_dispatch_page_jwa_jobs_form_submit($form, &$form_state) {
	global $user;
	$user_full = user_load($user->uid);
	$json = sirius_json_get($user_full);
	$json['sirius']['dispatch']['jwa']['prefs']['jobs'] = $form_state['values']['jobs'];
	sirius_json_set($user_full, $json);
	user_save($user_full);
	drupal_set_message("Preferences saved.");
	if ($form_state['clicked_button']['#name'] == 'submit_assign') { drupal_goto('sirius/dispatch/jwa'); }
}