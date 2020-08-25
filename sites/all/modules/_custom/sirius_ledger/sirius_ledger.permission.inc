<?php

/***
 * Implements hook_permission
 */

function sirius_ledger_permission() {
  return array(
    'sirius ledger admin' => array(
      'title' => t('Sirius Ledger Admin'), 
      'description' => t('Permission to manage all aspects of the ledger system.'),
    ),
  );
}

/**
 * Implements hook_node_access
 */

function sirius_ledger_node_access($node, $op, $account) {
  // Get the node type
  if (is_object($node)) { $type = $node->type; } else { $type = $node; }

  // Specific checks by node type
	if ($node->type == 'sirius_payment') { return sirius_ledger_node_access_payment($node, $op, $account); }

  // We know nothing.
  return NODE_ACCESS_IGNORE;
}

function sirius_ledger_node_access_payment($node, $op, $account) {
  // Sirius Admin can do anything
  if (user_access('sirius admin')) { return NODE_ACCESS_ALLOW; }

  // The only operation that can be performed directly on these nodes is "view".
  if ($op != 'view') { return NODE_ACCESS_DENY; }

  // Must have "sirius ledger" permission
  if (!user_access('sirius ledger admin')) { return NODE_ACCESS_DENY; }

  // Ok
  return NODE_ACCESS_IGNORE;
}