<?php

function sirius_node_tab_phonenumber_manual_block($phonenumber_node) {
	return drupal_get_form('sirius_node_tab_phonenumber_manual_block_form', $phonenumber_node);
}

function sirius_node_tab_phonenumber_manual_block_form($form, &$form_state, $phonenumber_node) {
	$form_state['phonenumber_nid'] = $phonenumber_node->nid;

	$phonenumber_json = sirius_json_get($phonenumber_node);

	$form['#tree'] = TRUE;

	$form['manual_block']['sms'] = array(
		'#type' => 'select',
		'#title' => t('SMS Block'),
		'#options' => array(
			NULL => t('-- Select --'),
			'No' => t('No'),
			'Yes' => t('Yes'),
		),
		'#description' => t('Should a manual SMS block be applied to this number? If set to "Yes", then it will not be possible to send SMS messages to this number.'),
		'#default_value' => $phonenumber_json['manual_block']['sms'],
	);

	$form['manual_block']['voice'] = array(
		'#type' => 'select',
		'#title' => t('Voice Block'),
		'#options' => array(
			NULL => t('-- Select --'),
			'No' => t('No'),
			'Yes' => t('Yes'),
		),
		'#description' => t('Should a manual voice block be applied to this number? If set to "Yes", then it will not be possible to make outgoing voice calls to this number'),
		'#default_value' => $phonenumber_json['manual_block']['voice'],
	);

	$form['manual_block']['comments'] = array(
		'#type' => 'textarea',
		'#title' => t('Comments'),
		'#default_value' => $phonenumber_json['manual_block']['comments'],
	);

	/*
	$form['clear_last_sms'] = array(
		'#type' => 'select',
		'#title' => t('Clear Last SMS?'),
		'#options' => array(
			NULL => t('-- Select --'),
			'No' => t('No'),
			'Yes' => t('Yes'),
		),
		'#info' => 'Use this tool to remove the last SMS record. This can be used to manually remove a send block resulting from a STOP. Warning: This will not clear the block from the service provider, and should generally only be used for development or testing purposes.',
	);
	*/

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_node_tab_phonenumber_manual_block_form_submit($form, &$form_state) {
	$phonenumber_node = node_load($form_state['phonenumber_nid']);
	$phonenumber_json = sirius_json_get($phonenumber_node);
	$phonenumber_json['manual_block'] = $form_state['values']['manual_block'];
	$phonenumber_json['manual_block']['ts'] = time();

	if ($form_state['values']['clear_last_sms'] == 'Yes') {
		unset($phonenumber_json['last_sms']);
	}
	sirius_json_set($phonenumber_node, $phonenumber_json);
	node_save($phonenumber_node);
	drupal_set_message("Settings saved.");
  $log = sirius_minilog_render_html();
  if ($log) { drupal_set_message($log); }

  sirius_log(
    'phonenumber', 
    'settings', 
    $phonenumber_node->title . ': Settings updated', 
    $phonenumber_node->title . ': Settings updated',
    print_r($form_state['values']['manual_block'], 1), 
    array($phonenumber_node->nid),
    NULL,
    $form_state['values']['manual_block']
  );

}