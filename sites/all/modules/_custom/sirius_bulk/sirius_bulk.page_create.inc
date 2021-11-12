<?php

function sirius_bulk_page_create() {
	if (sirius_domain_nid() && !sirius_domain_component_enabled('bulk')) { return 'The bulk message component is not enabled.'; }
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
	$bulk_node = sirius_bulk_create($form_state['values']['title'], $form_state['values']['medium']);
	drupal_set_message(t("Created a new bulk message: ") . $bulk_node->title);
	drupal_goto('node/' . $bulk_node->nid . '/sirius_bulk_medium');
}

