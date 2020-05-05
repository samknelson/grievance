<?php

function sirius_node_tab_event_class_settings_access($event_node) {
	// Must be a class
	if (!sirius_event_class_enabled($event_node)) { return FALSE; }

	// Admins can access
  if (user_access('sirius event admin')) { return TRUE; }

  // Check our edit rules
  $event_type_term = taxonomy_term_load($event_node->field_sirius_event_type['und'][0]['tid']);
  $json = sirius_jsonfield_util_term_json_get($event_type_term);
  $rule = sirius_event_paccess_rule_match($event_node->nid, $json['edit']['rules']);
  if ($rule) { return TRUE; }

  // Oh well
  return FALSE;
}

function sirius_node_tab_event_class_settings($event_node) {
	return drupal_get_form('sirius_node_tab_event_class_settings_form', $event_node->nid);
}

function sirius_node_tab_event_class_settings_form($form, &$form_state, $event_nid) {
	$form_state['event_nid'] = $event_nid;
	$event_node = node_load($event_nid);
	$event_json = sirius_jsonfield_util_node_json_get($event_node);
	$event_type_term = taxonomy_term_load($event_node->field_sirius_event_type['und'][0]['tid']);

	$form['#tree'] = TRUE;

	$form['class']['listing']['mode'] = array(
		'#type' => 'select',
		'#title' => t('Listing Mode'),
		'#options' => array(
			'private' => t('Private: do not list this class'), 
			'public' => t('Public: list this class on the public web site'),
			'workers' => t('Workers: List this class to active workers only'),
		),
		'#default_value' => $event_json['class']['listing']['mode'],
		'#id' => 'class_listing_mode',
	);

	$ms_options = sirius_taxonomy_load('sirius_member_status', 'tid', 'name');
	$form['class']['listing']['ms_tids'] = array(
		'#type' => 'checkboxes',
		'#title' => t('Show only to workers with these member statuses'),
		'#options' => $ms_options,
		'#checkall' => TRUE,
		'#states' => array('visible' => array(":input[id=\"class_listing_mode\"]" => array('value' => 'workers'))),
	);
	if ($event_json['class']['listing']['ms_tids']) { $form['class']['listing']['ms_tids']['#default_value'] = $event_json['class']['listing']['ms_tids']; }

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_node_tab_event_class_settings_form_submit($form, &$form_state) {
	$event_nid = $form_state['event_nid'];
	$event_node = node_load($event_nid);
	$event_json = sirius_jsonfield_util_node_json_get($event_node);
	$event_json['class'] = $form_state['values']['class'];
	sirius_jsonfield_util_node_json_set($event_node, $event_json);
	node_save($event_node);
	drupal_set_message(t('Settings saved.'));
}
