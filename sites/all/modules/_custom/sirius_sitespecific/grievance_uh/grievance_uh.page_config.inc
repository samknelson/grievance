<?php

function grievance_uh_page_config() {
	return drupal_get_form('grievance_uh_page_config_form');
}

function grievance_uh_page_config_form($form, &$form_state) {
	$form['#tree'] = TRUE;

	$name = 'grievance_uh_local_id';
  $form[$name] = array(
  	'#title' => t('Local Number'),
  	'#type' => 'textfield',
    '#default_value' => sirius_domain_variable_get($name, ''),
    '#description' => t('This should be the local number, i.e. "226", or "23".'), 
  );

  $form['submit'] = array(
  	'#type' => 'submit',
  	'#value' => t('Submit'),
  );

  return $form;
}

function grievance_uh_page_config_form_submit($form, &$form_state) {
	foreach (array(
		'grievance_uh_local_id', 
	) as $name) {
		sirius_domain_variable_set($name, $form_state['values'][$name]);
	}

	drupal_set_message("Settings saved.");
}