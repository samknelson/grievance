<?php

function sirius_node_tab_employer_dispatch_preferences_access($employer_node) {
	return sirius_employer_node_tab_employer_users_access($employer_node);
}

function sirius_node_tab_employer_dispatch_preferences($employer_node) {
	return drupal_get_form('sirius_node_tab_employer_dispatch_preferences_form', $employer_node);
}

function sirius_node_tab_employer_dispatch_preferences_form($form, &$form_state, $employer_node) {
	$form_state['employer_nid'] = $employer_node->nid;
	$json = json_decode($employer_node->field_sirius_json['und'][0]['value'], TRUE);

	$form['notes'] = array(
		'#title' => t('Default Dispatch Notes'),
		'#type' => 'textarea',
		'#default_value' => $json['dispatch']['default']['notes'],
	);

	$form['address_parking'] = array(
		'#title' => t('Default Parking Instructions'),
		'#type' => 'textarea',
		'#default_value' => $json['dispatch']['default']['address_parking'],
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_node_tab_employer_dispatch_preferences_form_submit($form, &$form_state) {
	$employer_node = node_load($form_state['employer_nid']);
	$json = json_decode($employer_node->field_sirius_json['und'][0]['value'], TRUE);
	$json['dispatch']['default']['notes'] = $form_state['values']['notes'];
	$json['dispatch']['default']['address_parking'] = $form_state['values']['address_parking'];
	$employer_node->field_sirius_json['und'][0]['value'] = json_encode($json);
	node_save($employer_node);
	drupal_set_message(t('Settings saved.'));
}
