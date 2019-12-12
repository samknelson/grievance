<?php

function sirius_worker_term_tab_member_status_optin($member_status_term) {
	return drupal_get_form('sirius_worker_term_tab_member_status_optin_form', $member_status_term);
}

function sirius_worker_term_tab_member_status_optin_form($form, &$form_state, $member_status_term) {
	$form_state['member_status_tid'] = $member_status_term->tid;
	$json = json_decode($member_status_term->field_sirius_json['und'][0]['value'], TRUE);

	$form['optin'] = array(
		'#type' => 'select',
		'#title' => t('Can workers opt-in to the @name member status without approval?', array('@name' => $member_status_term->name)),
		'#options' => array(
			'No' => t('No'),
			'Yes' => t('Yes'),
		),
		'#default_value' => $json['optin']['optin'],
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_worker_term_tab_member_status_optin_form_submit($form, &$form_state) {
	$member_status_tid = $form_state['member_status_tid'];
	$member_status_term = taxonomy_term_load($member_status_tid);
	$json = json_decode($member_status_term->field_sirius_json['und'][0]['value'], TRUE);
	$json['optin'] = $form_state['values'];
	$member_status_term->field_sirius_json['und'][0]['value'] = json_encode($json);
	taxonomy_term_save($member_status_term);
	drupal_set_message("Settings saved.");
}

