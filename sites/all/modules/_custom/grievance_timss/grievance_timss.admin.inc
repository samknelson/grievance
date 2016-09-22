<?php

function grievance_timss_admin() {
  $form = array();

  $form['grievance_timss_url'] = array(
    '#type' => 'textfield',
    '#default_value' => variable_get('grievance_timss_url', ''),
    '#title' => t('TIMSS URL'),
  );

  $form['grievance_timss_user'] = array(
    '#type' => 'textfield',
    '#default_value' => variable_get('grievance_timss_user', ''),
    '#title' => t('TIMSS User'),
  );

  $form['grievance_timss_pass'] = array(
    '#type' => 'password',
    '#default_value' => variable_get('grievance_timss_pass', ''),
    '#title' => t('TIMSS Password'),
    '#description'   => t('TIMSS password. If you have already entered your password before, you should leave this field blank, unless you want to change the stored password.'),
  );

  $form['#submit'][] = 'grievance_timss_admin_submit';
  return system_settings_form($form);
}

function grievance_timss_admin_submit($form, &$form_state) {
  // A little hack. When form is presentend, the password is not shown (Drupal
  // way of doing). So, if user submits the form without changing the password,
  // we must prevent it from being reset.

  if (empty($form_state['values']['grievance_timss_pass'])) {
    unset($form_state['values']['grievance_timss_pass']);
  }
}