<?php

function sirius_user_tab_proxy_access($proxy_uid) {
	// Must be a user admin
	if (!user_access('administer users')) { return FALSE; }

	// Proxy must be active
	if (!sirius_proxy_active()) { return FALSE; }

	return TRUE;
}

function sirius_user_tab_proxy($proxy_uid) {
	return drupal_get_form('sirius_user_tab_proxy_form', user_load($proxy_uid));
}

function sirius_user_tab_proxy_form($form, &$form_state, $proxy_user) {
	$form_state['proxy_uid'] = $proxy_user->uid;

	$proxy_user_json = sirius_json_get($proxy_user);
	$json = $proxy_user_json['proxy'];

	$form['#tree'] = TRUE;

	$result = sirius_proxy_call('sirius_employer_list', array());
	if (!$result['success']) { return sirius_form_error($result['msg']); }
	$employer_options = $result['data'];

	$form['proxy']['employer_rnid'] = array(
		'#type' => 'select',
		'#title' => t('Employer'),
		'#options' => array(NULL => t('-- Select --')) + $employer_options,
		'#default_value' => $json['employer_rnid'],
	);

	$form['proxy']['worker_ssn'] = array(
		'#type' => 'select',
		'#title' => t('Worker SSN'),
		'#type' => 'textfield',
		'#default_value' => $json['worker_ssn'],
		'#size' => 10,
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_user_tab_proxy_form_validate($form, &$form_state) {
	$worker_ssn = $form_state['values']['proxy']['worker_ssn'];
	if ($worker_ssn) {
		$result = sirius_proxy_call('sirius_worker_get_by_ssn', $worker_ssn);
		if (!$result['success']) {
			form_set_error('proxy][worker_ssn', $result['msg']);
		} else if (!$result['data']) {
			form_set_error('proxy][worker_ssn', "Worker not found.");
		}
	}
}

function sirius_user_tab_proxy_form_submit($form, &$form_state) {
	$proxy_user = user_load($form_state['proxy_uid']);
	$proxy_user_json = sirius_json_get($proxy_user);
	$proxy_user_json['proxy'] = $form_state['values']['proxy'];
	sirius_json_set($proxy_user, $proxy_user_json);
	user_save($proxy_user);
}