<?php

/***
 * Implements hook_permission
 */

function sirius_denorm_permission() {
  return array(
    'sirius denorm admin' => array(
      'title' => t('Sirius Denormalize Admin'), 
      'description' => t('Permission to administer the denormalize system'),
    ),
    'sirius denorm user' => array(
      'title' => t('Sirius Denormalize User'), 
      'description' => t('Permission to use the denormalize system'),
    ),
  );
}

/**
 * Implements hook_node_access
 */

function sirius_denorm_node_access($node, $op, $account) {
  // Get the node type
  if (is_object($node)) { $type = $node->type; } else { $type = $node; }

  // We know nothing.
  return NODE_ACCESS_IGNORE;
}
