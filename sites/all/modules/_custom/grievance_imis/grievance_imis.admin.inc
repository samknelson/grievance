<?php

// @todo - move 'test' into its own include file.

function grievance_imis_test() {
  return drupal_get_form('grievance_imis_test_form');
}

function grievance_imis_test_form($form, &$form_state) {
  $form['intro'] = array(
    '#type' => 'markup',
    '#markup' => '<div>' . t('Use the form below to test your IMIS connection settings.') . '<br><br></div>',
  );

  $broughtby_options = sirius_taxonomy_load('grievance_broughtby', 'tid', 'name', TRUE);
  $broughtby_options['DEFAULT'] = t('Default');

  $form['broughtby_tid'] = array(
    '#type' => 'select',
    '#title' => t('IMIS Connection (Brought By)'),
    '#options' => $broughtby_options,
    '#required' => TRUE,
  );

  $form['which'] = array(
    '#type' => 'select',
    '#title' => t('Function'),
    '#options' => array('search' => 'Search', 'lookup' => 'Lookup'),
    '#required' => TRUE,
  );

  $form['str'] = array(
    '#type' => 'textfield',
    '#title' => t('Search String'),
    '#required' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Submit IMIS Test',
  );

  return $form;
}

function grievance_imis_test_form_submit($form, &$form_state) {
  $form_state['rebuild'] = TRUE;

  drupal_set_message("About to test IMIS connection.");
  $broughtby_tid = $form_state['values']['broughtby_tid'];
  $str = $form_state['values']['str'];
  $which = $form_state['values']['which'];
  drupal_set_message("Using connection for \"Brough By\" TID = $broughtby_tid");

  drupal_set_message("URL = " . variable_get('grievance_imis_broughtby_' . $broughtby_tid . '_url'));
  drupal_set_message("API Key = " . variable_get('grievance_imis_broughtby_' . $broughtby_tid . '_apikey'));

  drupal_set_message("About to search: [$str]");

  if ($which == 'search') {
    $response = grievance_imis_search($str, $broughtby_tid, TRUE);
  } else {
    $response = grievance_imis_lookup_raw($str, $broughtby_tid);
  }

  if ($response) {
    drupal_set_message("Response was:<pre>" . wordwrap(print_r($response, 1)) . "</pre>");
  }
}

function grievance_imis_admin() {
  $broughtby_vocabulary = taxonomy_vocabulary_machine_name_load('grievance_broughtby');
  $broughtby_terms = taxonomy_get_tree($broughtby_vocabulary->vid);
  $broughtby_terms[] = (object) array(
    'tid' => 'DEFAULT',
    'name' => t('Default')
  );

  foreach ($broughtby_terms as $broughtby_term) {
    $prefix = 'grievance_imis_broughtby_' . $broughtby_term->tid ;

    $form[$prefix] = array(
      '#type' => 'fieldset',
      '#title' => $broughtby_term->name,
    );

    $form[$prefix]["${prefix}_url"] = array(
      '#type' => 'textfield',
      '#default_value' => variable_get("${prefix}_url", ''),
      '#title' => t('IMIS URL'),
      '#description' => t('This will be something like: https://unite-test3.burstingsilver.com/BSi.iMIS.WebServices/api/member/grievance'),
    );

    $form[$prefix]["${prefix}_apikey"] = array(
      '#type' => 'textarea',
      '#title' => t('IMIS API Key'),
      '#description'   => t('IMIS API Key. If you have already entered your password before, you should leave this field blank, unless you want to change the stored key.'),
    );
  }

  // Add our own submit handler
  $form['#submit'][] = 'grievance_imis_admin_submit';

  return system_settings_form($form);
}

function grievance_imis_admin_submit($form, &$form_state) {
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
    $prefix = 'grievance_imis_broughtby_' . $broughtby_term->tid ;
    if (empty($form_state['values']["${prefix}_apikey"])) {
      unset($form_state['values']["${prefix}_apikey"]);
    }
  }
}