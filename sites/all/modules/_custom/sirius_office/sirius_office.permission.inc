<?php

/***
 * Implements hook_permission
 */

function sirius_office_permission() {
  return array(
    'sirius office iprestrict admin' => array(
      'title' => t('Sirius Office IPRestrict Admin'), 
      'description' => t('Permission to administer the IPRestrict policy'),
    ),
    'sirius office iprestrict remote' => array(
      'title' => t('Sirius Office IPRestrict Remote'), 
      'description' => t('Permission to override the IPRestriction policy and sign in remotely, regardless of other settings.'),
    ),
    'sirius office timeoff request' => array(
      'title' => t('Sirius Office Timeoff Request'), 
      'description' => t('Make a timeoff request.'),
    ),
    'sirius office timeoff admin' => array(
      'title' => t('Sirius Office Timeoff Admin'), 
      'description' => t('Manage timeoff requests.'),
    ),
    'sirius office operator admin' => array(
      'title' => t('Sirius Office Operator Admin'), 
      'description' => t('Add, edit, or remove operators.'),
    ),
    'sirius office operator user' => array(
      'title' => t('Sirius Office Operator User'), 
      'description' => t('Permission to set status and use the call log for the office(s) to which you are assigned.'),
    ),
  );
}

/**
 * Implements hook_node_access
 */

function sirius_office_node_access($node, $op, $account) {
  // Get the node type
  if (is_object($node)) { $type = $node->type; } else { $type = $node; }

  // Specific checks by node type
  if ($type == 'sirius_office_timeoff_request') { return sirius_office_node_access_timeoff_request($node, $op, $account); }
  if ($type == 'sirius_operator') { return sirius_office_node_access_operator($node, $op, $account); }
  if ($type == 'grievance_chapter') { return sirius_office_node_access_chapter($node, $op, $account); }

  // We know nothing.
  return NODE_ACCESS_IGNORE;
}
