<?php

/***
 * Implements hook_permission
 */

function sirius_worker_permission() {
  return array(
    'sirius worker view' => array(
      'title' => t('Sirius Worker View'), 
      'description' => t('Permission to view any worker.'),
    ),
    'sirius worker admin' => array(
      'title' => t('Sirius Worker Admin'), 
      'description' => t('Permission to perform any action on workers: add, edit, delete.'),
    ),
  );
}

function sirius_worker_node_access($node, $op, $account) {
	if ($node->type == 'sirius_worker' || $node == 'sirius_worker') { return sirius_worker_node_access_worker($node, $op, $account); }
  if ($node->type == 'sirius_skill' || $node == 'sirius_skill') { return sirius_worker_node_access_skill($node, $op, $account); }
  if ($node->type == 'sirius_employee' || $node == 'sirius_employee') { return sirius_worker_node_access_employee($node, $op, $account); }


	return NODE_ACCESS_IGNORE;
}

function sirius_worker_node_access_worker($node, $op, $account) {
  // Admins can access
  if (user_access('sirius admin', $account)) { return NODE_ACCESS_IGNORE; }
  if (user_access('sirius worker admin', $account)) { return NODE_ACCESS_IGNORE; }

  if ($op == 'view') {
    // Staff can access
    if (user_access('sirius worker view', $account)) { return NODE_ACCESS_IGNORE; }

    // Workers can view their own
    $my_worker_node = sirius_worker_get($account->uid);
    if ($my_worker_node && $node && $my_worker_node->nid == $node->nid) { return NODE_ACCESS_IGNORE; }
  }

  // Nobody else can do anything
  return NODE_ACCESS_DENY;
}

function sirius_worker_node_access_skill($node, $op, $account) {
  if (user_access('sirius worker admin', $account)) { return NODE_ACCESS_IGNORE; }
  return NODE_ACCESS_DENY;
}

/**
 * Emplolyee nodes are created and managed programatically, and can only be directly accessed
 * by users with the "sirius admin" permission
 */

function sirius_worker_node_access_employee($node, $op, $account) {
  if (user_access('sirius admin')) { return NODE_ACCESS_IGNORE; }
  return NODE_ACCESS_DENY;
}