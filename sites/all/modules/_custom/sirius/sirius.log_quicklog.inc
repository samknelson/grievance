<?php


function sirius_log_quicklog_config() {
  return drupal_get_form('sirius_log_quicklog_config_form');
}

function sirius_log_quicklog_config_form($form, &$form_state) {
  $name = sirius_domain_variable_name('sirius_log_quicklog_sirius_worker_options');
  $form[$name] = array(
    '#type' => 'textarea',
    '#title' => t('Quicklog Worker Options'),
    '#description' => t('Enter the worker options, one row per option. Each option should be of the form category|type|title, i.e. call|left_message|Called: Left Message'),
    '#default_value' => variable_get($name, ''),
  );

  return system_settings_form($form);
}

function sirius_log_quicklog_category_type_options($content_type) {
  $str = sirius_domain_variable_get('sirius_log_quicklog_' . $content_type . '_options', 'comment|comment|Comment');
  $lines = explode("\n", $str);
  $options = array();

  $my_excluded_categories = sirius_log_my_excluded_categories();
  
  foreach ($lines as $line) {
    list ($category, $type, $title) = explode('|', $line);
    if (in_array($category, $my_excluded_categories)) { continue; }
    $category = trim($category);
    $type = trim($type);
    $title = trim($title);
    if ($category && $type && $title) {
      $options["$category|$type"] = $title;
    }
  }
  return $options;
}

function sirius_log_quicklog_page_add($node) {
  return drupal_get_form('sirius_log_quicklog_page_add_form', $node);
}

function sirius_log_quicklog_page_add_form($form, &$form_state, $node) {
  // dismiss if the form was submitted via a modal
  if($form_state['input']['js'] && $form_state['submitted']) {
    ctools_include('modal');
    ctools_include('ajax');
    $commands = array();
    $commands[] = ctools_modal_command_dismiss();

    $commands[] = ajax_command_replace('#messages-wrapper', 
      '<div id="messages-wrapper">' .
      '<div id="messages">' . 
      theme_status_messages(array()) .
      '</div>' .
      '</div>'
    );

    print ajax_render($commands);
    drupal_exit();
  }

  $form_state['nid'] = $node->nid;
  $category_type_options = sirius_log_quicklog_category_type_options($node->type);

  $form['category_type'] = array(
    '#title' => t('Entry Type'),
    '#type' => 'select',
    '#options' => $category_type_options,
    '#required' => TRUE,
    '#suffix' => '<br clear="all">',
  );

  $form['reason'] = array(
    '#type' => 'textarea',
    '#title' => t('Comment'),
    '#required' => TRUE,
    '#rows' => 18,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  return $form;
}

function sirius_log_quicklog_page_add_form_submit($form, &$form_state) {
  $form_state['rebuild'] = TRUE;
  $form_state['submitted'] = TRUE;
  $node = node_load($form_state['nid']);
  $category_type_options = sirius_log_quicklog_category_type_options($node->type);
  $comment = $form_state['values']['reason'];
  list ($category, $type) = explode('|', $form_state['values']['category_type']);
  sirius_log($category, $type, $comment, $comment, NULL, array($node->nid), array());
  drupal_set_message('Log entry created: ' . $category_type_options["$category|$type"]);
}
