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

class Sirius_Hour_Access extends Sirius_Access {
  public function access($action, $context = array()) {
    global $user;

    $actions = array(
      // No parameters
      'admin',
      'staff',
      
      // Payperiod
      'payperiod_view',
      'payperiod_edit',

      // Worker
      'worker_hours_view',
      'worker_hours_edit',
    );

    // Who am I?
    if (user_access('sirius admin')) {
      $is_employer = $is_admin = $is_staff = TRUE;
    } else if (user_access('sirius hour admin')) {
      $is_employer = $is_staff = TRUE;
    } else if (FALSE) {
      $is_employer = TRUE;
    } else if (FALSE) {
      $is_worker = TRUE;
    }

    // Admin can do anything
    if ($is_admin) { return TRUE; }

    // Staff can do anything
    if ($is_staff) { return TRUE; }

    // Is this my worker?
    if ($context['worker_node']) {
      $my_worker_node = sirius_worker_get();
      if ($my_worker_node && ($my_worker_node->nid == $context['worker_node']->nid)) {
        $is_my_worker = TRUE;
      }
    }

    // Can view my own hours
    if (in_array($action, array('worker_hours_view'))) {
      if ($is_my_worker) { return TRUE; }

      return FALSE;
    }

    return $this->deny("Unknown action: $action");
  }
}