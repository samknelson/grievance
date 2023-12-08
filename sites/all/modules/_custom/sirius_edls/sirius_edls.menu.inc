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

  $node_tabs['grievance_shop']['grievance_shop_edls'] = array(
    'title' => 'EDLS',
    'description' => '',
    'callback' => 'sirius_edls_node_tab_employer_outsource',
    'permissions' => array('sirius'),
    'file' => 'sirius_edls.page_employer_outsource.inc',
    'skip_node_view_check' => TRUE,
    'component' => 'edls',
    'weight' => 3,
  );

  $node_tabs['grievance_shop']['grievance_shop_edls/outsource'] = array(
    'title' => 'Outsource Workers',
    'description' => '',
    'callback' => 'sirius_edls_node_tab_employer_outsource',
    'permissions' => array('sirius'),
    'file' => 'sirius_edls.page_employer_outsource.inc',
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
    'weight' => 1,
    'file' => 'sirius_edls.page_worker_list.inc'
  );






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

  $items['sirius/edls/ajax/worker/list'] = array(
    'title' => 'EDLS Ajax Worker List',
    'page callback' => 'sirius_edls_ajax_worker_list',
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

  $items['sirius/edls/ajax/worker/lookup'] = array(
    'title' => 'EDLS Ajax Worker Lookup',
    'page callback' => 'sirius_edls_ajax_worker_lookup',
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

  $items['sirius/edls/ajax/assign'] = array(
    'title' => 'EDLS Ajax Assign',
    'page callback' => 'sirius_edls_ajax_assign',
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

  $items['sirius/edls/ajax/assignment_set_extra'] = array(
    'title' => 'EDLS Ajax Update',
    'page callback' => 'sirius_edls_ajax_assignment_set_extra',
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

  $items['sirius/edls/ajax/unassign'] = array(
    'title' => 'EDLS Ajax Unassign',
    'page callback' => 'sirius_edls_ajax_unassign',
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

  $items['sirius/edls/ajax/sheet/assignments'] = array(
    'title' => 'EDLS Ajax Sheet Assignments',
    'page callback' => 'sirius_edls_ajax_sheet_assignments',
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

  return $items;
}
