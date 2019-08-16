<?php

/**
 * Implements hook_action_info
 */

function grievance_action_info() {
  return array(
    'grievance_vbo_status_correct' => array(
      'type' => 'node',
      'label' => t('Grievance Status Correct'),
      'configurable' => TRUE,
      'vbo_configurable' => FALSE,
      'triggers' => array(),
    ),
  );
}

