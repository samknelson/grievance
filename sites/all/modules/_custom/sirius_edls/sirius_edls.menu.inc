<?php

/**
 * Implements hook_node_tabs
 */

function sirius_edls_node_tabs() {
  $node_tabs = array();

  $node_tabs['sirius_edls_sheet']['sirius_edls_sheet_view'] = array(
    'title' => 'View Sheet',
    'description' => '',
    'callback' => 'sirius_edls_node_tab_sheet_view',
    'permissions' => array('sirius'),
    'file' => 'sirius_edls.page_sheet_view.inc',
    'skip_node_view_check' => TRUE,
    'component' => 'edls',
    'weight' => 1,
  );

  $node_tabs['sirius_edls_sheet']['sirius_edls_sheet_edit'] = array(
    'title' => 'Edit Sheet',
    'description' => '',
    'callback' => 'sirius_edls_node_tab_sheet_edit',
    'permissions' => array('sirius'),
    'file' => 'sirius_edls.page_sheet_edit.inc',
    'skip_node_view_check' => TRUE,
    'component' => 'edls',
    'weight' => 2,
  );

  $node_tabs['sirius_edls_sheet']['sirius_edls_sheet_manage'] = array(
    'title' => 'Manage Sheet',
    'description' => '',
    'callback' => 'sirius_edls_node_tab_sheet_manage',
    'permissions' => array('sirius'),
    'file' => 'sirius_edls.page_sheet_manage.inc',
    'skip_node_view_check' => TRUE,
    'component' => 'edls',
    'weight' => 3,
  );

  $node_tabs['sirius_edls_sheet']['sirius_edls_sheet_workers'] = array(
    'title' => 'Assign Workers',
    'description' => '',
    'callback' => 'sirius_edls_node_tab_sheet_workers',
    'permissions' => array('sirius'),
    'file' => 'sirius_edls.page_sheet_workers.inc',
    'skip_node_view_check' => TRUE,
    'component' => 'edls',
    'weight' => 4,
  );

  $node_tabs['sirius_edls_sheet']['sirius_edls_sheet_revisions'] = array(
    'title' => 'Sheet Change History',
    'description' => '',
    'callback' => 'sirius_edls_node_tab_sheet_revisions',
    'permissions' => array('sirius'),
    'file' => 'sirius_edls.page_sheet_revisions.inc',
    'skip_node_view_check' => TRUE,
    'component' => 'edls',
    'weight' => 4,
  );

  $node_tabs['sirius_edls_sheet']['sirius_edls_sheet_revisions/revisions'] = array(
    'title' => 'Revisions',
    'description' => '',
    'callback' => 'sirius_edls_node_tab_sheet_revisions',
    'permissions' => array('sirius'),
    'file' => 'sirius_edls.page_sheet_revisions.inc',
    'skip_node_view_check' => TRUE,
    'component' => 'edls',
    'weight' => 1,
    'type' => MENU_DEFAULT_LOCAL_TASK,
  );

  $node_tabs['sirius_edls_sheet']['sirius_edls_sheet_revisions/assignmentlog'] = array(
    'title' => 'Assignment Log',
    'description' => '',
    'callback' => 'sirius_edls_node_tab_sheet_assignmentlog',
    'permissions' => array('sirius'),
    'file' => 'sirius_edls.page_sheet_assignmentlog.inc',
    'skip_node_view_check' => TRUE,
    'component' => 'edls',
    'weight' => 2,
  );

  $node_tabs['grievance_shop']['grievance_shop_edls'] = array(
    'title' => 'EDLS',
    'description' => '',
    'callback' => 'sirius_edls_node_tab_employer_active',
    'permissions' => array('sirius'),
    'file' => 'sirius_edls.page_employer_active.inc',
    'skip_node_view_check' => TRUE,
    'component' => 'edls',
    'weight' => 3,
  );

  $node_tabs['grievance_shop']['grievance_shop_edls/active'] = array(
    'title' => 'Enable EDLS',
    'description' => '',
    'callback' => 'sirius_edls_node_tab_employer_active',
    'permissions' => array('sirius'),
    'file' => 'sirius_edls.page_employer_active.inc',
    'skip_node_view_check' => TRUE,
    'component' => 'edls',
    'weight' => 1,
    'type' => MENU_DEFAULT_LOCAL_TASK,
  );

  $node_tabs['grievance_shop']['grievance_shop_edls/outsource_ms'] = array(
    'title' => 'Outsource Member Statuses',
    'description' => '',
    'callback' => 'sirius_edls_node_tab_employer_outsource_ms',
    'permissions' => array('sirius'),
    'file' => 'sirius_edls.page_employer_outsource_ms.inc',
    'skip_node_view_check' => TRUE,
    'component' => 'edls',
    'weight' => 2,
  );

  $node_tabs['grievance_shop']['grievance_shop_edls/tasks'] = array(
    'title' => 'Tasks',
    'description' => '',
    'callback' => 'sirius_edls_node_tab_employer_tasks',
    'permissions' => array('sirius'),
    'file' => 'sirius_edls.page_employer_tasks.inc',
    'skip_node_view_check' => TRUE,
    'component' => 'edls',
    'weight' => 3,
  );

  $node_tabs['grievance_shop']['grievance_shop_edls/client'] = array(
    'title' => 'Client',
    'description' => '',
    'callback' => 'sirius_edls_node_tab_employer_client',
    'permissions' => array('sirius'),
    'file' => 'sirius_edls.page_employer_client.inc',
    'skip_node_view_check' => TRUE,
    'component' => 'edls:service:client',
    'weight' => 4,
  );

  $node_tabs['grievance_shop']['grievance_shop_edls/server'] = array(
    'title' => 'Server',
    'description' => '',
    'callback' => 'sirius_edls_node_tab_employer_server',
    'permissions' => array('sirius'),
    'file' => 'sirius_edls.page_employer_server.inc',
    'skip_node_view_check' => TRUE,
    'component' => 'edls:service:server',
    'weight' => 4,
  );

  sirius_node_tab_set_module('sirius_edls', $node_tabs);

  return $node_tabs;
}

