<?php

/***
 * Implements hook_permission
 */

function sirius_training_permission() {
  return array(
    'sirius training admin' => array(
      'title' => t('Sirius Training Admin'), 
      'description' => t('Administer all aspects of the training program'),
    ),
    'sirius training staff' => array(
      'title' => t('Sirius Training Staff'), 
      'description' => t('Access to use the training prorram as a staff member'),
    ),
    'sirius training certification view' => array(
      'title' => t('Sirius Training Certification View'), 
      'description' => t('Ability to view any certification'),
    ),
    'sirius training certification edit' => array(
      'title' => t('Sirius Training Certification Edit'), 
      'description' => t('Ability to edit or grant any certification'),
    ),
  );
}

/**
 * Implements hook_node_access
 */

function sirius_training_node_access($node, $op, $account) {
  // Get the node type
  if (is_object($node)) { $type = $node->type; } else { $type = $node; }

  // We know nothing.
  return NODE_ACCESS_IGNORE;
}
