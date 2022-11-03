<?php

/***
 * Implements hook_permission
 */

function sirius_trust_permission() {
  return array(
    'sirius trust admin' => array(
      'title' => t('Sirius Trust Admin'), 
      'description' => t('Administer all aspects of the Trust'),
    ),
    'sirius trust staff' => array(
      'title' => t('Sirius Trust Staff'), 
      'description' => t('Access to use the TrustFund component as a staff member'),
    ),
    'sirius trust employer' => array(
      'title' => t('Sirius Trust Employer'), 
      'description' => t('Access to use the TrustFund component as an employer'),
    ),
  );
}

/**
 * Implements hook_node_access
 */

function sirius_trust_node_access($node, $op, $account) {
  // Get the node type
  if (is_object($node)) { $type = $node->type; } else { $type = $node; }

  if ($type == 'sirius_trust_benefit') {
    // Must be trust admin to do anything at all to benefits
    if (!user_access('sirius trust admin')) { return NODE_ACCESS_DENY; }
  }

  if ($type == 'sirius_trust_worker_benefit') {
    // Must be trust staff to do anything at all to worker benefits
    if (!user_access('sirius trust staff')) { return NODE_ACCESS_DENY; }
  }

  // We know nothing.
  return NODE_ACCESS_IGNORE;
}
