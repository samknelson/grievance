<?php


function sirius_log_quicklog_config() {
  return drupal_get_form('sirius_log_quicklog_config_form');
}

function sirius_log_quicklog_config_form($form, &$form_state) {

  $content_types = array('sirius_worker');
  foreach ($content_types as $content_type) {
    $form[$content_type] = array(
      '#type' => 'fieldset',
      '#title' => t('Settings: ' . sirius_content_type_name($content_type)),
    );

    $name = sirius_domain_variable_name("sirius_log_quicklog_${content_type}_options");
    $form[$content_type][$name] = array(
      '#type' => 'textarea',
      '#title' => t('Quicklog Options'),
      '#description' => t('Enter the options, one row per option. Each option should be of the form category|type|title, i.e. call|left_message|Called: Left Message'),
      '#default_value' => variable_get($name, ''),
    );

    $name = sirius_domain_variable_name("sirius_log_quicklog_${content_type}_bucketview");
    $form[$content_type][$name] = array(
      '#type' => 'select',
      '#title' => t('Breakout?'),
      '#description' => t('Should the quicklog be broken out as a separate tab under "Log"?'),
      '#default_value' => variable_get($name, ''),
      '#options' => array(
        NULL => t('-- Select --'),
        'No' => t('No'),
        'Yes' => t('Yes'),
      ),
    );
  }

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
    if (!$title) { $title = "$category - $type"; }
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
  $ct_options = sirius_log_quicklog_category_type_options($node->type);
  $ct_tree = array();
  foreach ($ct_options as $ct => $title) {
    list ($category, $type) = explode('|', $ct);
    $ct_tree[$category][$type] = $title;
  }

  $category_options = array_combine(array_keys($ct_tree), array_keys($ct_tree));
  $form['category'] = array(
    '#type' => 'select',
    '#title' => t('Category'),
    '#required' => TRUE,
    '#options' => $category_options,
    '#default_value' => $category_options[array_key_first($category_options)],
    '#id' => 'quicklog_category'
  );

  foreach ($ct_tree as $category => $type_options) {
    $form["type_$category"] = array(
      '#type' => 'select',
      '#title' => t('Type'),
      '#required' => TRUE,
      '#options' => $type_options,
      '#default_value' => $type_options[array_key_first($type_options)],
      '#states' => array('visible' => array(array(":input[id=\"quicklog_category\"]" => array('value' => $category)))),
    );
  }

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
  $comment = $form_state['values']['reason'];
  $category = $form_state['values']['category'];
  $type = $form_state['values']["type_$category"];
  sirius_log($category, $type, $comment, $comment, NULL, array($node->nid), array());
  drupal_set_message("Log entry created: $category, $type.");
}
