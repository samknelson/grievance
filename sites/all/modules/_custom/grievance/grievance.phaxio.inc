<?php

function grievance_phaxio_admin() {
  $form = array();

  $form['grievance_phaxio_api_key'] = array(
		'#default_value' => variable_get('grievance_phaxio_api_key', ''),
    '#title' => t('Phaxio API Key'),
    '#type' => 'textfield',
  );

  $form['grievance_phaxio_api_secret'] = array(
		'#default_value' => variable_get('grievance_phaxio_api_secret', ''),
    '#title' => t('Phaxio API Secret'),
    '#type' => 'textfield',
  );

  $form['grievance_phaxio_api_host'] = array(
		'#default_value' => variable_get('grievance_phaxio_api_host', 'https://api.phaxio.com/v1/'),
    '#title' => t('Phaxio API Host'),
    '#type' => 'textfield',
  );

  return system_settings_form($form);
}
