<?php


/**
 * Implements hook_sirius_node_tabs()
 */
 
function sirius_steward_node_tabs() {
  $node_tabs = array();

  $node_tabs['sirius_worker']['sirius_worker_steward'] = array(
    'title' => 'Steward',
    'description' => '',
    'callback' => 'sirius_steward_node_tab_worker_checkin_list',
    'permissions' => array('sirius'),
    'file' => 'sirius_steward.page_worker_checkin_list.inc',
    'component' => 'steward:checkin',
    'skip_node_view_check' => TRUE,
  );

  $node_tabs['sirius_worker']['sirius_worker_steward/list'] = array(
    'title' => 'List',
    'description' => '',
    'callback' => 'sirius_steward_node_tab_worker_checkin_list',
    'permissions' => array('sirius'),
    'weight' => 1,
    'file' => 'sirius_steward.page_worker_checkin_list.inc',
    'component' => 'steward:checkin',
    'skip_node_view_check' => TRUE,
    'type' => MENU_DEFAULT_LOCAL_TASK,
  );

  $node_tabs['sirius_worker']['sirius_worker_steward/checkin'] = array(
    'title' => 'Check In',
    'description' => '',
    'callback' => 'sirius_steward_node_tab_worker_checkin',
    'permissions' => array('sirius steward'),
    'weight' => 2,
    'file' => 'sirius_steward.page_worker_checkin.inc',
    'component' => 'steward:checkin',
    'skip_node_view_check' => TRUE,
  );

  $node_tabs['sirius_worker']['sirius_worker_steward/checkin-history'] = array(
    'title' => 'Check-in History',
    'description' => '',
    'callback' => 'sirius_steward_node_tab_worker_checkin_history',
    'permissions' => array('sirius steward admin'),
    'weight' => 3,
    'file' => 'sirius_steward.page_worker_checkin_history.inc',
    'component' => 'steward:checkin',
    'skip_node_view_check' => TRUE,
    'type' => MENU_LOCAL_TASK,
  );


  $node_tabs['grievance_shop']['sirius_employer_steward'] = array(
    'title' => 'Steward',
    'description' => '',
    'callback' => 'sirius_steward_node_tab_employer_checkin_list',
    'permissions' => array('sirius steward'),
    'file' => 'sirius_steward.page_employer_checkin_list.inc',
    'component' => 'steward:checkin',
    'skip_node_view_check' => TRUE,
  );

  $node_tabs['grievance_shop']['sirius_employer_steward/list'] = array(
    'title' => 'List',
    'description' => '',
    'callback' => 'sirius_steward_node_tab_employer_checkin_list',
    'permissions' => array('sirius steward'),
    'file' => 'sirius_steward.page_employer_checkin_list.inc',
    'component' => 'steward:checkin',
    'skip_node_view_check' => TRUE,
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => 1,
  );

  $node_tabs['grievance_shop']['sirius_employer_steward/sched'] = array(
    'title' => 'Schedule',
    'description' => '',
    'callback' => 'sirius_steward_node_tab_employer_sched',
    'permissions' => array('sirius steward sched'),
    'file' => 'sirius_steward.page_employer_sched.inc',
    'component' => 'steward:sched',
    'skip_node_view_check' => TRUE,
    'type' => MENU_LOCAL_TASK,
    'weight' => 2,
  );



  $node_tabs['sirius_dispatch_facility']['sirius_steward_dispatch_facility_checkin'] = array(
    'title' => 'Check In',
    'description' => '',
    'callback' => 'sirius_steward_node_tab_facility_checkin',
    'permissions' => array('sirius steward admin'),
    'weight' => 1,
    'file' => 'sirius_steward.page_facility_checkin.inc',
    'component' => 'steward:checkin',
    'skip_node_view_check' => TRUE,
  );

  $node_tabs['sirius_dispatch_facility']['sirius_steward_dispatch_facility_checkin/current'] = array(
    'title' => 'Current',
    'description' => '',
    'callback' => 'sirius_steward_node_tab_facility_checkin',
    'permissions' => array('sirius steward admin'),
    'weight' => 1,
    'file' => 'sirius_steward.page_facility_checkin.inc',
    'component' => 'steward:checkin',
    'skip_node_view_check' => TRUE,
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => 1,
  );

  $node_tabs['sirius_dispatch_facility']['sirius_steward_dispatch_facility_checkin/history'] = array(
    'title' => 'History',
    'description' => '',
    'callback' => 'sirius_steward_node_tab_facility_checkin_history',
    'permissions' => array('sirius steward admin'),
    'weight' => 1,
    'file' => 'sirius_steward.page_facility_checkin_history.inc',
    'component' => 'steward:checkin',
    'skip_node_view_check' => TRUE,
    'type' => MENU_LOCAL_TASK,
    'weight' => 2,
  );

  $node_tabs['sirius_log']['sirius_steward_log_checkin'] = array(
    'title' => 'Check In',
    'description' => '',
    'callback' => 'sirius_steward_node_tab_log_checkin',
    'permissions' => array('sirius steward admin'),
    'weight' => 1,
    'file' => 'sirius_steward.page_log_checkin.inc',
    'component' => 'steward:checkin',
    'skip_node_view_check' => TRUE,
  );

  $node_tabs['sirius_log']['sirius_steward_log_sched'] = array(
    'title' => 'Schedule',
    'description' => '',
    'callback' => 'sirius_steward_node_tab_log_sched',
    'permissions' => array('sirius steward admin'),
    'weight' => 1,
    'file' => 'sirius_steward.page_log_sched.inc',
    'component' => 'steward:sched',
    'skip_node_view_check' => TRUE,
  );

  sirius_node_tab_set_module('sirius_steward', $node_tabs);

  return $node_tabs;
}

