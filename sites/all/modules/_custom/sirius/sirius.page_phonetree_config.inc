<?php

function sirius_page_phonetree_config() {
  return drupal_get_form('sirius_page_phonetree_config_form');
}

function sirius_page_phonetree_config_form($form, &$form_state) {
  $form['sirius_phonetree_text_worker_login_intro'] = array(
    '#type' => 'textarea',
    '#title' => t('Phonetree text: Worker login intro'),
    '#description' => t('This is the message that workers will hear when the first call the phonetree. It should include a prompt to log in by entering their worker ID.'),
    '#default_value' => sirius_phonetree_text('worker_login_intro'),
  );

  return system_settings_form($form);
}
