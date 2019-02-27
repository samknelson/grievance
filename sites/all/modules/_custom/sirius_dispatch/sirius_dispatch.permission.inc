<?php

/***
 * Implements hook_permission
 */

function sirius_dispatch_permission() {
  return array(
    'sirius dispatch staff' => array(
      'title' => t('Sirius Dispatch Staff'), 
      'description' => t('Basic staff permission -- create, modify, or delete any job or dispatch'),
    ),
    'sirius dispatch admin' => array(
      'title' => t('Sirius Dispatch Admin'), 
      'description' => t('Administration permission -- create, modify, or delete facilities, or configure the dispatch system.'),
    ),
    'sirius dispatch employer' => array(
      'title' => t('Sirius Dispatch Employer'), 
      'description' => t('Operate as an employer in the Sirius Dispatch System -- create and update your own jobs and dispatches.'),
    ),
    'sirius dispatch worker' => array(
      'title' => t('Sirius Dispatch Worker'), 
      'description' => t('Operate as an worker in the Sirius Dispatch System -- view or respond to your own dispatches.'),
    ),
    'sirius dispatch resign' => array(
      'title' => t('Sirius Dispatch Resign'), 
      'description' => t('Permission to resign from your own dispatch'),
    ),
    'sirius dispatch signin' => array(
      'title' => t('Sirius Dispatch Signin'), 
      'description' => t('Permission to sign yourself in to the out-of-work list'),
    ),
    'sirius dispatch bullpen' => array(
      'title' => t('Sirius Dispatch Bullpen'), 
      'description' => t('Permission to set sign-ins for a bullpen'),
    ),
  );
}

function sirius_dispatch_node_access($node, $op, $account) {
  if ($node->type == 'sirius_dispatch') { return sirius_dispatch_node_access_dispatch($node, $op, $account); }
  return NODE_ACCESS_IGNORE;
}

function sirius_dispatch_node_access_dispatch($node, $op, $account) {
  // Admins can access
  if (user_access('sirius dispatch staff', $account)) { return NODE_ACCESS_ALLOW; }

  // Nobody else can do anything
  return NODE_ACCESS_DENY;
}

