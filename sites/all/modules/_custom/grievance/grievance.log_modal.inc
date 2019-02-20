<?php

function grievance_log_modal_add($grievance_node) {
	return drupal_get_form('grievance_log_modal_add_form', $grievance_node);
}

function grievance_log_modal_add_form($form, &$form_state, $grievance_node) {
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

  drupal_set_title($grievance_node->title . ' - Add Log Entry');
  
  $form = array();
  $form_state['grievance_node'] = $grievance_node;

  if ($grievance_node->field_grievance_phone['und'][0]['value'] || $grievance_node->field_grievance_email['und'][0]['email']) {
    $headfoot = t('(Grievant: ');
    if ($grievance_node->field_grievance_phone['und'][0]['value']) {
      $headfoot .= '<strong>' . $grievance_node->field_grievance_phone['und'][0]['value'] . '</strong>';
    }
    if ($grievance_node->field_grievance_phone['und'][0]['value'] && $grievance_node->field_grievance_email['und'][0]['email']) {
      $headfoot .= ' / ';
    }
    if ($grievance_node->field_grievance_email['und'][0]['email']) {
      $headfoot .= '<strong>' . $grievance_node->field_grievance_email['und'][0]['email'] . '</strong>';
    }
    $headfoot .= ')<br><br>';
    $form['headfoot'] = array(
      '#type' => 'markup',
      '#markup' => $headfoot,
    );
  }

  $form['grievance_log_type'] = array(
    '#type'           => 'select',
    '#title'          => 'Type',
    '#options'        => grievance_log_types_i_can_edit(),
    '#required'       => TRUE,
    '#default_value'  => variable_get('grievance_log_modal_default_type', NULL),
  );


  if (taxonomy_allowed_values(field_info_field('field_grievance_log_tags'))) {
    $form['grievance_log_tags'] = array(
      '#type'           => 'checkboxes',
      '#title'          => 'Tags',
      '#options'        => taxonomy_allowed_values(field_info_field('field_grievance_log_tags')),
    );
  }

  $form['grievance_status_date'] = array(
    '#type'           => 'date_popup',
    '#title'          => 'Date',
    '#required'       => TRUE,
    '#date_format'    => 'm/d/Y H:i',
    '#date_label_position'  => 'none',
    '#default_value'  =>  date('Y-m-d H:i'),
  );

  $form['body'] = array(
    '#type'           => 'textarea',
    '#title'          => 'Body',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Save',
  );

  return $form;
}

function grievance_log_modal_add_form_submit($form, &$form_state) {
  $form_state['rebuild'] = TRUE;
  $form_state['submitted'] = TRUE;

  $grievance_node = $form_state['grievance_node'];
  $log_type_term = taxonomy_term_load($form_state['values']['grievance_log_type']);
  global $user;

  $log_node = new stdClass();
  $log_node->title = $log_type_term->name;
  $log_node->type = 'grievance_log';
  node_object_prepare($log_node);
  $log_node->language = LANGUAGE_NONE;
  $log_node->uid = $user->uid; 
  $log_node->status = 1;
  $log_node->promote = 0;
  $log_node->comment = 0;

  $log_node->field_grievance['und'][0]['target_id'] = $grievance_node->nid;
  $log_node->field_grievance_log_type['und'][0]['tid'] = $form_state['values']['grievance_log_type'];
  if ($form_state['values']['grievance_log_tags']) {
    $log_node->field_grievance_log_tags['und'] = array();
    foreach ($form_state['values']['grievance_log_tags'] as $tid => $value) {
      if ($value) {
        $log_node->field_grievance_log_tags['und'][] = array('tid' => $tid);
      }
    }
  }

  $log_node->field_grievance_status_date['und'][0]['value'] = date('Y-m-d H:i:s', strtotime($form_state['values']['grievance_status_date']));
  $log_node->body['und'][0]['value'] = $form_state['values']['body'];

  $log_node = node_submit($log_node); // Prepare node for saving
  node_save($log_node);

  drupal_set_message(t('Log entry added.'));

  return;
}


