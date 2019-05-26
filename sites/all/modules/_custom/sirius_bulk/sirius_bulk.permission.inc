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
  );
}

function sirius_bulk_node_access($node, $op, $account) {
  // Only interested in bulk message nodes
	if ($node->type != 'sirius_bulk') { return NODE_ACCESS_IGNORE; }

  // Admins can access
  if (user_access('sirius bulk send', $account)) { return NODE_ACCESS_ALLOW; }

  // Nobody else can do anything
  return NODE_ACCESS_DENY;
}
