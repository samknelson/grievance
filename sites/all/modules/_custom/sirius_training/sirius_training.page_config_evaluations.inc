<?php

function sirius_training_config_evaluations($form, &$form_state) {
  $name = sirius_domain_variable_name('sirius_training_evaluations_enabled');
  $form[$name] = array(
    '#type' => 'select',
    '#title' => t('Evaluations Enabled?'),
    '#options' => array(
      'No' => t('No'),
      'Yes' => t('Yes'),
    ),
    '#description' => t('Select "Yes" to enable worker evaluations.'),
    '#default_value' => variable_get($name, ''),
  );


  $name = sirius_domain_variable_name('sirius_training_evaluations_roles');
  $roles = user_roles();
  $form[$name] = array(
    '#type' => 'checkboxes',
    '#title' => t('Roles'),
    '#options' => $roles,
    '#description' => t('List the roles which have permission to submit an evaluation.'),
    '#default_value' => variable_get($name, ''),
  );

  /*
  $name = sirius_domain_variable_name('sirius_training_evaluations_pdf');
  $fid_name = sirius_domain_variable_name('sirius_training_evaluations_pdf_fid');
  $prefix_html = '';
  if ($fid = variable_get($fid_name, '')) {
    $file = file_load($fid);
    $url = file_create_url($file->uri);
    $prefix_html = "Current file: <a href=\"$url\" target=\"_blank\">Download</a>";
    $title = 'Replace Worker Evaluation Form';
  } else {
    $title = 'Upload Worker Evaluation Form';
  }
  $form[$name] = array(
      '#type' => 'file',
      '#title' => $title,
      '#prefix' => $prefix_html,
  ); 
  $form['#validate'][] = 'sirius_training_config_evaluations_form_validate';
  $form['#submit'][] = 'sirius_training_config_evaluations_form_submit';
  */

  $name = sirius_domain_variable_name('sirius_training_evaluations_email');
  $form[$name] = array(
    '#type' => 'textfield',
    '#title' => t('Send Evaluations To'),
    '#description' => t('Evaluations should be sent to this address.'),
    '#default_value' => variable_get($name, ''),
  );

  $name = sirius_domain_variable_name('sirius_training_evaluations_form_intro');
  $default = variable_get($name, array());
  $form[$name] = array(
    '#type' => 'text_format',
    '#title' => t('Evaluation Form Introductory Text'),
    '#default_value' => $default['value'],
    '#format' => $default['format'],
    '#description' => t('Introductory text to appear at the top of the evaluation form.'),
  );

  $form['request_template'] = array(
    '#type' => 'fieldset', 
    '#title' => 'Request Template',
    '#description' => 'Use the form below to specify the evaluation request template which a worker may send to an evaluator. Use the tokens' . 
      '<ul>' .
      '<li>[worker_node:title] (the worker\'s name)</li>' .
      '<li>[sirius:request:name] (the evaluator\'s name)</li>' .
      '<li>[sirius:request:comments] (the workers\'s comments)</li>' .
      '<li>[sirius:response:email] (the email address to send the response to)</li>' .
      '<li>[sirius:response:evaluate_url] (the address of the evaluation form)</li>' .
      '</ul>',
  );

  $name = sirius_domain_variable_name('sirius_training_evaluations_request_email_subject');
  $form['request_template'][$name] = array(
    '#type' => 'textfield',
    '#title' => t('Request Email Subject'),
    '#default_value' => variable_get($name, ''),
  );

  $name = sirius_domain_variable_name('sirius_training_evaluations_request_email_body');
  $default = variable_get($name, array());
  $form['request_template'][$name] = array(
    '#type' => 'text_format',
    '#title' => t('Request Email Body'),
    '#default_value' => $default['value'],
    '#format' => $default['format'],
  );

  $name = sirius_domain_variable_name('sirius_training_evaluations_request_sms');
  $default = variable_get($name, array());
  $form['request_template'][$name] = array(
    '#type' => 'textarea',
    '#title' => t('Request SMS Body'),
    '#default_value' => variable_get($name, ''),
  );

  return system_settings_form($form);
}

function sirius_training_config_evaluations_edit() {
  $domain_nid = sirius_domain_nid();
  $definition_nid = sirius_contact_definition_get_nid($domain_nid, TRUE);
  return drupal_get_form('sirius_json_definition_edit_form', $definition_nid, NULL, 'default_evaluation_form');
}

function sirius_training_config_evaluations_preview() {
  $domain_nid = sirius_domain_nid();
  $definition_nid = sirius_contact_definition_get_nid($domain_nid, TRUE);
  return drupal_get_form('sirius_json_definition_preview_form', $definition_nid, 'default_evaluation_form');
}

function sirius_training_config_evaluations_exim() {
  $domain_nid = sirius_domain_nid();
  $definition_nid = sirius_contact_definition_get_nid($domain_nid, TRUE);
  return drupal_get_form('sirius_json_definition_exim_form', $definition_nid, 'default_evaluation_form');
}


/*
function sirius_training_config_evaluations_form_validate($form, &$form_state) {
  $name = sirius_domain_variable_name('sirius_training_evaluations_pdf');
  $file = file_save_upload($name, array('file_validate_extensions' => array('pdf')));
  $domain_nid = sirius_domain_nid();
  $file_name = 'evaluation_form';
  if ($domain_nid) { $file_name .= '_' . $domain_nid; }
  $file_name .= '.pdf';
  if ($file) {
    if ($file = file_move($file, "public://sirius/training/$file_name", FILE_EXISTS_REPLACE)) {
      $form_state['storage'][$name] = $file;
    }
    else {
      form_set_error($name, t("Failed to write the uploaded file to the site's file folder."));
    }
  }
}

function sirius_training_config_evaluations_form_submit($form, &$form_state) {
  $name = sirius_domain_variable_name('sirius_training_evaluations_pdf');
  $fid_name = sirius_domain_variable_name('sirius_training_evaluations_pdf_fid');
  $file = $form_state['storage'][$name];
  if ($file) {
    unset($form_state['storage'][$name]);
    unset($file->destination);
    unset($file->uri);
    $file->display = 1;
    $file->description = 'Worker evaluation form';
    $file->status = FILE_STATUS_PERMANENT;
    file_save($file);
    variable_set($fid_name, $file->fid);
  }
}

function sirius_training_evaluation_pdf_download() {
  $fid_name = sirius_domain_variable_name('sirius_training_evaluations_pdf_fid');
  $fid = variable_get($fid_name, '');
  if (!$fid) { return 'No evaluation form has been configured.'; }
  $file = file_load($fid);
  $realpath = drupal_realpath($file->uri);

  drupal_add_http_header('Content-Type', 'application/pdf');
  drupal_add_http_header('Content-Disposition', 'attachment;filename=evaluation_form.pdf');
  $fp = fopen('php://output', 'w');
  print file_get_contents($realpath);
  exit();
}
*/