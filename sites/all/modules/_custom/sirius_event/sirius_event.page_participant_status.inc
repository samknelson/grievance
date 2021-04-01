<?php

function sirius_event_node_tab_participant_status($participant_node) {
	return drupal_get_form('sirius_event_node_tab_participant_status_form', $participant_node);
}

function sirius_event_node_tab_participant_status_form($form, &$form_state, $participant_node) {
	$form_state['participant_nid'] = $participant_node->nid;

	$participant_json = sirius_jsonfield_util_node_json_get($participant_node);
	$prole_tid = $participant_node->field_sirius_event_prole['und'][0]['tid'];

	$form['#tree'] = TRUE;
  $form['pstatus_tid'] = array(
  	'#title' => t('Status'),
  	'#type' => 'select',
  	'#options' => sirius_event_participant_role_status_tids($prole_tid),
  	'#default_value' => $participant_node->field_sirius_event_pstatus['und'][0]['tid'],
  );

	$form['notify']['medium'] = array(
		'#title' => 'Notify by',
		'#type' => 'select',
		'#options' => array(
			'NULL' => t('No notification'),
			'sms' => t('Text message'),
			'email' => t('Email message'),
			'both' => t('Both text and email messages'),
		),
	);

	$form['notify']['body'] = array(
		'#type' => 'textarea',
		'#title' => t('Add a message'),
		'#states' => array(
			'visible' => array(
				':input[name="notify"]' => array(
					array('value' => 'sms'),
					array('value' => 'email'),
					array('value' => 'both')
				),
			),
		),
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_event_node_tab_participant_status_form_submit($form, &$form_state) {
	$participant_node = node_load($form_state['participant_nid']);
	$participant_json = sirius_jsonfield_util_node_json_get($participant_node);

	$prole_tid = $participant_node->field_sirius_event_prole['und'][0]['tid'];
	$result = sirius_event_participant_pstatus_set(
		$participant_node, 
		$prole_tid, 
		$form_state['values']['pstatus_tid'],
		$form_state['values']['notify']
	);
	if ($result['success']) { 
		drupal_set_message($result['msg']);
	} else {
		drupal_set_message($result['msg'], 'warning');
	}
}
