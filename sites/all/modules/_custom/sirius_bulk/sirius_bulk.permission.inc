<?php

/***
 * Implements hook_permission
 */

function sirius_bulk_permission() {
  return array(
    'sirius bulk admin' => array(
      'title' => t('Sirius Bulk Admin'), 
      'description' => t('Administer the bulk messaging system.'),
    ),
    'sirius bulk send' => array(
      'title' => t('Sirius Bulk Send'), 
      'description' => t('Create and send bulk messages.'),
    ),
    'sirius bulk manual' => array(
      'title' => t('Sirius Bulk Manual'), 
      'description' => t('Deliver existing queue entries which require manual confirmation'),
    ),
  );
}

/**
 * Implements hook_node_access
 */

function sirius_bulk_node_access($node, $op, $account) {
  if (is_object($node)) { $type = $node->type; } else { $type = $node; }
  if ($type != 'sirius_bulk') { return NODE_ACCESS_IGNORE; }

  if (user_access('sirius bulk send')) { return NODE_ACCESS_IGNORE; }

  return NODE_ACCESS_DENY;

  /*
  // Admins can do anything
  if ($op == 'view' && user_access('sirius bulk send', $account)) {return NODE_ACCESS_IGNORE; }

  // Nobody else can do anything
  return NODE_ACCESS_IGNORE;
  */
}
