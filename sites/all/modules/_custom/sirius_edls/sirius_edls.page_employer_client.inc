<?php

function sirius_edls_node_tab_employer_client_access($employer_node) {
	return sirius_access_instance('edls')->access('employer_config', array('employer_nid' => $employer_node->nid));
}

function sirius_edls_node_tab_employer_client_form($form, &$form_state, $employer_node) {
	$form['#tree'] = TRUE;
	$form_state['employer_nid'] = $employer_node->nid;

	$employer_json = sirius_json_get($employer_node);

	$form['client']['active'] = array(
		'#type' => 'select',
		'#title' => t('Is this employer active as a client?'),
		'#options' => array(
			NULL => t('-- Select --'),
			'No' => t('No'),
			'Yes' => t('Yes'),
		),
		'#default_value' => $employer_json['edls']['client']['active'],
		'#description' => t('(Active employers will attempt to fetch updated worker lists from the server.'),
	);

  $form['client']['uuid'] = array(
  	'#type' => 'textfield',
  	'#title' => 'Server Account',
  	'#default_value' => $employer_json['edls']['client']['uuid']
  );

  $form['client']['access_code'] = array(
  	'#type' => 'textfield',
  	'#title' => 'Server Access Code',
  	'#default_value' => $employer_json['edls']['client']['access_code']
  );

  $form['submit'] = array(
  	'#type' => 'submit',
  	'#value' => t('Submit'),
  );

	return $form;
}

function sirius_edls_node_tab_employer_client_form_submit($form, &$form_state) {
	$employer_node = node_load($form_state['employer_nid']);
	$employer_json = sirius_json_get($employer_node);
	$employer_json['edls']['client'] = $form_state['values']['client'];
	sirius_json_set($employer_node, $employer_json);
	node_save($employer_node);
	drupal_set_message("Settings saved.");

	if ($form_state['values']['client']['active'] == 'Yes') {
		$result = sirius_edls_client_worker_list($form_state['values']['client']['uuid'], $form_state['values']['client']['access_code']);
		if ($result['success']) {
			drupal_set_message("Remote employer lookup succeeded.");
		} else {
			drupal_set_message("Remote employer lookup failed: " . $result['msg'], 'warning');
		}
	}
}
