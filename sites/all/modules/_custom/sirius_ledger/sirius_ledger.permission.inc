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
    'sirius ledger employer' => array(
      'title' => t('Sirius Ledger Employer'), 
      'description' => t('Permission to access the A/R system as an employer.'),
    ),
    'sirius ledger ledger worker' => array(
      'title' => t('Sirius Ledger Worker'), 
      'description' => t('Permission to access the A/R system as a worker.'),
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

class Sirius_Ledger_Access extends Sirius_Access {
  public $type = 'ledger';
  public $name = 'Ledger';
  public $description = '';

  public function access($action, $context = array()) {
    global $user;

    $actions = array(
      // No parameters
      'admin',
      
      // Parameter entity_node
      'entity_page_view', 
    );

    // Who am I?
    if (user_access('sirius admin') || user_access('sirius ledger admin')) { 
      $is_admin = TRUE;
    }

    // Admins can do anything
    if ($is_admin) { return TRUE; }

    $user_full = user_load($user->uid);

    // I may be restricted by "entity". For exmaple, if I am an employer,
    // I might be restricted to only see A/R entries or features within the context of
    // my specific employer.
    $is_my_entity = FALSE;
    if (user_access('sirius ledger stripe employer')) {
      $my_employer_nid = $user_full->field_grievance_shops['und'][0]['target_id'];
      if ($context['entity_node'] && 
        $context['entity_node']->type == 'grievance_shop' && 
        $context['entity_node']->nid == $my_employer_nid) {
        $is_my_entity = TRUE;
      }
    }

    if (user_access('sirius ledger stripe worker')) {
      // @todo: Same logic for a worker
    }

    // If we have an entity node, it must be MY entity node
    if ($context['entity_node'] && !$is_my_entity) {
      return FALSE;
    }

    // View entity page
    if ($action == 'entity_page_view') {
      return $is_my_entity;
    }

    // Oh well
    return FALSE;

    // Check to make sure that I am in the right employer
    if ($context['employer_nid']) {
      if ($my_employer_nid != $context['employer_nid']) { return FALSE; }
    }
    if ($context['sheet_node']) {
      if ($my_employer_nid != $context['sheet_node']->field_grievance_shop['und'][0]['target_id']) { return FALSE; }
    }

    // Employer Configuration: workers, tasks, member statuses
    if (in_array($action, array('employer_config_tasks', 'employer_config_outsource_workers', 'employer_config_outsource_ms'))) {
      // Labor coordinators can configure the employer
      if ($is_employer_coordinator) { return TRUE; }
    }

    // View the employer statistics
    if (in_array($action, array('employer_dashboard'))) {
      // Labor coordinators can view the statistics
      if ($is_employer_coordinator) { return TRUE; }
      if ($is_worker_advisor) { return TRUE; }
    }

    // Change the supervisor on a sheet
    if (in_array($action, array('sheet_change_supervisor'))) {
      // Labor coordinators can configure the employer
      if ($is_employer_coordinator) { return TRUE; }
    }

    // Create a sheet, list sheets, list workers
    if (in_array($action, array('sheet_create', 'sheet_list', 'worker_list'))) {
      // Labor supervisors can create and list sheets and workers
      if ($is_employer_supervisor) { return TRUE; }
      if ($is_worker_advisor) { return TRUE; }
    }

    // View, clone, or manage a sheet
    if (in_array($action, array('sheet_view', 'sheet_clone', 'sheet_manage', 'sheet_export'))) {
      // Labor coordinators can view anything
      if ($is_employer_coordinator) { return TRUE; }
      if ($is_worker_advisor) { return TRUE; }

      // Labor supervisors can view their own sheets
      if ($is_employer_supervisor && $is_my_sheet) { return TRUE; }
    }

    // Edit a sheet or assign workers
    if (in_array($action, array('sheet_edit', 'sheet_edit_workers'))) {
      // Labor coordinators can edit sheets in "draft" or "request")
      if ($is_employer_coordinator && in_array($sheet_status, array('draft', 'request'))) { return TRUE; }

      // Labor advisors can edit sheets that are not in "lock"
      if ($is_worker_advisor && ($sheet_status != 'lock')) { return TRUE; }

      // Labor supervisors can edit their own sheets in "draft"
      if ($is_employer_supervisor && $is_my_sheet && ($sheet_status == 'draft')) { return TRUE; }
    }

    // View changes
    if (in_array($action, array('sheet_revisions', 'sheet_assignmentlog'))) {
      // Labor coordinators only
      if ($is_employer_coordinator) { return TRUE; }
      if ($is_worker_advisor) { return TRUE; }
    }

    // Set sheet to "Draft"
    if ($action == 'sheet_status_draft') {
      // Can't change status to what it already is
      if ($sheet_status == 'draft') { return FALSE; }

      // Coordinators can set any sheet to "draft"
      if ($is_employer_coordinator) { return TRUE; }

      // Labor advisors can edit sheets that are not in "lock"
      if ($is_worker_advisor && ($sheet_status != 'lock')) { return TRUE; }
    }

    // Set sheet to "Request"
    if ($action == 'sheet_status_request') {
      // Can't change status to what it already is
      if ($sheet_status == 'request') { return FALSE; }

      // Coordinators can set any sheet to "request"
      if ($is_employer_coordinator) { return TRUE; }

      // Labor advisors can edit sheets that are not in "lock"
      if ($is_worker_advisor && ($sheet_status != 'lock')) { return TRUE; }

      // Supervisors can set their own sheet to "request" if it's in "draft"
      if ($is_employer_supervisor && $is_my_sheet && ($sheet_status == 'draft')) { return TRUE; }
    }

    // Set sheet to "Lock"
    if ($action == 'sheet_status_lock') {
      // Can't change status to what it already is
      if ($sheet_status == 'lock') { return FALSE; }

      // Coordinators can set any sheet to "lock"
      if ($is_employer_coordinator) { return TRUE; }
    }

    // Set sheet to "Trash"
    if ($action == 'sheet_status_trash') {
      // Can't change status to what it already is
      if ($sheet_status == 'trash') { return FALSE; }

      // Coordinators can set any sheet to "trash"
      if ($is_employer_coordinator) { return TRUE; }

      // Labor advisors can edit sheets that are not in "lock"
      if ($is_worker_advisor && ($sheet_status != 'lock')) { return TRUE; }

      // Supervisors can set their own sheet to "trash" if it's in "draft"
      if ($is_employer_supervisor && $is_my_sheet && ($sheet_status == 'draft')) { return TRUE; }
    }
  }
}











