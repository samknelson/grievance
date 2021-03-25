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

  return system_settings_form($form);
}
