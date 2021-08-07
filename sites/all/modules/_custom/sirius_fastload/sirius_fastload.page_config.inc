<?php

function sirius_fastload_page_config($form, &$form_state) {
  $form['sirius_fastload_enabled'] = array(
    '#type' => 'select',
    '#options' => array('No' => t('No'), 'Yes' => t('Yes')),
    '#title' => 'FastLoad Enabled?',
    '#default_value' => variable_get('sirius_fastload_enabled', ''),
    '#description' => t('Is FastLoad enabled for this site?'),
  );

  $form['sirius_fastload_buffer_size'] = array(
    '#type' => 'textfield',
    '#title' => 'Buffer Size',
    '#default_value' => variable_get('sirius_fastload_buffer_size', ''),
    '#description' => t('Enter the number of available FastLoad nodes to maintain.'),
    '#size' => 6,
    '#default_value' => variable_get('sirius_fastload_buffer_size', ''),
  );

  $form['sirius_fastload_batch_size'] = array(
    '#type' => 'textfield',
    '#title' => 'Batch Size',
    '#default_value' => variable_get('sirius_fastload_batch_size', ''),
    '#description' => t('Enter the number of FastLoad nodes that will be generated in a given "batch".'),
    '#size' => 6,
    '#default_value' => variable_get('sirius_fastload_batch_size', '1000'),
  );

  $form['#validate'][] = 'sirius_fastload_page_config_validate';

  return system_settings_form($form);
}

function sirius_fastload_page_config_validate($form, &$form_state) {
  $buffer_size = $form_state['values']['sirius_fastload_buffer_size'];
  if (!is_numeric($buffer_size)) {
    form_set_error('sirius_fastload_buffer_size', 'Please enter a number.');
  } else if ($buffer_size < 0 || $buffer_size > 100000000) {
    form_set_error('sirius_fastload_buffer_size', 'Please enter a number between 0 and 100,000,000.');
  }

  $batch_size = $form_state['values']['sirius_fastload_batch_size'];
  if (!is_numeric($batch_size)) {
    form_set_error('sirius_fastload_batch_size', 'Please enter a number.');
  } else if ($batch_size < 0 || $batch_size > 100000) {
    form_set_error('sirius_fastload_batch_size', 'Please enter a number between 0 and 100,000.');
  }

}