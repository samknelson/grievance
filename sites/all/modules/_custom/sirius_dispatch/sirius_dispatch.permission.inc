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
    'sirius dispatch payrate override' => array(
      'title' => t('Sirius Dispatch Payrate Override'), 
      'description' => t('Permission to override the pay rate for a job'),
    ),
    'sirius dispatch vlri' => array(
      'title' => t('Sirius Dispatch VLRI'), 
      'description' => t('Permission to VLRI (voluntary layoff with a request for an involuntary layoff) from your own dispatch'),
    ),
    'sirius dispatch signin' => array(
      'title' => t('Sirius Dispatch Signin'), 
      'description' => t('Permission to sign yourself in to the out-of-work list'),
    ),
    'sirius dispatch own preferences' => array(
      'title' => t('Sirius Dispatch Own Preferences'), 
      'description' => t('Permission to set your own dispatch preferences, except for the ones listed below'),
    ),
    'sirius dispatch own preferences skill' => array(
      'title' => t('Sirius Dispatch Own Preferences Skill'), 
      'description' => t('Permission to set your own dispatch skill preferences'),
    ),
    'sirius dispatch own preferences department' => array(
      'title' => t('Sirius Dispatch Own Preferences department'), 
      'description' => t('Permission to set your own dispatch skill preferences'),
    ),
  );
}

function sirius_dispatch_node_access($node, $op, $account) {
  if ($node->type == 'sirius_dispatch') { return sirius_dispatch_node_access_dispatch($node, $op, $account); }
  if ($node->type == 'sirius_dispatch_job') { return sirius_dispatch_node_access_job($node, $op, $account); }
  return NODE_ACCESS_IGNORE;
}

function sirius_dispatch_node_access_dispatch($node, $op, $account) {
  // Admins can access
  if (user_access('sirius dispatch staff', $account)) { return NODE_ACCESS_ALLOW; }

  // Nobody else can do anything
  return NODE_ACCESS_DENY;
}

function sirius_dispatch_node_access_job($node, $op, $account) {
  // Admins can access
  if (user_access('sirius dispatch staff', $account)) { return NODE_ACCESS_ALLOW; }

  // Nobody else can do anything
  return NODE_ACCESS_DENY;
}

