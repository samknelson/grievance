<?php

/**
 * Tabs: Which node type has which tabs. 
 *
 * Much of the functionality is via local tasks in nodes.  For instance, nodes of type "worker" have a local tab called "housing" which 
 * shows housing-related functions. 
 */
 
function sirius_dispatch_node_tabs() {
  $node_tabs = array();

  $node_tabs['sirius_dispatch']['sirius_dispatch_manage'] = array(
    'title' => 'Manage',
    'description' => '',
    'callback' => 'sirius_node_tab_dispatch_manage',
    'permissions' => array('sirius dispatch staff'),
  );

  $node_tabs['sirius_dispatch']['sirius_dispatch_worker_view'] = array(
    'title' => 'Dispatch',
    'description' => '',
    'callback' => 'sirius_node_tab_dispatch_view',
    'permissions' => array(),
  );

  $node_tabs['sirius_dispatch_job']['sirius_dispatch_job_manage'] = array(
    'title' => 'Manage',
    'description' => '',
    'callback' => 'sirius_node_tab_dispatch_job_manage',
    'permissions' => array('sirius dispatch employer'),
    'weight' => 4,
  );

  $node_tabs['sirius_dispatch_job']['sirius_dispatch_job_dispatches'] = array(
    'title' => 'Dispatches',
    'description' => '',
    'callback' => 'sirius_node_tab_dispatch_job_dispatches',
    'permissions' => array('sirius'),
    'weight' => 5,
  );

  $node_tabs['sirius_dispatch_job']['sirius_dispatch_job_bullpen'] = array(
    'title' => 'Bullpen',
    'description' => '',
    'callback' => 'sirius_node_tab_dispatch_job_bullpen',
    'permissions' => array('sirius dispatch employer'),
    'weight' => 6,
  );

  $node_tabs['sirius_dispatch_job']['sirius_dispatch_job_attachments'] = array(
    'title' => 'Attachments',
    'description' => '',
    'callback' => 'sirius_node_tab_dispatch_job_attachments',
    'permissions' => array('sirius dispatch employer'),
    'weight' => 7,
  );

  $node_tabs['sirius_dispatch_job']['sirius_dispatch_job_payrate'] = array(
    'title' => 'Pay Rate',
    'description' => '',
    'callback' => 'sirius_node_tab_dispatch_job_payrate',
    'permissions' => array('sirius dispatch payrate override'),
    'weight' => 8,
  );

  $node_tabs['sirius_dispatch_facility']['sirius_dispatch_facility_halls'] = array(
    'title' => 'Halls',
    'description' => '',
    'callback' => 'sirius_node_tab_dispatch_facility_halls',
    'permissions' => array('sirius dispatch staff'),
  );

  $node_tabs['sirius_worker']['sirius_worker_dispatches'] = array(
    'title' => 'Dispatch',
    'description' => '',
    'callback' => 'sirius_node_tab_worker_dispatches',
    'permissions' => array('sirius dispatch worker'),
  );

  $node_tabs['sirius_worker']['sirius_worker_dispatches/status'] = array(
    'title' => 'Dispatch Status',
    'description' => '',
    'callback' => 'sirius_node_tab_worker_dispatch_status',
    'permissions' => array('sirius dispatch worker'),
    'weight' => 0,
  );

  /*
  $node_tabs['sirius_worker']['sirius_worker_dispatches/current'] = array(
    'title' => 'Current Dispatches',
    'description' => '',
    'callback' => 'sirius_node_tab_worker_dispatches_current',
    'permissions' => array('sirius dispatch worker'),
    'weight' => 1,
  );
  */

  $node_tabs['sirius_worker']['sirius_worker_dispatches/archive'] = array(
    'title' => 'Prior Dispatches',
    'description' => '',
    'callback' => 'sirius_node_tab_worker_dispatches_archive',
    'permissions' => array('sirius dispatch worker'),
    'weight' => 2,
  );

  $node_tabs['sirius_worker']['sirius_worker_dispatches/all'] = array(
    'title' => 'All Dispatches',
    'description' => '',
    'callback' => 'sirius_node_tab_worker_dispatches_all',
    'permissions' => array('sirius dispatch staff'),
    'weight' => 3,
  );

  $node_tabs['sirius_worker']['sirius_worker_dispatches/sirius_worker_dispatch_preferences'] = array(
    'title' => 'Dispatch Preferences',
    'description' => '',
    'callback' => 'sirius_node_tab_worker_dispatch_preferences',
    'permissions' => array('sirius dispatch worker'),
    'weight' => 4,
  );

  $node_tabs['sirius_worker']['sirius_worker_dispatches/dnc'] = array(
    'title' => 'DNC',
    'description' => '',
    'callback' => 'sirius_node_tab_worker_dnc',
    'permissions' => array('sirius dispatch staff'),
    'weight' => 5,
  );

  $node_tabs['grievance_shop']['sirius_employer_dispatch_preferences'] = array(
    'title' => 'Dispatch Preferences',
    'description' => '',
    'callback' => 'sirius_node_tab_employer_dispatch_preferences',
    'permissions' => array('sirius dispatch employer'),
  );

  return $node_tabs;
}

