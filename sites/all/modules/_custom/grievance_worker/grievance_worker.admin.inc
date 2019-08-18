<?php

// @todo - move 'test' into its own include file.

function grievance_worker_test() {
  return drupal_get_form('grievance_worker_test_form');
}

function grievance_worker_test_form($form, &$form_state) {
  $form['intro'] = array(
    '#type' => 'markup',
    '#markup' => '<div>' . t('Use the form below to test your local worker database connection.') . '<br><br></div>',
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

  $form['str'] = array(
    '#type' => 'textfield',
    '#title' => t('Search String'),
    '#required' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Submit Test',
  );

  return $form;
}

function grievance_worker_test_form_submit($form, &$form_state) {
  $form_state['rebuild'] = TRUE;

  if ($response) {
    drupal_set_message("Response was:<pre>" . wordwrap(print_r($response, 1)) . "</pre>");
  }

}

function grievance_worker_admin() {
  $form = array();
  return system_settings_form($form);
}
