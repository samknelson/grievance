<?php

function sirius_worker_term_tab_member_status_tos($member_status_term) {
	return drupal_get_form('sirius_worker_term_tab_member_status_tos_form', $member_status_term);
}

function sirius_worker_term_tab_member_status_tos_form($form, &$form_state, $member_status_term) {
	$form_state['member_status_tid'] = $member_status_term->tid;
	$json = sirius_jsonfield_util_term_json_get($member_status_term);

	$form['#tree'] = TRUE;

	$form['tos']['worker_can_submit'] = array(
		'#type' => 'select',
		'#title' => t('Can workers with the member status @name call in sick?', array('@name' => $member_status_term->name)),
		'#options' => array(
			'No' => t('No'),
			'Yes' => t('Yes'),
		),
		'#required' => TRUE,
	);
	if ($json['tos']['worker_can_submit']) { $form['tos']['worker_can_submit']['#default_value'] = $json['tos']['worker_can_submit']; }

	$form['tos']['notify'] = array(
		'#type' => 'fieldset',
		'#title' => t('Notify'),
		'#description' => t('The employer will be notified of time-off sick events. Also send notifications to these email addresses.'),
	);

	for ($i=0; $i<5; ++$i) {
		$form['tos']['notify'][$i] = array(
			'#type' => 'textfield',
			'#title' => t('Email #@i', array('@i' => ($i+1))),
		);
		if ($json['tos']['notify'][$i]) { $form['tos']['notify'][$i]['#default_value'] = $json['tos']['notify'][$i]; }
	}

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_worker_term_tab_member_status_tos_form_submit($form, &$form_state) {
	$member_status_tid = $form_state['member_status_tid'];
	$member_status_term = taxonomy_term_load($member_status_tid);
	$json = sirius_jsonfield_util_term_json_get($member_status_term);
	$json['tos'] = $form_state['values']['tos'];
	sirius_jsonfield_util_term_json_set($member_status_term, $json);
	taxonomy_term_save($member_status_term);
	drupal_set_message("Settings saved.");
}

