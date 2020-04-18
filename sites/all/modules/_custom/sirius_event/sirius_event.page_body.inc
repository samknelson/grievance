<?php

function sirius_node_tab_event_body_access($event_node) {
	if (user_access('sirius event admin')) { return true; }

	$event_type_tid = $event_node->field_sirius_event_type['und'][0]['tid'];
	$event_type_term = taxonomy_term_load($event_type_tid);
	return TRUE;
}

function sirius_node_tab_event_body($event_node) {
	return drupal_get_form('sirius_node_tab_event_body_form', $event_node->nid);
}

function sirius_node_tab_event_body_form($form, &$form_state, $event_nid) {
	$form_state['event_nid'] = $event_nid;
	$event_node = node_load($event_nid);

  $form["description"] = array(
    '#title' => t('Description'),
		'#type' => 'text_format',
		'#required' => TRUE,
		'#default_value' => $event_node->body['und'][0]['value'],
		'#format' => $event_node->body['und'][0]['text_format'],
  );

  $form["active"] = array(
    '#title' => t('Active?'),
		'#type' => 'select',
		'#options' => array(
			'Yes' => t('Yes'),
			'No' => t('No'),
		),
		'#default_value' => $event_node->field_sirius_active['und'][0]['value'],
		'#description' => t('Marking an event as inactive removes it from most lists and prevents users from registering for the event.'),
  );

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => 'Save',
	);

	$form['suffix'] = array('#markup' => '</div>');

	return $form;
}

function sirius_node_tab_event_body_form_submit($form, &$form_state) {
	$event_nid = $form_state['event_nid'];
	$event_node = node_load($event_nid);
	$event_node->body['und'][0]['value'] = $form_state['values']['description']['value'];
	$event_node->body['und'][0]['format'] = $form_state['values']['description']['format'];
	$event_node->field_sirius_active['und'][0]['value'] = $form_state['values']['active'];
	node_save($event_node);
	drupal_set_message(t('Event description saved.'));
}
