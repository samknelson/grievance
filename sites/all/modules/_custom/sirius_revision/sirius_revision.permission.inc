<?php

/***
 * Implements hook_permission
 */

function sirius_revision_permission() {
  return array(
    'sirius revision admin' => array(
      'title' => t('Sirius Revision Admin'), 
      'description' => t('Permission to administer node revision settings'),
    ),
  );
}

/**
 * Implements hook_node_access
 */

/*
function sirius_revision_node_access($node, $op, $account) {
  // Get the node type
  if (is_object($node)) { $type = $node->type; } else { $type = $node; }

  // We know nothing.
  return NODE_ACCESS_IGNORE;
}
*/