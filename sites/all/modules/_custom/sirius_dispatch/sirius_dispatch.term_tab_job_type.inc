<?php

function sirius_dispatch_term_tab_job_type($job_type_term) {
	return drupal_get_form('sirius_dispatch_term_tab_job_type_form', $job_type_term);
}

function sirius_dispatch_term_tab_job_type_form($form, &$form_state, $job_type_term) {
	$form_state['job_type_tid'] = $job_type_term->tid;
	$json = json_decode($job_type_term->field_sirius_json['und'][0]['value'], TRUE);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}



function sirius_dispatch_term_tab_job_type_form_submit($form, &$form_state) {
	$job_type_tid = $form_state['job_type_tid'];
	$job_type_term = taxonomy_term_load($job_type_tid);
	$json = sirius_jsonfield_util_term_json_get($job_type_term);
	$json['dispatch']['default']['notes'] = $form_state['values']['notes'];
	$json['dispatch']['default']['address_parking'] = $form_state['values']['address_parking'];
	sirius_jsonfield_util_term_json_get($job_type_term, $json);
	taxonomy_term_save($job_type_term);
	drupal_set_message(t('Settings saved.'));
}

