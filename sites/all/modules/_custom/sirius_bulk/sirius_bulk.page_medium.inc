<?php

function sirius_bulk_node_tab_medium($bulk_node) {
	return drupal_get_form('sirius_bulk_node_tab_medium_form', $bulk_node);
}

function sirius_bulk_node_tab_medium_form($form, &$form_state, $bulk_node) {
	$form_state['bulk_nid'] = $bulk_node->nid;
	$medium = $bulk_node->field_sirius_bulk_medium['und'][0]['value'];
	$media = sirius_bulk_media();

  $form["medium"] = array(
    '#title' => t('Medum'),
		'#type' => 'select',
		'#options' => $media,
	  '#default_value' => $medium,
		'#required' => TRUE,
  );

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_bulk_node_tab_medium_form_submit($form, &$form_state) {
	$media = sirius_bulk_media();
	$bulk_node = node_load($form_state['bulk_nid']);
	$bulk_node->field_sirius_bulk_medium['und'][0]['value'] = $form_state['values']['medium'];
	node_save($bulk_node);
	drupal_set_message(t('Medium set to: ') . $media[$bulk_node->field_sirius_bulk_medium['und'][0]['value']]);
}