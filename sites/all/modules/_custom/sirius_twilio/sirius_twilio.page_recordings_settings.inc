<?php

function sirius_twilio_page_recordings_settings() {
	return drupal_get_form('sirius_twilio_page_recordings_settings_form');
}

function sirius_twilio_page_recordings_settings_form($form, &$form_state) {
	$name = sirius_domain_variable_name('sirius_twilio_recordings_delete');
  $form[$name] = array(
    '#type' => 'select',
    '#title' => t('Auto-delete recordings?'),
    '#options' => array(
      'No' => t('No'),
      'Yes' => t('Yes'),
    ),
    '#default_value' => variable_get($name, FALSE),
    '#description' => t('Should recordings stored on Twilio be automatically deleted? This will NOT affect recordings stored locally.'),
  );

	$name = sirius_domain_variable_name('sirius_twilio_recordings_delete_days');
  $form[$name] = array(
    '#type' => 'textfield',
    '#title' => t('Number of days to store recordings'),
    '#default_value' => variable_get($name, 30),
    '#size' => 4,
    '#description' => t('If recordings are automatically deleted, how many days should they be stored on Twilio before being purged?'),
  );

  return system_settings_form($form);
}