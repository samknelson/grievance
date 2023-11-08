<?php

function sirius_email_test_form($form, &$form_state) {
  $form['#tree'] = TRUE; 

  $form['message']['to'] = array(
    '#type' => 'textfield',
    '#title' => 'To',
  );

  $form['message']['headers']['cc'] = array(
    '#type' => 'textfield',
    '#title' => 'Cc',
  );

  $form['message']['headers']['bcc'] = array(
    '#type' => 'textfield',
    '#title' => 'Bcc',
  );

  $form['message']['reply-to'] = array(
    '#type' => 'textfield',
    '#title' => 'Reply-To',
  );

  $form['message']['subject'] = array(
    '#type' => 'textfield',
    '#title' => 'Subject',
  );

  $form['message']['body'] = array(
    '#type' => 'textarea',
    '#title' => 'Body',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Submit',
  );

  return $form;
}

function sirius_email_test_form_submit($form, &$form_state) {
  $form_state['rebuild'] = TRUE;
  $params = $form_state['values']['message'];
  $result = sirius_email_send($params);
  if ($result['success']) {
    drupal_set_message("Email sent.");
  } else {
    $msg = htmlspecialchars($result['msg']);
    drupal_set_message($msg, 'error');
  }

  if ($result['comm']) {
    $path = 'node/' . $result['comm']->nid . '/sirius_log_comm';
    $msg = "The mail message can be viewed and tracked <strong>" . l('here', $path) . '</strong>';
    drupal_set_message($msg);
    sirius_minilog($msg);
  }
}
