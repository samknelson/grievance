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
    'sirius worker view headshot' => array(
      'title' => t('Sirius Worker View Headshot'), 
      'description' => t('Permission to view the headshot of any worker. This may be granted to users without "Sirius Worker View", for example, if a steward has permission to see headshots on worker lists but not to view the underlying worker record.'),
    ),
    'sirius worker admin' => array(
      'title' => t('Sirius Worker Admin'), 
      'description' => t('Permission to perform any action on workers: add, edit, delete.'),
    ),
    'sirius worker create' => array(
      'title' => t('Sirius Worker Create'), 
      'description' => t('Permission to create new workers.'),
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

class Sirius_Worker_Access extends Sirius_Access {
  public $type = 'worker';
  public $name = 'Worker';
  public $description = '';

  public function access($action, $context = array()) {
    global $user;
    $user_full = user_load($user->uid);

    $actions = array(
      // No parameters
      'config',
      'staff', 

      // optional employer_nid
      'create worker',
    );

    // Who am I?
    if (user_access('sirius admin')) { 
      $is_admin = $is_staff = TRUE;
    } else if (user_access('sirius worker admin')) {
      $is_staff = TRUE; 
    } else if (user_access('sirius dispatch employer') || 
        user_access('sirius employer user manager') || 
        user_access('sirius employer contact manager')) { 
      $is_employer = TRUE;
    } 


    // If we have an employer node in the context, it must be valid
    if (isset($context['employer_node'])) {
      if (!$context['employer_node']) { return FALSE; }
      if ($context['employer_node']->type != 'grievance_shop') {  return FALSE; }
      if ($context['employer_node']->field_sirius_domain['und'][0]['target_id'] != sirius_domain_nid()) { return FALSE; }

      if (!$is_admin) {
        $my_employer_node = sirius_employer_get();
        if ($my_employer_node->nid != $context['employer_node']->nid) { return FALSE; }
      }
    }

    // Admins can do anything
    if ($is_admin) { return TRUE; }

    if ($action == 'staff') {
      if ($is_staff) { return TRUE; }
    }

    if ($action == 'create worker') {
      if (user_access('sirius worker create')) { return TRUE; }
    }

    return FALSE;

    // Anonymous users can view notifications, if they have the correct UUID
    if (in_array($action, array('notification_view', 'notification_accept', 'notification_decline'))) {
      // Must have a UUID
      if (!$context['notification_uuid']) { return FALSE; }

      // It must match
      if ($context['notification_uuid'] != $context['notification']['notification_uuid']) { return FALSE; }

      // That's all you need to view the notification.
      if ($action == 'notification_view') { return TRUE; }

      // You can accept or decline if status = 'sent'
      if (
          (in_array($action, array('notification_accept', 'notification_decline'))) && 
          (in_array($context['notification']['notification_status'], array('sent', 'error')))
        ) {
        return TRUE;
      }
    }

    // Everybody else is restricted to one employer_nid
    $user_full = user_load($user->uid);
    $my_employer_nid = $user_full->field_grievance_shops['und'][0]['target_id'];
    if (!$my_employer_nid) { return FALSE; }

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
