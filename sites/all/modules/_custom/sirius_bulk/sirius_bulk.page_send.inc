<?php

function sirius_bulk_node_tab_send($bulk_node) {
	return drupal_get_form('sirius_bulk_node_tab_send_form', $bulk_node);
}

function sirius_bulk_node_tab_send_form($form, &$form_state, $bulk_node) {
	drupal_add_css(drupal_get_path('module', 'sirius_bulk') . '/css/bulk.css');

	$form_state['bulk_nid'] = $bulk_node->nid;

	$form['summary'] = array('#markup' => sirius_bulk_queue_summary_render($bulk_node));

	$status = $bulk_node->field_sirius_bulk_status['und'][0]['value'];

	$actions = array(NULL => t('-- Select --'));
	if ($status == 'draft' || $status == 'sent') {
		$actions['send_now'] = 'Send Now';
		$actions['send_later'] = 'Send Later';
	}
	if ($status == 'pending') {
		$actions['pause'] = 'Pause';
	}

	$form['action'] = array(
		'#type' => 'select',
		'#title' => t('Action'),
		'#options' => $actions,
		'#required' => TRUE,
	);

	$form['send_at'] = array(
		'#title' => 'Send At',
		'#type' => 'date_popup',
		'#states' => array('visible' => array(array(":input[name=\"action\"]" => array('value' => 'send_later')))),
		'#attributes' => array('autocomplete' => 'off'),
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_bulk_node_tab_send_form_validate($form, &$form_state) {
	if ($form_state['values']['action'] == 'send_later') {
		$ts = strtotime($form_state['values']['send_at']);
		if ($ts < time()) { 
			form_set_error('send_at', t('Please select a date in the future.'));
		}
	}
}

function sirius_bulk_node_tab_send_form_submit($form, &$form_state) {
	$bulk_node = node_load($form_state['bulk_nid']);
	
	$action = $form_state['values']['action'];
	if ($action == 'send_now') {
		$bulk_node->field_sirius_datetime['und'][0]['value'] = gmdate('Y-m-d H:i:s');
		$bulk_node->field_sirius_bulk_status['und'][0]['value'] = 'pending';
	}

	if ($action == 'send_later') {
		$bulk_node->field_sirius_datetime['und'][0]['value'] = gmdate('Y-m-d H:i:s', strtotime($form_state['values']['send_at']));
		$bulk_node->field_sirius_bulk_status['und'][0]['value'] = 'pending';
	}

	if ($action == 'pause') {
		$bulk_node->field_sirius_bulk_status['und'][0]['value'] = 'draft';
	}

	node_save($bulk_node);
	drupal_set_message("Bulk message saved.");
}