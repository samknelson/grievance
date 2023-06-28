<?php

function sirius_dispatch_term_tab_job_type_form_form($form, &$form_state, $job_type_term) {
	$form_state['job_type_tid'] = $job_type_term->tid;

	$form['#tree'] = TRUE;

	$form['form']['hide_skill'] = array(
		'#type' => 'select', 
		'#title' => 'Hide Skill?',
		'#options' => array('No' => 'No', 'Yes' => 'Yes'),
		'#description' => t('If "Yes", then the "Skill" field will be hidden on the "create a job" form.')
	);

	$form['form']['hide_department'] = array(
		'#type' => 'select', 
		'#title' => 'Hide Department?',
		'#options' => array('No' => 'No', 'Yes' => 'Yes'),
		'#description' => t('If "Yes", then the "Department" field will be hidden on the "create a job" form.')
	);

	$form['form']['hide_address_notes'] = array(
		'#type' => 'select', 
		'#title' => 'Hide Report Location?',
		'#options' => array('No' => 'No', 'Yes' => 'Yes'),
		'#description' => t('If "Yes", then the "Report Location" field will be hidden on the "create a job" form.')
	);

	$form['form']['hide_address_parking'] = array(
		'#type' => 'select', 
		'#title' => 'Hide Parking Instructions?',
		'#options' => array('No' => 'No', 'Yes' => 'Yes'),
		'#description' => t('If "Yes", then the "Parking Instructions" field will be hidden on the "create a job" form.')
	);

	$form['form']['shift_single'] = array(
		'#type' => 'select', 
		'#title' => 'Single Shift',
		'#options' => array('No' => 'No', 'Yes' => 'Yes'),
		'#description' => t('Is this a single-shift dispatch? If "Yes", then an end time will be required, and the dispatch will automatically terminated after the shift is complete.'),
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

 	sirius_jsonfield_util_set_defaults($form, sirius_json_get($job_type_term));

	return $form;
}



function sirius_dispatch_term_tab_job_type_form_form_submit($form, &$form_state) {
	$job_type_tid = $form_state['job_type_tid'];
	$job_type_term = taxonomy_term_load($job_type_tid);
	$json = sirius_json_get($job_type_term);
	$json['form'] = $form_state['values']['form'];
	sirius_json_set($job_type_term, $json);
	taxonomy_term_save($job_type_term);
	drupal_set_message(t('Settings saved.'));
}

