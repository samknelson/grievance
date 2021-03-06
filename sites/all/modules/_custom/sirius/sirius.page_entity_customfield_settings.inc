<?php

function sirius_node_tab_entity_customfield_settings_access($entity_node) {
	if (user_access('sirius admin')) { return TRUE; }
	if (user_access('sirius domain admin')) { return TRUE; }

  // @todo: At some point, we probably need some kind of a fancy system for deciding which event roles and statuses can access which custom field sets.
  if ($entity_node->type == 'sirius_event') {
	 if (user_access('sirius event admin')) { return TRUE; }
  }

  if ($entity_node->type == 'sirius_ledger_account') {
   if (user_access('sirius ledgerr admin')) { return TRUE; }
  }

	return FALSE;
}


function sirius_node_tab_entity_customfield_settings($entity_node) {
	return drupal_get_form('sirius_node_tab_entity_customfield_settings_form', $entity_node);
}

function sirius_node_tab_entity_customfield_settings_form($form, &$form_state, $entity_node) {
	$form_state['entity_nid'] = $entity_node->nid;
	$domain_nid = sirius_domain_nid();
	$options = sirius_customfield_set_list_all();
	if (!$options) { 
		drupal_set_message('No custom field sets have been defined.', 'warning');
		return array();
	}

	$json = sirius_jsonfield_util_node_json_get($entity_node);
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


function sirius_node_tab_entity_customfield_settings_form_submit($form, &$form_state) {
	$entity_nid = $form_state['entity_nid'];
	$entity_node = node_load($entity_nid);
	$json = sirius_jsonfield_util_node_json_get($entity_node);
	$json['customfield'] = $form_state['values']['customfield'];
	sirius_jsonfield_util_node_json_set($entity_node, $json);
	node_save($entity_node);
	drupal_set_message("Settings saved.");
}

