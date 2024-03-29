<?php

function sirius_node_tab_entity_customfield_settings_access($entity_node) {
	// You can't put custom fields on custom fields. 
	// If we're a log entry of type "customfield", it means we're a single customfield entry for somebody else.
	// So we can't add or remove other customfield forms.
	if ($entity_node->type == 'sirius_log' && $entity_node->field_sirius_category['und'][0]['value'] == 'customfield') {
		return FALSE;
	}

	// If there aren't any non-show-by-default options, don't show the tab
	$options = sirius_customfield_set_list_all(
		$entity_node->type, 
		$entity_node->field_sirius_category['und'][0]['value'],
		array('tab' => array('show_by_default' => 'No'))
	);
	if (!$options) { return FALSE; }

	return TRUE;
}

function sirius_node_tab_entity_customfield_settings_form($form, &$form_state, $entity_node) {
	$form_state['entity_nid'] = $entity_node->nid;
	$options = sirius_customfield_set_list_all($entity_node->type);
	if (!$options) { 
		drupal_set_message('No custom field sets have been defined.', 'warning');
		return array();
	}

	$json = sirius_json_get($entity_node);
	$form['#tree'] = TRUE;
	$form['customfield']['sets'] = array(
		'#type' => 'checkboxes',
		'#title' => 'Custom Fields Sets',
		'#options' => $options,
		'#default_value' => sirius_form_array_default_value(array_keys(sirius_customfield_set_list($entity_node))),
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => 'Submit',
	);

	return $form;
}

function sirius_node_tab_entity_customfield_settings_form_submit($form, &$form_state) {
	$entity_nid = $form_state['entity_nid'];
	$entity_node = node_load($entity_nid);
	$json = sirius_json_get($entity_node);
	$json['customfield'] = $form_state['values']['customfield'];
	sirius_json_set($entity_node, $json);
	node_save($entity_node);
	drupal_set_message("Settings saved.");
}

function sirius_term_tab_entity_customfield_settings_access($entity_term) {
	if (user_access('sirius admin')) { return TRUE; }
	if (user_access('sirius domain admin')) { return TRUE; }

  // @todo: At some point, we probably need some kind of a fancy system for deciding which event roles and statuses can access which custom field sets.
  if ($entity_term->vocabulary_machine_name == 'sirius_event_type') {
	 if (user_access('sirius event admin')) { return TRUE; }
  }

	return FALSE;
}


function sirius_term_tab_entity_customfield_settings($entity_term) {
	return drupal_get_form('sirius_term_tab_entity_customfield_settings_form', $entity_term);
}

function sirius_term_tab_entity_customfield_settings_form($form, &$form_state, $entity_term) {
	$form_state['entity_tid'] = $entity_term->tid;
	$options = sirius_customfield_set_list_all();
	if (!$options) { 
		drupal_set_message('No custom field sets have been defined.', 'warning');
		return array();
	}

	$json = sirius_json_get($entity_term);
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

function sirius_term_tab_entity_customfield_settings_form_submit($form, &$form_state) {
	$entity_tid = $form_state['entity_tid'];
	$entity_term = taxonomy_term_load($entity_tid);
	$json = sirius_json_get($entity_term);
	$json['customfield'] = $form_state['values']['customfield'];
	sirius_json_set($entity_term, $json);
	taxonomy_term_save($entity_term);
	drupal_set_message("Settings saved.");
}

