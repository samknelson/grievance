<?php

/***
 * Implements hook_permission
 */

function sirius_hour_permission() {
  return array(
    'sirius hour user' => array(
      'title' => t('Sirius Hour User'), 
      'description' => t('Permission to see your own hours.'),
    ),
    'sirius hour admin' => array(
      'title' => t('Sirius Hour Admin'), 
      'description' => t('Permission to manage hours and pay periods.'),
    ),
  );
}

/**
 * Implements hook_node_access
 */

function sirius_hour_node_access($node, $op, $account) {
  // Specific checks by node type
	// if ($node->type == 'sirius_log') { return sirius_node_access_log($node, $op, $account); }

  // We know nothing.
  return NODE_ACCESS_IGNORE;
}
