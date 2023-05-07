<?php

function sirius_twilio_config() {
  $form = array();

  /*
  $name = sirius_domain_variable_name('sirius_twilio_provider');
  $form[$name] = array(
    '#type' => 'select',
    '#title' => t('Provider'),
    '#options' => array(
      'twilio' => t('Twilio'),
      'signalwire' => t('Signalwire'),
    ),
    '#default_value' => variable_get($name, 'twilio'),
    '#description' => t('Select the telelphony service provider.'),
  );
  */

  $name = sirius_domain_variable_name('sirius_twilio_phone');
  $form[$name] = array(
    '#type' => 'textfield',
    '#title' => t('Phone Number'),
    '#default_value' => variable_get($name, ''),
    '#description' => t('Phone number in E164 format (i.e. "+15551234567". This must be a phone number provisioned at Twilio. It will be the default phone number used for outgoing calls (when no caller ID is selected) and will be used for all outgoing SMS messages.'),
  );

  $name = sirius_domain_variable_name('sirius_twilio_account_sid');
  $form[$name] = array(
    '#type' => 'textfield',
    '#title' => t('Account SID'),
    '#default_value' => variable_get($name, ''),
  );

  $name = sirius_domain_variable_name('sirius_twilio_account_token');
  $form[$name] = array(
    '#type' => 'textfield',
    '#title' => t('Account Token'),
    '#default_value' => variable_get($name, ''),
  );

  /*
  $name = sirius_domain_variable_name('sirius_twilio_signalwire_space_url');
  $form[$name] = array(
    '#type' => 'textfield',
    '#title' => t('Space URL'),
    '#default_value' => variable_get($name, ''),
    '#description' => t('The SignalWire "Space URL", e.g. yourspace.signalwire.com.'),
    '#states' => array('visible' => array(array(":input[name=\"" . sirius_domain_variable_name('sirius_twilio_provider') . "\"]" => array('value' => 'signalwire')))),
  );
  */

  $name = sirius_domain_variable_name('sirius_twilio_app_sid');
  $form[$name] = array(
    '#type' => 'textfield',
    '#title' => t('App SID'),
    '#default_value' => variable_get($name, ''),
  );

  $name = sirius_domain_variable_name('sirius_twilio_site_name_tts');
  $form[$name] = array(
    '#type' => 'textfield',
    '#title' => t('Site Name (TTS)'),
    '#default_value' => variable_get($name, ''),
    '#description' => t('Site name as it should be spoken (TTS format)'),
  );

  $name = sirius_domain_variable_name('sirius_twilio_twiml_open');
  $form[$name] = array(
    '#type' => 'textfield',
    '#title' => t('TWIML Open'),
    '#default_value' => variable_get($name, '<Say><prosody>'),
    '#description' => t('Enter the default TWIML wrapper. This allows you to specify a voice and a prosody call.'),
  );

  $name = sirius_domain_variable_name('sirius_twilio_twiml_close');
  $form[$name] = array(
    '#type' => 'textfield',
    '#title' => t('TWIML Close'),
    '#default_value' => variable_get($name, '</prosody></Say>'),
  );

  $name = sirius_domain_variable_name('sirius_twilio_sms_consolidate');
  $form[$name] = array(
    '#type' => 'select',
    '#title' => t('Consolidate Outgoing Messages'),
    '#options' => array(
      FALSE => t('No'),
      TRUE => t('Yes'),
    ),
    '#default_value' => variable_get($name, FALSE),
    '#description' => t('Sometimes multiple SMS messages are sent within the same page load (i.e. "you have been laid off" and "you have been signed out". Select "Yes" to consolidate these into a single SMS messages.'),
  );

  $name = sirius_domain_variable_name('sirius_twilio_sms_unsubscribe');
  $form[$name] = array(
    '#type' => 'select',
    '#title' => t('Include unsubscribe instructions in all SMS messages?'),
    '#options' => array(
      FALSE => t('No'),
      TRUE => t('Yes'),
    ),
    '#default_value' => variable_get($name, TRUE),
    '#description' => t('If "Yes", then unsubscribe instructions will be appended to EVERY SMS message sent by the system.'),
  );

  $form['sirius_twilio_lookup'] = array(
    '#type' => 'fieldset',
    '#title' => t('Lookups'),
    '#description' => t('Lookups perform a line test before sending to each phone number. (Results are cached and only re-checked once a month.) They involve a certain amount of computational overhead, and there is a small charge from the service provider. However, they can prevent messages from being sent to invalid phone numbers.'),
  );

  $name = sirius_domain_variable_name('sirius_twilio_lookup_enable_sms');
  $form['sirius_twilio_lookup'][$name] = array(
    '#type' => 'select',
    '#title' => t('Enable Lookups for SMS?'),
    '#options' => array(
      FALSE => t('No'),
      TRUE => t('Yes'),
    ),
    '#default_value' => variable_get($name, FALSE),
  );

  $name = sirius_domain_variable_name('sirius_twilio_lookup_enable_voice');
  $form['sirius_twilio_lookup'][$name] = array(
    '#type' => 'select',
    '#title' => t('Enable Lookups for Voice?'),
    '#options' => array(
      FALSE => t('No'),
      TRUE => t('Yes'),
    ),
    '#default_value' => variable_get($name, FALSE),
  );

  $domain_nid = sirius_domain_nid();
  if ($domain_nid < 1) {
    $name = sirius_domain_variable_name('sirius_twilio_operator');
    $form[$name] = array(
      '#type' => 'textfield',
      '#title' => t('(Legacy) Main Operator Number'),
      '#default_value' => variable_get($name, FALSE),
      '#description' => t('The main operator number supports legacy code, and should not be used for new installations. Please use the office list for all new installations.'),
    );
  }

  return system_settings_form($form);
}

function sirius_twilio_amd_setting_names() {
  return array(
    'MachineDetectionTimeout',
    'MachineDetectionSpeechThreshold',
    'MachineDetectionSpeechEndThreshold',
    'MachineDetectionSilenceTimeout'
  ); 
}

function sirius_twilio_config_amd() {
  $form = array();

  $names = sirius_twilio_amd_setting_names();
  foreach ($names as $raw_name) {
    $name = sirius_domain_variable_name($raw_name);
    $form[$name] = array(
      '#type' => 'textfield',
      '#title' => $raw_name,
      '#default_value' => variable_get($name, ''),
      '#size' => 6,
    );
  }

  return system_settings_form($form);
}




