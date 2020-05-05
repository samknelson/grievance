<?php

function sirius_bulk_page_create() {
	return drupal_get_form('sirius_bulk_page_create_form');
}

function sirius_bulk_page_create_form($form, &$form_state) {
	$media = sirius_bulk_media();

	$form['title'] = array(
		'#title' => t('Subject'),
		'#type' => 'textfield',
		'#required' => TRUE,
	);

  $form["medium"] = array(
    '#title' => t('Medum'),
		'#type' => 'select',
		'#options' => $media,
		'#required' => TRUE,
  );

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Create Bulk Message'),
	);

	return $form;
}

function sirius_bulk_page_create_form_submit($form, &$form_state) {
	global $user;
	$domain_nid = sirius_domain_nid();

	// Create the event node
	$bulk_node = new stdClass();
	$bulk_node->type = 'sirius_bulk';
	$bulk_node->no_recurse = TRUE;
	$bulk_node->language = $user->language;
	$bulk_node->uid = $user->uid;
	$bulk_node->status = 1;
	$bulk_node->promote = 0;
	$bulk_node->comment = 0;
	$bulk_node->title = $form_state['values']['title'];
	$bulk_node->field_sirius_bulk_medium['und'][0]['value'] = $form_state['values']['medium'];
	$bulk_node->field_sirius_bulk_status['und'][0]['value'] = 'draft';
	if ($domain_nid) { $bulk_node->field_sirius_domain['und'][0]['target_id'] = $domain_nid; }
	node_object_prepare($bulk_node);
	$bulk_node = node_submit($bulk_node);
	node_save($bulk_node);
	drupal_set_message(t("Created a new bulk message: ") . $bulk_node->title);

	drupal_goto('node/' . $bulk_node->nid . '/sirius_bulk_body');
}
