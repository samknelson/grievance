<?php

/***
 * Implements hook_permission
 */

function sirius_worker_permission() {
  return array(
    'sirius worker admin' => array(
      'title' => t('Sirius Worker Admin'), 
      'description' => t('Administer workers.'),
    ),
  );
}

function sirius_worker_node_access($node, $op, $account) {
	if ($node->type == 'sirius_worker') { return sirius_worker_node_access_worker($node, $op, $account); }
  if ($node->type == 'sirius_skill') { return sirius_worker_node_access_skill($node, $op, $account); }

	return NODE_ACCESS_IGNORE;
}

function sirius_worker_node_access_worker($node, $op, $account) {
  // Admins can access
  if (user_access('sirius worker admin', $account)) { return NODE_ACCESS_ALLOW; }

  // Users can view their own
  if ($op == 'view') {
    $worker_node = sirius_worker_get($account->uid);
    if ($worker_node && $node && $worker_node->nid == $node->nid) { return NODE_ACCESS_ALLOW; }
  }

  // Nobody else can do anything
  return NODE_ACCESS_DENY;
}

function sirius_worker_node_access_skill($node, $op, $account) {
  if (user_access('sirius worker admin', $account)) { return NODE_ACCESS_IGNORE; }
  return NODE_ACCESS_DENY;
}

