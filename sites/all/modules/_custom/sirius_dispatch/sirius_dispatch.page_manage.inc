<?php

function sirius_node_tab_dispatch_manage($dispatch_node) {
	return drupal_get_form('sirius_node_tab_dispatch_manage_form', $dispatch_node);
}

function sirius_node_tab_dispatch_manage_form($form, &$form_state, $dispatch_node) {
	$form_state['dispatch_node'] = $dispatch_node;

	$renderable = node_view($dispatch_node, 'full');
	$form['intro'] = array(
		'#type' => 'markup',
		'#markup' => drupal_render($renderable)
	);

	$status = $dispatch_node->field_sirius_dispatch_status['und'][0]['value'];

	$actions = array(NULL => t('-- Select --'));

	$possible = sirius_dispatch_notify_possible($dispatch_node);
	if ($possible['success']) {
		$actions['notify'] 				= t('Send a notification');
	} 

	$possible = sirius_dispatch_accept_possible($dispatch_node);
	if ($possible['success']) {
		$actions['accept'] 				= t('Accept this dispatch');
	}

	$possible = sirius_dispatch_decline_possible($dispatch_node);
	if ($possible['success']) {
		$actions['decline'] 			= t('Decline this dispatch');
	}

	$possible = sirius_dispatch_undecline_possible($dispatch_node);
	if ($possible['success']) {
		$actions['undecline'] 			= t('Un-Decline this dispatch');
	}

	$possible = sirius_dispatch_terminate_possible($dispatch_node);
	if ($possible['success']) {
		$actions['terminate'] 		= t('Terminate this dispatch');
	}

	$possible = sirius_dispatch_resign_possible($dispatch_node);
	if ($possible['success']) {
		$actions['resign'] 			= t('Resign this dispatch');
	}

	$form['action'] = array(
		'#type' => 'select',
		'#title' => t('Action'),
		'#options' => $actions,
		'#required' => TRUE,
	);

	$medium_options = array(NULL => t('Use the worker\'s notification preference'));
	$medium_options += sirius_dispatch_notify_medium_options();
	$form['medium'] = array(
		'#title' => t('Notify by:'),
		'#type' => 'select',
		'#options' => $medium_options,
		'#default_value' => $worker_node->field_sirius_dispatch_medium['und'][0]['value'],
		'#states' => array(
			'visible' => array(
				array(':input[name="action"]' => array('value' => 'notify')),
			)
		),
	);


	$form['notes'] = array(
		'#type' => 'textarea',
		'#title' => t('Notes'),
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => 'Go',
	);

	$form += sirius_log_render_form($dispatch_node->nid);

  return $form;
}

function sirius_node_tab_dispatch_manage_form_submit($form, &$form_state) {
	$dispatch_node = $form_state['dispatch_node'];
	$action = $form_state['values']['action'];
	$notes = $form_state['values']['notes'];

	if ($action == 'notify') 				{ $result = sirius_dispatch_notify($dispatch_node, $form_state['values']['medium'], $notes); }
	if ($action == 'accept')				{ $result = sirius_dispatch_accept($dispatch_node, $notes); }
	if ($action == 'decline')				{ $result = sirius_dispatch_decline($dispatch_node, $notes); }
	if ($action == 'undecline')			{ $result = sirius_dispatch_undecline($dispatch_node, $notes); }
	if ($action == 'terminate')			{ $result = sirius_dispatch_terminate($dispatch_node, $notes); }
	if ($action == 'resign')				{ $result = sirius_dispatch_resign($dispatch_node, $notes); }

	if ($result['success']) {
		drupal_set_message("Success - " . $result['msg']);
	} else {
		drupal_set_message("Error - " . $result['msg'], 'error');
	}
}
