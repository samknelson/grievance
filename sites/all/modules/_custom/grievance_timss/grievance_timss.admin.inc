<?php

function grievance_timss_admin() {

  $broughtby_vocabulary = taxonomy_vocabulary_machine_name_load('grievance_broughtby');
  $broughtby_terms = taxonomy_get_tree($broughtby_vocabulary->vid);
  $broughtby_terms[] = (object) array(
    'tid' => 'DEFAULT',
    'name' => t('Default')
  );

  foreach ($broughtby_terms as $broughtby_term) {
    $prefix = 'grievance_timss_broughtby_' . $broughtby_term->tid ;

    $form[$prefix] = array(
      '#type' => 'fieldset',
      '#title' => $broughtby_term->name,
    );

    $form[$prefix]["${prefix}_url"] = array(
      '#type' => 'textfield',
      '#default_value' => variable_get("${prefix}_url", ''),
      '#title' => t('TIMSS URL'),
    );

    $form[$prefix]["${prefix}_user"] = array(
      '#type' => 'textfield',
      '#default_value' => variable_get("${prefix}_user", ''),
      '#title' => t('TIMSS User'),
    );

    $form[$prefix]["${prefix}_pass"] = array(
      '#type' => 'password',
      '#default_value' => variable_get("${prefix}_pass", ''),
      '#title' => t('TIMSS Password'),
      '#description'   => t('TIMSS password. If you have already entered your password before, you should leave this field blank, unless you want to change the stored password.'),
    );
  }

  $form['#submit'][] = 'grievance_timss_admin_submit';
  return system_settings_form($form);
}

function grievance_timss_admin_submit($form, &$form_state) {
  // A little hack. When form is presentend, the password is not shown (Drupal
  // way of doing). So, if user submits the form without changing the password,
  // we must prevent it from being reset.

  $broughtby_vocabulary = taxonomy_vocabulary_machine_name_load('grievance_broughtby');
  $broughtby_terms = taxonomy_get_tree($broughtby_vocabulary->vid);
  foreach ($broughtby_terms as $broughtby_term) {
    $prefix = 'grievance_timss_broughtby_' . $broughtby_term->tid ;
    if (empty($form_state['values']["${prefix}_pass"])) {
      unset($form_state['values']["${prefix}_pass"]);
    }
  }
}