function sirius_dispatch_term_tabs() {
  $term_tabs = array();

  $term_tabs['sirius_industry']['sirius_dispatch_industry'] = array(
    'title' => 'Dispatch',
    'description' => '',
    'callback' => 'sirius_dispatch_term_tab_industry',
    'permissions' => array('sirius dispatch staff'),
    'weight' => 4,
  );

  return $term_tabs;
}
/**
 * Implements hook_menu
 */

function sirius_dispatch_menu() {
  $items = array();

  $items['sirius/staff/dispatch/operator'] = array(
    'title' => 'Sirius: Dispatch: Operator',
    'description' => '',
    'page callback' => 'sirius_dispatch_page_operator',
    'page arguments' => array(),
    'access arguments' => array('sirius dispatch admin'),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/staff/dispatch/operator/ajax/%'] = array(
    'title' => 'Sirius: Dispatch: Operator: Ajax',
    'description' => '',
    'page callback' => 'sirius_dispatch_page_operator_ajax',
    'page arguments' => array(4),
    'access arguments' => array('sirius dispatch admin'),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/dispatch/job/create'] = array(
    'title' => 'Create a Job',
    'description' => '',
    'page callback' => 'sirius_dispatch_page_job_create_intro',
    'page arguments' => array(),
    'access arguments' => array('sirius dispatch employer'),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/dispatch/job/create/%node/%'] = array(
    'title' => 'Create a Job',
    'description' => '',
    'page callback' => 'sirius_dispatch_page_job_create',
    'page arguments' => array(4, 5),
    'access callback' => 'sirius_dispatch_page_job_create_access',
    'access arguments' => array(4, 5),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/employer/dispatch'] = array(
    'title' => 'Sirius: Employer: Dispatch',
    'description' => '',
    'page callback' => 'sirius_dispatch_page_employer_landing',
    'page arguments' => array(4),
    'access arguments' => array('sirius dispatch employer'),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/employer/dispatch/job/list'] = array(
    'title' => 'Sirius: Employer: Dispatch: Job: List',
    'description' => '',
    'page callback' => 'sirius_dispatch_page_employer_job_list',
    'page arguments' => array(4),
    'access arguments' => array('sirius dispatch employer'),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/employer/dispatch/vlri'] = array(
    'title' => 'Sirius: Employer: Layoff Requests',
    'description' => '',
    'page callback' => 'sirius_dispatch_page_employer_vlri',
    'page arguments' => array(),
    'access arguments' => array('sirius dispatch employer'),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/dispatch/config/vlri'] = array(
    'title' => 'Layoff Request Settings',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('sirius_dispatch_vlri_admin'),
    'access arguments' => array('sirius dispatch admin'),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/staff/dispatch'] = array(
    'title' => 'Sirius: Staff: Dispatch',
    'description' => '',
    'page callback' => 'sirius_dispatch_page_landing',
    'page arguments' => array(4),
    'access arguments' => array('sirius dispatch staff'),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/employer/dispatch/job/view/ajax/%node'] = array(
    'title' => 'Sirius: Employer: Dispatch: Job: View: Ajax',
    'description' => '',
    'page callback' => 'sirius_dispatch_job_employer_view_ajax',
    'page arguments' => array(6),
    'access callback' => 'sirius_node_tab_dispatch_job_employer_view_access',
    'access arguments' => array(6),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/dispatch/config/run'] = array(
    'title' => 'Job Settings',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('sirius_dispatch_run_admin'),
    'access arguments' => array('sirius dispatch admin'),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/dispatch/config/view'] = array(
    'title' => 'Dispatch View Settings',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('sirius_dispatch_view_admin'),
    'access arguments' => array('sirius dispatch admin'),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/dispatch/config/threestrikes'] = array(
    'title' => 'Three-Strikes Settings',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('sirius_dispatch_threestrikes_admin'),
    'access arguments' => array('sirius dispatch admin'),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/dispatch/jwa'] = array(
    'title' => 'Job / Worker Assign',
    'page callback' => 'sirius_dispatch_page_jwa_assign',
    'page arguments' => array(),
    'access arguments' => array('sirius dispatch staff'),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/dispatch/jwa/assign'] = array(
    'title' => 'Assign',
    'page callback' => 'sirius_dispatch_page_jwa_assign',
    'page arguments' => array(),
    'access arguments' => array('sirius dispatch staff'),
    'type' => MENU_DEFAULT_LOCAL_TASK,
  );

  $items['sirius/dispatch/jwa/prefs'] = array(
    'title' => 'Preferences',
    'page callback' => 'sirius_dispatch_page_jwa_prefs',
    'page arguments' => array(),
    'access arguments' => array('sirius dispatch staff'),
    'type' => MENU_LOCAL_TASK,
  );

  $items['sirius/dispatch/eba/cleanup'] = array(
    'title' => 'EBA Cleanup',
    'page callback' => 'sirius_dispatch_page_eba_cleanup',
    'page arguments' => array(),
    'access arguments' => array('sirius dispatch admin'),
    'type' => MENU_LOCAL_TASK,
  );

  return $items;
}
