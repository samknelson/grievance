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
  if (is_object($node)) { $type = $node->type; } else { $type = $node; }

  // Specific checks by node type
  if ($type == 'sirius_payperiod') { return sirius_hour_node_access_payperiod($node, $op, $account); }
  if ($type == 'sirius_employer_payperiod') { return sirius_hour_node_access_employer_payperiod($node, $op, $account); }

  // We know nothing.
  return NODE_ACCESS_IGNORE;
}


function sirius_hour_node_access_payperiod($node, $op, $account) {
  if (!user_access('sirius hour admin')) { return NODE_ACCESS_DENY; }
  return NODE_ACCESS_IGNORE;
}

function sirius_hour_node_access_employer_payperiod($node, $op, $account) {
  if (!user_access('sirius hour admin')) { return NODE_ACCESS_DENY; }
  return NODE_ACCESS_IGNORE;
}
