<?php

// @todo - move 'test' into its own include file.

function grievance_timss_test() {
  return drupal_get_form('grievance_timss_test_form');
}

function grievance_timss_test_form($form, &$form_state) {
  $form['intro'] = array(
    '#type' => 'markup',
    '#markup' => '<div>' . t('Use the form below to test your TIMSS connection settings.') . '<br><br></div>',
  );

  $broughtby_vocabulary = taxonomy_vocabulary_machine_name_load('grievance_broughtby');
  $broughtby_terms = taxonomy_get_tree($broughtby_vocabulary->vid);
  $broughtby_terms[] = (object) array(
    'tid' => 'DEFAULT',
    'name' => t('Default')
  );

  $broughtby_options = array('DEFAULT' => t('Default'));
  foreach ($broughtby_terms as $broughtby_term) {
    $broughtby_options[$broughtby_term->tid] = $broughtby_term->name;
  }

  $form['broughtby_tid'] = array(
    '#type' => 'select',
    '#title' => t('TIMSS Connection (Brought By)'),
    '#options' => $broughtby_options,
    '#required' => TRUE,
  );

  $form['str'] = array(
    '#type' => 'textfield',
    '#title' => t('Search String'),
    '#required' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Submit TIMSS Test',
  );

  return $form;
}

function grievance_timss_test_form_submit($form, &$form_state) {
  $form_state['rebuild'] = TRUE;

  drupal_set_message("About to test TIMSS connection.");
  $broughtby_tid = $form_state['values']['broughtby_tid'];
  drupal_set_message("Using connection for \"Brough By\" TID = $broughtby_tid");

  drupal_set_message("URL = " . variable_get('grievance_timss_broughtby_' . $broughtby_tid . '_url'));
  drupal_set_message("User = " .  variable_get('grievance_timss_broughtby_' . $broughtby_tid . '_user'));
  drupal_set_message("Password = " . variable_get('grievance_timss_broughtby_' . $broughtby_tid . '_pass'));

  drupal_set_message("Attempting TIMSS login ...");
  $token = grievance_timss_login($form_state['values']['broughtby_tid']);
  if (!$token) { return; }
  drupal_set_message("TIMSS login was successful, and the token returned was: <pre>" . print_r($token, 1) . '</pre>');

  $str = $form_state['values']['str'];
  
  drupal_set_message("About to search: [$str]");

  $response = grievance_timss_search($str, $broughtby_tid, TRUE);

  if ($response) {
    drupal_set_message("Response was:<pre>" . wordwrap(print_r($response, 1)) . "</pre>");
  }

}

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

  // Add our own submit handler
  $form['#submit'][] = 'grievance_timss_admin_submit';

  return system_settings_form($form);
}

function grievance_timss_admin_submit($form, &$form_state) {
  // A little hack. When form is presented, the password is not shown (Drupal
  // way of doing). So, if user submits the form without changing the password,
  // we must prevent it from being reset.

  $broughtby_vocabulary = taxonomy_vocabulary_machine_name_load('grievance_broughtby');
  $broughtby_terms = taxonomy_get_tree($broughtby_vocabulary->vid);
  $broughtby_terms[] = (object) array(
    'tid' => 'DEFAULT',
    'name' => t('Default')
  );
  foreach ($broughtby_terms as $broughtby_term) {
    $prefix = 'grievance_timss_broughtby_' . $broughtby_term->tid ;
    if (empty($form_state['values']["${prefix}_pass"])) {
      unset($form_state['values']["${prefix}_pass"]);
    }
  }
}