function sirius_edls_menu() {
  $items = array();

  $items['sirius/edls/sheet-create'] = array(
    'title' => 'Create a Sheet',
    'page callback' => 'sirius_edls_page_sheet_create',
    'page arguments' => array(),
    'access callback' => 'sirius_menu_access_callback',
    'access arguments' => array(
      array(
        'component' => 'edls',
        'access' => array(
          'type' => 'edls',
          'action' => 'sheet_create',
          'context' => array(),
        ),
      ),
    ),
    'type' => MENU_NORMAL_ITEM,
    'file' => 'sirius_edls.page_sheet_create.inc'
  );

  $items['sirius/edls/sheet-create-reserved'] = array(
    'title' => 'Create a Sheet',
    'page callback' => 'sirius_edls_page_sheet_create_reserved',
    'page arguments' => array(),
    'access callback' => 'sirius_menu_access_callback',
    'access arguments' => array(
      array(
        'component' => 'edls',
        'access' => array(
          'type' => 'edls',
          'action' => 'sheet_create_reserved',
          'context' => array(),
        ),
      ),
    ),
    'type' => MENU_NORMAL_ITEM,
    'file' => 'sirius_edls.page_sheet_create_reserved.inc'
  );

  $items['sirius/edls'] = array(
    'title' => 'Employer Daily Labor Schedules',
    'page callback' => 'sirius_edls_page_landing',
    'page arguments' => array(),
    'access callback' => 'sirius_menu_access_callback',
    'access arguments' => array(
      array(
        'component' => 'edls',
        'access' => array(
          'type' => 'edls',
          'action' => 'sheet_list',
          'context' => array(),
        ),
      ),
    ),
    'type' => MENU_NORMAL_ITEM,
    'file' => 'sirius_edls.page_landing.inc'
  );

  $items['sirius/edls/landing'] = array(
    'title' => 'Home',
    'page callback' => 'sirius_edls_page_landing',
    'page arguments' => array(),
    'access callback' => 'sirius_menu_access_callback',
    'access arguments' => array(
      array(
        'component' => 'edls',
        'access' => array(
          'type' => 'edls',
          'action' => 'sheet_list',
          'context' => array(),
        ),
      ),
    ),
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => 1,
    'file' => 'sirius_edls.page_landing.inc'
  );

  $items['sirius/edls/sheets'] = array(
    'title' => 'Sheets',
    'page callback' => 'sirius_edls_page_sheet_list',
    'page arguments' => array(),
    'access callback' => 'sirius_menu_access_callback',
    'access arguments' => array(
      array(
        'component' => 'edls',
        'access' => array(
          'type' => 'edls',
          'action' => 'sheet_list',
          'context' => array(),
        ),
      ),
    ),
    'type' => MENU_LOCAL_TASK,
    'weight' => 1,
    'file' => 'sirius_edls.page_sheet_list.inc'
  );

  $items['sirius/edls/workers'] = array(
    'title' => 'Workers',
    'page callback' => 'sirius_edls_page_worker_list',
    'page arguments' => array(),
    'access callback' => 'sirius_menu_access_callback',
    'access arguments' => array(
      array(
        'component' => 'edls',
        'access' => array(
          'type' => 'edls',
          'action' => 'sheet_list',
          'context' => array(),
        ),
      ),
    ),
    'type' => MENU_LOCAL_TASK,
    'weight' => 2,
    'file' => 'sirius_edls.page_worker_list.inc'
  );

  $items['sirius/edls/workers/list'] = array(
    'title' => 'List',
    'page callback' => 'sirius_edls_page_worker_list',
    'page arguments' => array(),
    'access callback' => 'sirius_menu_access_callback',
    'access arguments' => array(
      array(
        'component' => 'edls',
        'access' => array(
          'type' => 'edls',
          'action' => 'sheet_list',
          'context' => array(),
        ),
      ),
    ),
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => 1,
    'file' => 'sirius_edls.page_worker_list.inc'
  );

  $items['sirius/edls/workers/outsource'] = array(
    'title' => 'Outsource',
    'page callback' => 'sirius_edls_page_worker_outsource',
    'page arguments' => array(),
    'access callback' => 'sirius_menu_access_callback',
    'access arguments' => array(
      array(
        'component' => 'edls',
        'access' => array(
          'type' => 'edls',
          'action' => 'employer_config_outsource_workers',
          'context' => array(),
        ),
      ),
    ),
    'type' => MENU_LOCAL_TASK,
    'weight' => 2,
    'file' => 'sirius_edls.page_worker_outsource.inc'
  );

  $items['sirius/edls/workers/outsource/%'] = array(
    'title' => 'Outsource',
    'page callback' => 'sirius_edls_page_worker_outsource',
    'page arguments' => array(4),
    'access callback' => 'sirius_menu_access_callback',
    'access arguments' => array(
      array(
        'component' => 'edls',
        'access' => array(
          'type' => 'edls',
          'action' => 'employer_config_outsource_workers',
          'context' => array(),
        ),
      ),
    ),
    'type' => MENU_LOCAL_TASK,
    'weight' => 2,
    'file' => 'sirius_edls.page_worker_outsource.inc'
  );

  $items['sirius/edls/workers/soop'] = array(
    'title' => 'Scheduled Out of Population',
    'page callback' => 'sirius_edls_page_worker_soop',
    'page arguments' => array(),
    'access callback' => 'sirius_menu_access_callback',
    'access arguments' => array(
      array(
        'component' => 'edls',
        'access' => array(
          'type' => 'edls',
          // 'action' => 'employer_dashboard',
          'action' => 'employer_dashboard',
          'context' => array(),
        ),
      ),
    ),
    'type' => MENU_LOCAL_TASK,
    'weight' => 10,
    'file' => 'sirius_edls.page_worker_soop.inc'
  );

  /*
  $items['sirius/edls/assignments'] = array(
    'title' => 'Assignments',
    'page callback' => 'sirius_edls_page_assignment_list',
    'page arguments' => array(),
    'access callback' => 'sirius_menu_access_callback',
    'access arguments' => array(
      array(
        'component' => 'edls',
        'access' => array(
          'type' => 'edls',
          'action' => 'assignment_list',
          'context' => array(),
        ),
      ),
    ),
    'type' => MENU_LOCAL_TASK,
    'weight' => 3,
    'file' => 'sirius_edls.page_assignment_list.inc'
  );
  */

  $items['edls-notify/%'] = array(
    'title' => 'Notification',
    'page callback' => 'sirius_edls_page_notification',
    'page arguments' => array(1),
    'access callback' => 'sirius_menu_access_callback',
    'access arguments' => array(
      array(
        'component' => 'edls',
      ),
    ),
    'type' => MENU_NORMAL_ITEM,
    'file' => 'sirius_edls.page_notification.inc'
  );

  /*
  $node_tabs['grievance_shop']['grievance_shop_edls/outsource'] = array(
    'title' => 'Outsource Workers',
    'description' => '',
    'callback' => 'sirius_edls_node_tab_employer_outsource',
    'permissions' => array('sirius'),
    'file' => 'sirius_edls.page_employer_outsource.inc',
    'skip_node_view_check' => TRUE,
    'component' => 'edls',
  );
  */

  $items['sirius/edls/config'] = array(
    'title' => 'EDLS Configuration',
    'page callback' => 'sirius_edls_page_config',
    'page arguments' => array(),
    'access callback' => 'sirius_menu_access_callback',
    'access arguments' => array(
      array(
        'component' => 'edls',
        'permission' => 'sirius edls admin'
      ),
    ),
    'type' => MENU_NORMAL_ITEM,
    'file' => 'sirius_edls.page_config.inc'
  );

  $ajax_actions = array(
    'worker_list' => 'sirius_edls_ajax_worker_list',
    'worker_lookup' => 'sirius_edls_ajax_worker_lookup',
    'assign' => 'sirius_edls_ajax_assign',
    'assignment_set_extra' => 'sirius_edls_ajax_assignment_set_extra',
    'unassign' => 'sirius_edls_ajax_unassign',
    'sheet_assignments' => 'sirius_edls_ajax_sheet_assignments',
    'sheet_set_notes' => 'sirius_edls_ajax_sheet_set_notes',
  );

  foreach ($ajax_actions as $action => $callback) {
    $items["sirius/edls/ajax/$action"] = array(
      'page callback' => $callback,
      'page arguments' => array(),
      'access callback' => 'sirius_menu_access_callback',
      'access arguments' => array(
        array(
          'component' => 'edls',
          'permission' => 'sirius'
        ),
      ),
      'type' => MENU_CALLBACK,
    );
  }

  return $items;
}
