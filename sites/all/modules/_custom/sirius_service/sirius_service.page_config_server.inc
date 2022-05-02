<?php

function sirius_service_page_config_server() {
	return drupal_get_form('sirius_service_page_config_server_form');
}

function sirius_service_page_config_server_form($form, &$form_state) {
	$form['#tree'] = TRUE;

	$name_active = 'sirius_service_server_active';
  $form[$name_active] = array(
  	'#title' => t('Server Active'),
  	'#type' => 'select',
  	'#options' => array(
  		'No' => 'No',
  		'Yes' => 'Yes',
  	),
    '#default_value' => sirius_domain_variable_get($name_active, ''),
    '#id' => $name_active,
  );

  $form['sirius_service_user_name'] = array(
  	'#type' => 'textfield',
  	'#title' => 'Run As User Name',
		'#states' => array(
			'visible' => array(
				array(':input[id="' . $name_active . '"]' => array('value' => 'Yes')),
			)
		),
    '#default_value' => sirius_domain_variable_get('sirius_service_user_name', ''),
		'#description' => t('Enter the user name that remote commands should run as.'),
  );

  $name = 'sirius_service_server_ips';
  $form[$name] = array(
  	'#title' => t('Server IP Allowlist'),
  	'#type' => 'textarea',
    '#default_value' => sirius_domain_variable_get($name, ''),
    '#field_prefix' => '<p>' . sirius_ip_rawrange_helptext(sirius_domain_variable_get($name)) . '</p>',
		'#states' => array(
			'visible' => array(
				array(':input[id="' . $name_active . '"]' => array('value' => 'Yes')),
			)
		),
  );

  $form['tokens'] = array(
  	'#type' => 'fieldset',
  	'#title' => 'Access Tokens',
		'#collapsible' => TRUE, 
		'#collapsed' => TRUE,
		'#states' => array(
			'visible' => array(
				array(':input[id="' . $name_active . '"]' => array('value' => 'Yes')),
			)
		),
  );

  $domain = sirius_domain();

  $form['tokens']['account'] = array(
  	'#markup' => sirius_fakerender_field('Account ID:', $domain->uuid),
  );

  $access = sirius_domain_variable_get('sirius_service_server_access');
  if ($access) {
	  $form['tokens']['access'] = array(
	  	'#markup' => sirius_fakerender_field('Access Code:', $access),
	  );
	} else {
  	$form['tokens']['access'] = array(
  		'#markup' => sirius_fakerender_field('Access Code:', '<em>No access code set</em>'),
  	);
  }

  $form['tokens']['action'] = array(
  	'#type' => 'select',
  	'#title' => 'Action',
  	'#options' => array(
  		NULL => t('-- No action --'),
  		'clear' => t('Clear the current access code'),
  		'generate' => t('Generate a new access code'),
  	),
  	'#description' => t('Select <strong>"Clear"</strong> to remove the access code. No connections will be allowed.') . '<br /> ' . t('Select <strong>"Generate"</strong> to generate a new access code. All clients will need to be updated.'),
  );

  $form['submit'] = array(
  	'#type' => 'submit',
  	'#value' => t('Submit'),
  );

  return $form;
}

function sirius_service_page_config_server_form_validate($form, &$form_state) {
	$name = 'sirius_service_server_ips';
	$result = sirius_ip_rawrange_validate($form_state['values'][$name]);
	if (!$result['success']) { form_set_error($name, $result['msg']); }

	$user_name = $form_state['values']['sirius_service_user_name'];
	if ($user_name) {
		$remote_user = user_load_by_name($user_name);
		if (!$remote_user) {
			form_set_error('sirius_service_user_name', "User $user_name not found.");
		} else if ($remote_user->field_sirius_domain['und'][0]['target_id'] != sirius_domain_nid()) {
			form_set_error('sirius_service_user_name', "User $user_name is not in the current domain.");
		}
	}
}

function sirius_service_page_config_server_form_submit($form, &$form_state) {
	foreach (array('sirius_service_server_active', 'sirius_service_server_ips', 'sirius_service_user_name') as $name) {
		sirius_domain_variable_set($name, $form_state['values'][$name]);
	}

	if ($form_state['values']['tokens']['action'] == 'clear') {
		sirius_domain_variable_set('sirius_service_server_access', '');
		drupal_set_message("The access code has been cleared. All connections to this server will fail.");
	}
	if ($form_state['values']['tokens']['action'] == 'generate') {
		sirius_domain_variable_set('sirius_service_server_access', uuid_generate());
		drupal_set_message("A new access code has been generated. All clients will need to be updated.");
	}

	drupal_set_message("Settings saved.");
}