/**
 * Implements hook_menu()
 */

function sirius_steward_menu() {
  $items = array();

  $items['sirius/steward/config'] = array(
    'title' => 'Steward Settings',
    'description' => '',
    'page callback' => 'sirius_steward_page_settings',
    'page arguments' => array(),
    'access arguments' => array('sirius steward admin'),
    'type' => MENU_NORMAL_ITEM,
    'file' => 'sirius_steward.page_settings.inc',
  );

  $items['sirius/steward/config/general'] = array(
    'title' => 'General',
    'description' => '',
    'page callback' => 'sirius_steward_page_settings',
    'page arguments' => array(),
    'access arguments' => array('sirius steward admin'),
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'file' => 'sirius_steward.page_settings.inc',
    'weight' => 0,
  );

  $items['sirius/steward/config/checkin'] = array(
    'title' => 'Checkin',
    'description' => '',
    'page callback' => 'sirius_steward_page_settings_checkin',
    'page arguments' => array(),
    'access arguments' => array('sirius steward admin'),
    'type' => MENU_LOCAL_TASK,
    'file' => 'sirius_steward.page_settings_checkin.inc',
    'weight' => 1,
  );

  $items['sirius/steward/config/sched'] = array(
    'title' => 'Schedule',
    'description' => '',
    'page callback' => 'sirius_steward_page_settings_sched',
    'page arguments' => array(),
    'access arguments' => array('sirius steward admin'),
    'type' => MENU_LOCAL_TASK,
    'file' => 'sirius_steward.page_settings_sched.inc',
    'weight' => 2,
  );

  $items['sirius/steward/checkin/list'] = array(
    'title' => 'Active Steward Check-Ins',
    'description' => '',
    'page callback' => 'sirius_steward_page_checkin_list',
    'page arguments' => array(),
    'access arguments' => array('sirius steward admin'),
    'type' => MENU_NORMAL_ITEM,
    'file' => 'sirius_steward.page_checkin_list.inc',
  );

  return $items;
}
