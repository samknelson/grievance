<?php

function sirius_edls_node_tab_employer_server_access($employer_node) {
	return sirius_access_instance('edls')->access('employer_config', array('employer_nid' => $employer_node->nid));
}

function sirius_edls_node_tab_employer_server_form($form, &$form_state, $employer_node) {
	$form['#tree'] = TRUE;
	$form_state['employer_nid'] = $employer_node->nid;

	$employer_json = sirius_json_get($employer_node);

  $form['server']['uuid'] = array(
  	'#markup' => sirius_fakerender_field('Employer UUID:', $employer_node->uuid),
  );

  $access = $employer_json['edls']['server']['access_code'];
  if ($access) {
	  $form['server']['access_code'] = array(
	  	'#markup' => sirius_fakerender_field('Access Code:', $access),
	  );
	} else {
  	$form['server']['access_code'] = array(
  		'#markup' => sirius_fakerender_field('Access Code:', '<em>No access code set</em>'),
  	);
  }

  $form['action'] = array(
  	'#type' => 'select',
  	'#title' => 'Action',
  	'#options' => array(
  		NULL => t('-- No action --'),
  		'clear' => t('Clear the current access code'),
  		'generate' => t('Generate a new access code'),
  	),
  	'#description' => t('Select <strong>"Clear"</strong> to remove the access code. No connections will be allowed.') . '<br /> ' . t('Select <strong>"Generate"</strong> to generate a new access code. All clients will need to be updated.'),
  	'#required' => TRUE,
  );

	if (sirius_access_instance('edls')->access('employer_config_active', array('employer_nid' => $employer_node->nid))) {
		$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Submit'),
		);
	} else {
		$form['edls']['active']['#disabled'] = TRUE;
	}

	return $form;
}

function sirius_edls_node_tab_employer_server_form_submit($form, &$form_state) {
	$employer_node = node_load($form_state['employer_nid']);
	$employer_json = sirius_json_get($employer_node);

	if ($form_state['values']['action'] == 'clear') {
		$acccess = NULL;
		drupal_set_message("The access code has been cleared. All connections to this employer will fail.");
	}

	if ($form_state['values']['action'] == 'generate') {
		$access = uuid_generate();
		drupal_set_message("A new access code has been generated. All clients will need to be updated.");
	}

	$employer_json['edls']['server']['access_code'] = $access;
	sirius_json_set($employer_node, $employer_json);
	node_save($employer_node);

	drupal_set_message("Settings saved.");
}
