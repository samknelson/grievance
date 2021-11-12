<?php

function sirius_bulk_node_tab_medium_access($bulk_node) {
	if (sirius_domain_nid() && !sirius_domain_component_enabled('bulk')) { return FALSE; }
	return TRUE;
}

function sirius_bulk_node_tab_medium($bulk_node) {
	return drupal_get_form('sirius_bulk_node_tab_medium_form', $bulk_node);
}

function sirius_bulk_node_tab_medium_form($form, &$form_state, $bulk_node) {
	$form_state['bulk_nid'] = $bulk_node->nid;
	$medium = $bulk_node->field_sirius_bulk_medium['und'][0]['value'];
	$media = sirius_bulk_media();
	$json = sirius_jsonfield_util_node_json_get($bulk_node);

	$form['#tree'] = TRUE; 
  $form["medium"] = array(
    '#title' => t('Medium'),
		'#type' => 'select',
		'#options' => $media,
	  '#default_value' => $medium,
		'#required' => TRUE,
  );

	$options = sirius_do_not_disturb_list();
	$form['do_not_disturb']['id'] = array(
		'#type' => 'select',
		'#options' => $options,
		'#title' => t('Which Do Not Disturb schedules should be used?'),
		'#default_value' => $json['do_not_disturb']['id'],
	);

	if ($json['do_not_disturb']['id']) {
		$form['do_not_disturb']['render'] = array('#markup' => sirius_do_not_disurb_render($json['do_not_disturb']['id']));
	}

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
	$json = sirius_jsonfield_util_node_json_get($bulk_node);
	$json['do_not_disturb'] = $form_state['values']['do_not_disturb'];
	sirius_jsonfield_util_node_json_set($bulk_node, $json);
	node_save($bulk_node);
	drupal_set_message(t('Medium set to: ') . $media[$bulk_node->field_sirius_bulk_medium['und'][0]['value']]);
}