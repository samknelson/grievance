<?php

class Sirius_Edls_Access extends Sirius_Access {
  public $type = 'edls';
  public $name = 'EDLS';
  public $description = '';

  public function access($action, $context = array()) {
    global $user;

    $actions = array(
      // No parameters
      'staff',
      
    	// Parameter employer_nid
			'employer_config_tasks', 
			'employer_config_outsource_workers', 
			'employer_config_outsource_ms',

      // Parameter employer_nid
      'employer_dashboard',

    	// Parameter employer_nid
    	'worker_list',

    	// Parameter employer_nid
    	'sheet_create',
    	'sheet_list',

    	// parameter sheet_node
    	'sheet_view',
    	'sheet_manage',
    	'sheet_clone',
    	'sheet_edit',
    	'sheet_edit_workers',
      'sheet_change_supervisor',
      'sheet_revisions',
      'sheet_assignmentlog',

      // parameter sheet_node
      'sheet_status_draft',
      'sheet_status_request',
      'sheet_status_lock',
    );

    $sheet_node = $context['sheet_node'];
    $sheet_status = $sheet_node->field_sirius_edls_sheet_status['und'][0]['value'];
    $sheet_uid = $sheet_node->field_grievance_rep_assignee['und'][0]['target_id'];

    $is_my_sheet = ($sheet_uid == $user->uid);

    // Who am I?
    if (user_access('sirius admin') || user_access('sirius edls admin')) { 
      $is_admin = $is_employer_manager = $is_employer_coordinator = $is_employer_supervisor = $is_worker_advisor = TRUE;
    } else if (user_access('sirius edls employer labor manager')) { 
      $is_employer_manager = $is_employer_coordinator = $is_employer_supervisor = $is_worker_advisor = TRUE;
    } else if (user_access('sirius edls employer labor coordinator')) { 
      $is_employer_coordinator = $is_employer_supervisor = TRUE;
    } else if (user_access('sirius edls employer labor supervisor')) { 
      $is_employer_supervisor = TRUE;
    }

    if (user_access('sirius edls worker labor advisor')) { 
      $is_worker_advisor = TRUE;
    }

    // Admins can do anything
    if ($is_admin) { return TRUE; }

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

      // Labor supervisors can view the statistics
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
    if (in_array($action, array('sheet_view', 'sheet_clone', 'sheet_manage'))) {
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
