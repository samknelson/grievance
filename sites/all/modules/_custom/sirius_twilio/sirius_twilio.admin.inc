<?php

function sirius_twilio_config() {
  $form = array();

  $form['sirius_twilio_phone'] = array(
    '#type' => 'textfield',
    '#title' => t('Phone Number'),
    '#default_value' => variable_get('sirius_twilio_phone', ''),
    '#description' => t('Phone number in E164 format (i.e. "+15551234567".'),
  );

  $form['sirius_twilio_account_sid'] = array(
    '#type' => 'textfield',
    '#title' => t('Account SID'),
    '#default_value' => variable_get('sirius_twilio_account_sid', ''),
  );

  $form['sirius_twilio_account_token'] = array(
    '#type' => 'textfield',
    '#title' => t('Account Token'),
    '#default_value' => variable_get('sirius_twilio_account_token', ''),
  );

  $form['sirius_twilio_app_sid'] = array(
    '#type' => 'textfield',
    '#title' => t('App SID'),
    '#default_value' => variable_get('sirius_twilio_app_sid', ''),
  );

  $form['sirius_twilio_operator'] = array(
    '#type' => 'textfield',
    '#title' => t('Operator Phone Number'),
    '#default_value' => variable_get('sirius_twilio_operator', ''),
    '#description' => t('Phone number in E164 format (i.e. "+15551234567".'),
  );

  return system_settings_form($form);
}

