<?php

/**
 * Implements hook_action_info
 */

function sirius_worker_action_info() {
  return array(
    'sirius_worker_vbo_usersync' => array(
      'type' => 'node',
      'label' => t('Sirius Worker / User Sync'),
      'configurable' => FALSE,
      'vbo_configurable' => FALSE,
      'triggers' => array(),
    ),
  );
}

function sirius_worker_vbo_usersync(&$node, $context) {
  sirius_worker_usersync($node->nid);
}

