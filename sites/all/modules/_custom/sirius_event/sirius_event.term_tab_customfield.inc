<?php

function sirius_event_term_tab_customfield($event_type_term) {
	return drupal_get_form('sirius_event_term_tab_customfield_form', $event_type_term);
}

function sirius_event_term_tab_customfield_form($form, &$form_state, $event_type_term) {
	$form_state['event_type_tid'] = $event_type_term->tid;
	$domain_nid = sirius_domain_nid();
	$options = sirius_customfield_set_list_all();
	if (!$options) { 
		drupal_set_message('No custom field sets have been defined.', 'warning');
		return array();
	}

	$json = sirius_jsonfield_util_term_json_get($event_type_term);
	$form['#tree'] = TRUE;
	$form['customfield']['sets'] = array(
		'#type' => 'checkboxes',
		'#title' => 'Custom Fields Sets',
		'#options' => $options,
	);
	if ($json['customfield']['sets']) { $form['customfield']['sets']['#default_value'] = $json['customfield']['sets']; }

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => 'Submit',
	);

	return $form;
}


function sirius_event_term_tab_customfield_form_submit($form, &$form_state) {
	$event_type_tid = $form_state['event_type_tid'];
	$event_type_term = taxonomy_term_load($event_type_tid);
	$json = sirius_jsonfield_util_term_json_get($event_type_term);
	$json['customfield'] = $form_state['values']['customfield'];
	sirius_jsonfield_util_term_json_set($event_type_term, $json);
	taxonomy_term_save($event_type_term);
	drupal_set_message("Settings saved.");
}

