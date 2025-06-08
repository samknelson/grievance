<?php

function sirius_smf_node_tab_automsg_test_access($cron_node) {
	// @todo: Hardcoding MLK, because for now we only know of one automsg
	if ($cron_node->field_sirius_type['und'][0]['value'] != 'cron:scan:smf_automsg_mlk') { return FALSE; }

	return TRUE;
}

function sirius_smf_node_tab_automsg_test_form($form, &$form_state, $cron_node) {
	$cron_key = 'smf_automsg_mlk';

	$form_state['cron_nid'] = $cron_node->nid;
	$form_state['cron_key'] = $cron_key;
	$form['#tree'] = TRUE;

	$cron_info = sirius_cron_scan_info($cron_key);
	$instance = $cron_info['instance'];

	$msg_info_all = $instance->msg_info_all();
	$msg_options = array();
	foreach ($msg_info_all as $key => $info) {
		$msg_options[$key] = $info['title'];
	}

	$form['msg_key'] = array(
		'#title' => t('Message'),
		'#type' => 'select',
		'#options' => array(NULL => t('-- Select --')) + $msg_options,
		'#required' => TRUE,
	);

	$form['mode'] = array(
		'#title' => t('Mode'),
		'#type' => 'select',
		'#options' => array(
			'test' => t('Test'),
			'live' => t('Live'),
		),
		'#required' => TRUE,
		'#description' => 'Select "Test" to determine check the logic without actually delivering the message.',
	);

	$form['force'] = array(
		'#title' => t('Force?'),
		'#type' => 'select',
		'#options' => array(
			'No' => t('No'),
			'Yes' => t('Yes'),
		),
		'#required' => TRUE,
		'#description' => t('Force-deliver the message: send it even if the recipient has already received it, and even if it is disabled.')
	);

	$form['recipient_type'] = array(
		'#title' => t('Recipient Type'),
		'#type' => 'select',
		'#options' => array(
			'phone' => t('Phone Number'),
			'worker' => t('Member Record'),
		),
		'#required' => TRUE,
	);

	$form['phone'] = array(
		'#title' => t('Phone'),
		'#type' => 'textfield',
		'#size' => 12,
		'#states' => array('visible' => array(":input[name=\"recipient_type\"]" => array('value' => 'phone'))),
		'#description' => t('Enter the phone number which should receive the message.'),
	);

	$form['worker_id'] = array(
		'#title' => t('Member ID'),
		'#type' => 'textfield',
		'#size' => 6,
		'#states' => array('visible' => array(":input[name=\"recipient_type\"]" => array('value' => 'worker'))),
		'#description' => t('Enter the ID of the member who should receive the message'),
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_smf_node_tab_automsg_test_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;
	$cron_key = $form_state['cron_key'];

	$cron_info = sirius_cron_scan_info($cron_key);
	$instance = $cron_info['instance'];

	$cron_node = node_load($form_state['cron_nid']);

	$msg_key = $form_state['values']['msg_key'];
	$mode = $form_state['values']['mode'];
	$force = ($form_state['values']['force'] == 'Yes');

	if ($form_state['values']['recipient_type'] == 'phone') {
		$phone = $form_state['values']['phone'];
		$result = SiriusResult::create($instance->deliver_raw($msg_key, $phone, $mode, array('force' => $force)));
	} else {
		$worker_id = $form_state['values']['worker_id'];
		$allow_redelivery = $form_state['values']['allow_redelivery'];
		$result = SiriusResult::create($instance->deliver_from_worker_id($msg_key, $worker_id, $mode, array('force' => $force)));
	}

	$result->drupalSetMessage();

	$comm_node = $result->get('comm');
  if ($comm_node) {
    $path = 'node/' . $comm_node->nid . '/sirius_log_comm';
    $msg = "The message can be viewed and tracked <strong>" . l('here', $path) . '</strong>';
    drupal_set_message($msg);
    sirius_minilog($msg);
  }
}
