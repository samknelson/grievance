<?php

function sirius_worker_term_tab_member_status_subscribe($member_status_term) {
	return drupal_get_form('sirius_worker_term_tab_member_status_subscribe_form', $member_status_term);
}

function sirius_worker_term_tab_member_status_subscribe_form($form, &$form_state, $member_status_term) {
	$form_state['member_status_tid'] = $member_status_term->tid;
	$json = sirius_jsonfield_util_term_json_get($member_status_term);

	$form['#tree'] = TRUE;

	$max = 2;
	if ($json['subscribe']['notify']) { $max = count($json['subscribe']['notify']) + 2; }

	for ($i=0; $i<$max; ++$i) {
		$form['subscribe']['notify'][$i] = array(
			'#type' => 'textfield',
			'#title' => t('Email #@i', array('@i' => ($i+1))),
		);
		if ($json['subscribe']['notify'][$i]) { $form['subscribe']['notify'][$i]['#default_value'] = $json['subscribe']['notify'][$i]; }
	}

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_worker_term_tab_member_status_subscribe_form_validate($form, &$form_state) {
	foreach ($form_state['values']['subscribe']['notify'] as $delta => $email) {
		if (!$email) { continue; }
		if (!sirius_email_validate($email)) {
			form_set_error("subscribe][notify][$delta", t('Please enter a valid email address.'));
		}
	}
}

function sirius_worker_term_tab_member_status_subscribe_form_submit($form, &$form_state) {
	$member_status_tid = $form_state['member_status_tid'];
	$member_status_term = taxonomy_term_load($member_status_tid);
	$json = sirius_jsonfield_util_term_json_get($member_status_term);

	$json['subscribe'] = $form_state['values']['subscribe'];
	$json['subscribe']['notify'] = array_unique(array_values(array_filter($json['subscribe']['notify'])));

	sirius_jsonfield_util_term_json_set($member_status_term, $json);
	taxonomy_term_save($member_status_term);
	drupal_set_message("Settings saved.");
}

