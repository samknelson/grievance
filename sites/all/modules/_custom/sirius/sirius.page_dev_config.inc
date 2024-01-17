<?php

function sirius_page_dev_config() {
	return drupal_get_form('sirius_page_dev_config_form');
}

function sirius_page_dev_config_form($form, &$form_state) {
	$form = array();
	$form['sirius_dev_server_type'] = array(
		'#title' => t('Server Type'),
		'#type' => 'select',
		'#options' => sirius_dev_server_types(),
		'#default_value' => sirius_dev_server_type(),
		'#description' => t('Server type -- development, staging, or live.'),
	);

  $form['#submit'][] = 'sirius_page_dev_config_form_submit';

  $form['submit'] = array(
  	'#type' => 'submit',
  	'#value' => t('Submit'),
  );

  return $form;
}

function sirius_page_dev_config_form_submit($form, &$form_state) {
	$type = $form_state['values']['sirius_dev_server_type'];
	sirius_dev_server_type_set($type);

	$name = sirius_dev_server_types()[$type];
	if (!$name) { $name = '[no server type]'; }
	drupal_set_message("Type set to: $name");
}