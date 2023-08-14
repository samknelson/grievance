<?php

function sirius_node_tab_worker_contact_phone_access($worker_node) {
	return node_access('view', $worker_node);
}

function sirius_node_tab_worker_contact_phone($worker_node) {
	return drupal_get_form('sirius_node_tab_worker_contact_phone_form', $worker_node);
}

function sirius_node_tab_worker_contact_phone_form($form, &$form_state, $worker_node) {
	$form_state['worker_nid'] = $worker_node->nid;
	$form['#tree'] = TRUE;

	form_load_include($form_state, 'inc', 'sirius_worker', 'sirius_worker.page_contact_phone');

	sirius_phone_field_with_validation(
		$form,
		$form_state,
		$worker_node->nid
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_node_tab_worker_contact_phone_form_submit($form, &$form_state) {
	drupal_set_message("Form submit: " . $form_state['field_submit']);
}






function sirius_phone_field_with_validation(&$form, &$form_state, $container_nid, $options = array()) {
	$container_node = node_load($container_nid);
	$container_json = sirius_json_get($container_node);

	$phone_field_disabled = FALSE;
	$code_field_hidden = TRUE;

	$action = $form_state['triggering_element']['#sirius_ajax']['action'];

	// Validate our input
	if ($action == 'send_code') {
		$phone = $form_state['input']['phone_field_with_validation']['phone'];
		if (!$phone) {
			$msg = "Please enter a phone number";
			$action = '';
		} else if (!sirius_phone_validate($phone)) {
			$msg = "Please enter a valid phone number";
			$action = '';
		}
	}

	if ($action == 'enter_code') {
		$input_code = $form_state['input']['phone_field_with_validation']['code'];
		$correct_code = $container_json['phone_field_with_validation']['code'];
		if (!$input_code) {
			$msg = "Please enter a validation code";
			$action = 'send_code';
		} else if ($input_code != $correct_code) {
			$msg = "That is not the correct code.";
			$action = 'send_code';
		}
	}

	$form['phone_field_with_validation'] = array(
		'#type' => 'fieldgroup',
		'#prefix' => '<div id="sirius_phone_field_with_validation">',
		'#suffix' => '</div>',
	);

	// No action -- Show the phone field and "send code" button
	if (!$action) {
		$form['phone_field_with_validation']['phone'] = array(
			'#type' => 'textfield',
			'#title' => 'Phone',
			'#size' => 12,
			'#default_value' => $container_json['phone_field_with_validation']['phone'],
		);

		$form['phone_field_with_validation']['send_code'] = array(
			'#type' => 'button',
			'#value' => t('Send Code'),
	    '#ajax' => array(
	      'callback' => 'sirius_ajax_generic_replace',
	    ),
	    '#sirius_ajax' => array(
	    	'wrapper' => '#sirius_phone_field_with_validation',
	    	'element' => 'phone_field_with_validation',
	    	'action' => 'send_code',
	    ),
	    '#limit_validation_errors' => array(),
		);
	}

	// Action is "send_code" -- send the code, disable the phone field and show the "enter code" option
	if ($action == 'send_code') {
		// Generate a code and save for later
		$container_json['phone_field_with_validation']['phone'] = sirius_phone_format($phone);
		$container_json['phone_field_with_validation']['confirmed'] = FALSE;
		$container_json['phone_field_with_validation']['code'] = rand(100000, 999999);
		$msg = "Your code is: " . $container_json['phone_field_with_validation']['code'];
		$msg .= "Your phone is: $phone";
		sirius_json_set($container_node, $container_json);
		node_save($container_node);

		$form['phone_field_with_validation']['phone'] = array(
			'#type' => 'textfield',
			'#title' => 'Phone',
			'#size' => 12,
			'#value' => $container_json['phone_field_with_validation']['phone'],
			'#disabled' => TRUE,
		);

		$form['phone_field_with_validation']['code'] = array(
			'#type' => 'textfield',
			'#title' => 'Validation Code',
			'#size' => 8,
		);

		$form['phone_field_with_validation']['enter_code'] = array(
			'#type' => 'button',
			'#value' => t('Enter Code'),
	    '#ajax' => array(
	      'callback' => 'sirius_ajax_generic_replace',
	    ),
	    '#sirius_ajax' => array(
	    	'wrapper' => '#sirius_phone_field_with_validation',
	    	'element' => 'phone_field_with_validation',
	    	'action' => 'enter_code',
	    ),
	    '#limit_validation_errors' => array(),
		);
	}

	// Action is "send_code" -- send the code, disable the phone field and show the "enter code" option
	if ($action == 'enter_code') {
		// Generate a code and save for later
		$container_json['phone_field_with_validation']['confirmed'] = TRUE;
		$msg = t('Your phone number has been confirmed.');
		sirius_json_set($container_node, $container_json);
		node_save($container_node);

		$form['phone_field_with_validation']['phone'] = array(
			'#type' => 'textfield',
			'#title' => 'Phone',
			'#size' => 12,
			'#value' => $container_json['phone_field_with_validation']['phone'],
			'#disabled' => TRUE,
		);

		$form['phone_field_with_validation']['enter_code'] = array(
			'#type' => 'button',
			'#value' => t('Start Over'),
	    '#ajax' => array(
	      'callback' => 'sirius_ajax_generic_replace',
	    ),
	    '#sirius_ajax' => array(
	    	'wrapper' => '#sirius_phone_field_with_validation',
	    	'element' => 'phone_field_with_validation',
	    	'action' => '',
	    ),
	    '#limit_validation_errors' => array(),
		);
	}

	$form['phone_field_with_validation']['msg'] = array(
		'#markup' => '<div class="sirius_phone_field_with_validation_msg">' . $msg . '</div>',
		'#weight' => -10,
	);
}

function sirius_phone_field_with_validation_submit($form, &$form_state) {
	drupal_set_message("Field submit.");
	$form_state['field_submit'] = 666;

	$form_id = $form_state['build_info']['form_id'];
	$callback = $form_id . "_submit";
	if (function_exists($callback)) {
		drupal_set_message("Chaining to: $callback");
		return $callback($form, $form_state);
	}
}

function sirius_phone_field_with_validation_validate($form, &$form_state) {
	drupal_set_message("Field validate.");
}