<?php

function sirius_worker_term_tab_work_status_settings($work_status_term) {
	return drupal_get_form('sirius_worker_term_tab_work_status_settings_form', $work_status_term);
}

function sirius_worker_term_tab_work_status_settings_form($form, &$form_state, $work_status_term) {
	$form_state['work_status_tid'] = $work_status_term->tid;
	$work_status_json = sirius_json_get($work_status_term);

	$form['#tree'] = TRUE;

	$form['feed']['locked'] = array(
		'#title' => t('Feed-Locked?'),
		'#type' => 'select',
		'#options' => array(
			'No' => t('No'),
			'Yes' => t('Yes'),
		),
		'#description' => t('Is this term "locked" against updates by feeds? Terms that are feed-locked cannot be changed via bulk feed import; they need to be updated manually.'),
		'#default_value' => $work_status_json['feed']['locked'],
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);
	return $form;
}

function sirius_worker_term_tab_work_status_settings_form_submit($form, &$form_state) {
	$work_status_term = taxonomy_term_load($form_state['work_status_tid']);
	$work_status_json = sirius_json_get($work_status_term);
	$work_status_json['feed'] = $form_state['values']['feed'];
	sirius_json_set($work_status_term, $work_status_json);
	taxonomy_term_save($work_status_term);
	drupal_set_message("Settings saved.");
}