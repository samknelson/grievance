<?php

function sirius_worker_page_skill_settings() {
	return drupal_get_form('sirius_worker_page_skill_settings_form');
}

function sirius_worker_page_skill_settings_form($form, &$form_state) {
  $name = sirius_domain_variable_name('sirius_worker_skill_never_expires_text');
  $form[$name] = array(
    '#type' => 'textfield',
    '#title' => t('"Never Expires" text'),
    '#default_value' => variable_get($name, 'Never Expires'),
    '#description' => t('Text to display when a skill never expires.'),
  );

  $form['expiration_reminders'] = array(
    '#type' => 'fieldset',
    '#title' => 'Expiration Reminders',
  );

  /*
  $name = sirius_domain_variable_name('sirius_worker_skill_expiration_reminders_active');
  $form['expiration_reminders'][$name] = array(
    '#type' => 'select',
    '#title' => t('Send Expiration Reminders?'),
    '#default_value' => variable_get($name, ''),
    '#options' => array(
      'No' => t('No'),
      'Yes' => t('Yes'),
    ),
  );
  */

  // @todo: This is NOT domain specific!!!
  $name = 'sirius_worker_skill_expiration_reminders_suffix';
  $form['expiration_reminders'][$name] = array(
    '#type' => 'textarea',
    '#title' => t('Expiration Reminder Text'),
    '#default_value' => variable_get($name, ''),
    '#description' => t('Enter the text that should accompany skill expiration reminders, e.g. "Please contact the Training Center at 555-123-4567 to find out how to renew your certifications.'),
  );

  return system_settings_form($form);
}
