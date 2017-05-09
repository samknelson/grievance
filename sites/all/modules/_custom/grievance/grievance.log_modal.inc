<?php

function grievance_log_modal_add($node) {
	return drupal_get_form('grievance_log_modal_add_form', $grievance_node);
}

function grievance_log_modal_add_form($form, &$form_state, $grievance_node) {
	$form_state['grievance_node'] = $grievance_node;

  // dismiss if the form was submitted via a modal
  if($form_state['input']['js'] && $form_state['submitted']) {
    ctools_include('modal');
    ctools_include('ajax');
    $commands = array();
    $commands[] = ctools_modal_command_dismiss();
    print ajax_render($commands);
    drupal_exit();
  }

  drupal_set_title($grievance_node->title . ' - Add Log Entry');
  
  $form = array();
  $form['notes'] = array(
    '#type'          => 'textarea',
    '#default_value' => $node->field_coach_notes[LANGUAGE_NONE][0]['value'],
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Save',
  );

  return $form;
}

function grievance_log_modal_add_form_submit($form, &$form_state) {
	drupal_set_message("Log message created.");
}