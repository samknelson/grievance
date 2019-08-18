<?php

function sirius_node_tab_employer_dispatch_preferences_access($employer_node) {
	return node_access('view', $employer_node);
}

function sirius_node_tab_employer_dispatch_preferences($employer_node) {
	return drupal_get_form('sirius_node_tab_employer_dispatch_preferences_form', $employer_node);
}

function sirius_node_tab_employer_dispatch_preferences_form($form, &$form_state, $employer_node) {
	$form_state['employer_node'] = $employer_node;

	$form['sirius_dispatch_notesdef'] = array(
		'#title' => 'Default Dispatch Notes',
		'#type' => 'textarea',
		'#default_value' => $employer_node->field_sirius_dispatch_notesdef['und'][0]['value'],
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => 'Submit',
	);

	return $form;
}

function sirius_node_tab_employer_dispatch_preferences_form_submit($form, &$form_state) {
	$employer_node = $form_state['employer_node'];

	$need_save = FALSE;
	if ($employer_node->field_sirius_dispatch_notesdef['und'][0]['value'] != $form_state['values']['sirius_dispatch_notesdef']) {
		$employer_node->field_sirius_dispatch_notesdef['und'][0]['value'] = $form_state['values']['sirius_dispatch_notesdef'];
		$need_save = TRUE;
		drupal_set_message("Updated default dispatch notes.");
	}

	if ($need_save) {
		node_save($employer_node);
	}
}
