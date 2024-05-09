<?php

function sirius_node_tab_employer_dispatch_facilities_form($form, &$form_state, $employer_node) {
	$form_state['employer_nid'] = $employer_node->nid;

	$employer_json = sirius_json_get($employer_node);

	$all_facilities = sirius_dispatch_facility_dropdown(array('include_null' => FALSE));

	$form['#tree'] = TRUE;

	$form['dispatch']['facilities']['restrict'] = array(
		'#type' => 'select',
		'#title' => t('Restrict the facilities for this employer'),
		'#options' => array(
			'No' => t('No'),
			'Yes' => t('Yes'),
		),
		'#default_value' => $employer_json['dispatch']['facilities']['restrict'],
	);

	$form['dispatch']['facilities']['list'] = array(
		'#type' => 'checkboxes',
		'#title' => t('Available Facilities'),
		'#options' => $all_facilities,
		'#default_value' => sirius_form_array_default_value($employer_json['dispatch']['facilities']['list']),
		'#states' => array('visible' => array(array(":input[name=\"dispatch[facilities][restrict]\"]" => array('value' => 'Yes')))),
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_node_tab_employer_dispatch_facilities_form_submit($form, &$form_state) {
	$employer_node = node_load($form_state['employer_nid']);
	$employer_json = sirius_json_get($employer_node);
	$employer_json['dispatch']['facilities'] = $form_state['values']['dispatch']['facilities'];
	sirius_json_set($employer_node, $employer_json);
	node_save($employer_node);
	drupal_set_message("Settings saved.");
}