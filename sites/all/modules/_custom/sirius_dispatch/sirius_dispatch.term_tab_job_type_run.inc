<?php

function sirius_dispatch_term_tab_job_type_run_form($form, &$form_state, $job_type_term) {
	$form_state['job_type_tid'] = $job_type_term->tid;

	$form['#tree'] = TRUE;

	$roles = user_roles();
	$form['run']['role'] = array(
		'#type' => 'checkboxes', 
		'#title' => 'Roles',
		'#options' => $roles,
		'#description' => t('Check the roles with permission to run this job type.')
	);

	$form['run']['fan'] = array(
		'#type' => 'select', 
		'#title' => 'FAN?',
		'#options' => array('No' => 'No', 'Yes' => 'Yes'),
		'#description' => t('Fill as Needed - If Yes, then once all call-by-name dispatches have been sent, additional dispatches will be created from all available workers until the job is filled.')
	);

	$form['run']['timelimit'] = array(
		'#title' => 'Default Expire After',
		'#type' => 'textfield',
		'#size' => 5,
		'#default_value' => 30,
		'#description' => t('Enter the number of minutes before a dispatch notification should expire.'),
	);

	$form['run']['batch_max'] = array(
		'#title' => 'Default Maximum Batch Size',
		'#type' => 'textfield',
		'#size' => 5,
		'#default_value' => 10,
		'#description' => t('Enter the maxminum number ofnotifications that will be sent in any given batch'),
	);

	$form['run']['batch_min'] = array(
		'#title' => 'Default Minimum Batch Size',
		'#type' => 'textfield',
		'#size' => 5,
		'#default_value' => 1,
		'#description' => t('Enter the minimum numer of notifications that will be sent in any given batch. If this is greater than 1, then it is possible that more notifications will be sent than the number of open slots, meaning that there is no guarantee that the job will be filled in seniority order.'),
	);

	$form['run']['batch_ratio'] = array(
		'#title' => 'Default Ratio of Notifications to Slots',
		'#type' => 'textfield',
		'#size' => 5,
		'#default_value' => 1,
		'#description' => t('Enter the number of notifications to be sent for each open slot. If this is greater than 1, then there will be more than one notification sent for each slot, meaning that there is no guarantee that the job will be filled in seniority order.'),
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

 	sirius_jsonfield_util_set_defaults($form, sirius_json_get($job_type_term));

	return $form;
}



function sirius_dispatch_term_tab_job_type_run_form_submit($form, &$form_state) {
	$job_type_tid = $form_state['job_type_tid'];
	$job_type_term = taxonomy_term_load($job_type_tid);
	$json = sirius_json_get($job_type_term);
	$json['run'] = $form_state['values']['run'];
	sirius_json_set($job_type_term, $json);
	taxonomy_term_save($job_type_term);
	drupal_set_message(t('Settings saved.'));
}

