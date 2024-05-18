<?php

function sirius_bulk_node_tab_body_postal_access($bulk_node) {
	if (sirius_domain_nid() && !sirius_domain_component_enabled('bulk')) { return FALSE; }
	$medium = $bulk_node->field_sirius_bulk_medium['und'][0]['value'];
	return $medium == 'postal';
}

function sirius_bulk_node_tab_body_postal_form($form, &$form_state, $bulk_node) {
	$form_state['bulk_nid'] = $bulk_node->nid;
	$json = sirius_json_get($bulk_node);

	$form['#tree'] = TRUE;

	$postal_client = sirius_postal_client_instance();

	$form['title'] = array(
		'#title' => t('Title'),
		'#type' => 'textfield',
		'#default_value' => $bulk_node->title,
		'#description' => t('Will be used as the title for this record, but will not be shown to the user.'),
		'#required' => TRUE,
	);

	$form['postal']['options'] = $postal_client->mail_options_form();
 	sirius_jsonfield_util_set_defaults($form['postal']['options'], $json['postal']['options']);

	$form['body'] = array(
		'#title' => t('Body'),
		'#type' => 'text_format',
		'#default_value' => $bulk_node->body['und'][0]['value'],
		'#format' => $bulk_node->body['und'][0]['text_format'],
		'#required' => TRUE,
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
		'#required' => TRUE,
	);

	return $form;
}

function sirius_bulk_node_tab_body_postal_form_submit($form, &$form_state) {
	$bulk_node = node_load($form_state['bulk_nid']);

	$bulk_json = sirius_json_get($bulk_node);
	$bulk_json['postal'] = $form_state['values']['postal'];
	sirius_json_set($bulk_node, $bulk_json);

	$bulk_node->title = $form_state['values']['title'];
	$bulk_node->body['und'][0]['value'] = $form_state['values']['body']['value'];
	$bulk_node->body['und'][0]['text_format'] = $form_state['values']['body']['text_format'];
  node_submit($bulk_node);
	node_save($bulk_node);
	drupal_set_message("Message body saved.");
}