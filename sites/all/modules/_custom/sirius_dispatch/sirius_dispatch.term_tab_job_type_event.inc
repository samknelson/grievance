<?php

function sirius_dispatch_term_tab_job_type_event_form($form, &$form_state, $job_type_term) {
	$form_state['job_type_tid'] = $job_type_term->tid;

	$form['#tree'] = TRUE;

	$form['event']['has_event'] = array(
		'#type' => 'select', 
		'#title' => 'Has Event?',
		'#options' => array('No' => 'No', 'Yes' => 'Yes'),
		'#description' => t('If "Yes", then each job of this type should be associated with an event. The worker list will be drawn from event registrants.'),
	);

	$event_type_options = sirius_taxonomy_load('sirius_event_type', 'tid', 'name', TRUE);
	$form['event']['event_type'] = array(
		'#type' => 'select',
		'#title' => 'Event Type',
		'#options' => $event_type_options,
	);

	$form['event']['on_job_create'] = array(
		'#type' => 'select',
		'#title' => 'On Job Creation',
		'#options' => array(
			NULL => t('-- Select --'),
			'create' => t('Create a new event'),
			'select' => t('Select an existing event'),
		),
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

 	sirius_jsonfield_util_set_defaults($form, sirius_json_get($job_type_term));

	return $form;
}



function sirius_dispatch_term_tab_job_type_event_form_submit($form, &$form_state) {
	$job_type_tid = $form_state['job_type_tid'];
	$job_type_term = taxonomy_term_load($job_type_tid);
	$json = sirius_json_get($job_type_term);
	$json['event'] = $form_state['values']['event'];
	sirius_json_set($job_type_term, $json);
	taxonomy_term_save($job_type_term);
	drupal_set_message(t('Settings saved.'));
}

