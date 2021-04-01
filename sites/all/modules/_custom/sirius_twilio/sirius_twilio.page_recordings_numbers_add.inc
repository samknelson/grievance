<?php

function sirius_twilio_page_recordings_numbers_add() {
	return drupal_get_form('sirius_twilio_page_recordings_numbers_add_form');
}

function sirius_twilio_page_recordings_numbers_add_form($form, &$form_state) {
	$form['phone'] = array(
		'#type' => 'textfield',
		'#title' => t('Phone Number'),
		'#required' => TRUE,
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Add to Recording List'),
	);

	return $form;
}
function sirius_twilio_page_recordings_numbers_add_form_validate($form, &$form_state) {
	$phone = $form_state['values']['phone'];
	if (!sirius_phone_validate($phone)) {
		form_set_error('phone', 'Please enter a valid phone number.');
	}
}

function sirius_twilio_page_recordings_numbers_add_form_submit($form, &$form_state) {
	$phone = sirius_phone_format($form_state['values']['phone']);
	$phone_node = sirius_phonenumber_lookup($phone);
	$json = sirius_jsonfield_util_node_json_get($phone_node);
	$json['record']['enabled'] = 'Yes';
	sirius_jsonfield_util_node_json_set($phone_node, $json);
	node_save($phone_node);
	drupal_set_message("Here with " . $phone_node->nid);
	drupal_set_message("All recordings for phone number $phone will be archived locally.");
}