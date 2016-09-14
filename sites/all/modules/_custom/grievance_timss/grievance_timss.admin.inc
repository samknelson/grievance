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
    '#type' => 'textfield',
    '#default_value' => variable_get('grievance_timss_pass', ''),
    '#title' => t('TIMSS Password'),
  );

  return system_settings_form($form);
}