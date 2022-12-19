<?php


/**
 * Implements hook_sirius_node_tabs()
 */
 
function sirius_steward_node_tabs() {
  $node_tabs = array();

  $node_tabs['sirius_worker']['sirius_steward_worker_facility_checkin'] = array(
    'title' => 'Checkin',
    'description' => '',
    'callback' => 'sirius_steward_node_tab_worker_facility_checkin',
    'permissions' => array('sirius steward'),
    'weight' => 1,
    'file' => 'sirius_steward.page_worker_facility_checkin.inc',
    'component' => 'steward:facility:checkin',
    'skip_node_view_check' => TRUE,
  );

  $node_tabs['sirius_worker']['sirius_steward_worker_facility_checkin/current'] = array(
    'title' => 'Current',
    'description' => '',
    'callback' => 'sirius_steward_node_tab_worker_facility_checkin',
    'permissions' => array('sirius steward'),
    'weight' => 1,
    'file' => 'sirius_steward.page_worker_facility_checkin.inc',
    'component' => 'steward:facility:checkin',
    'skip_node_view_check' => TRUE,
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => 1,
  );

  $node_tabs['sirius_worker']['sirius_steward_worker_facility_checkin/history'] = array(
    'title' => 'History',
    'description' => '',
    'callback' => 'sirius_steward_node_tab_worker_facility_checkin_history',
    'permissions' => array('sirius steward admin'),
    'weight' => 1,
    'file' => 'sirius_steward.page_worker_facility_checkin_history.inc',
    'component' => 'steward:facility:checkin',
    'skip_node_view_check' => TRUE,
    'type' => MENU_LOCAL_TASK,
    'weight' => 2,
  );

  $node_tabs['sirius_log']['sirius_steward_log_facility_checkin'] = array(
    'title' => 'Checkin',
    'description' => '',
    'callback' => 'sirius_steward_node_tab_log_facility_checkin',
    'permissions' => array('sirius steward admin'),
    'weight' => 1,
    'file' => 'sirius_steward.page_log_facility_checkin.inc',
    'component' => 'steward:facility:checkin',
    'skip_node_view_check' => TRUE,
  );

  $node_tabs['sirius_dispatch']['sirius_dispatch_steward_list'] = array(
    'title' => 'Stewards',
    'description' => '',
    'callback' => 'sirius_dispatch_steward_list',
    'permissions' => array('sirius'),
    'weight' => 1,
    'file' => 'sirius_steward.page_dispatch_steward_list.inc',
    'component' => 'steward:facility:checkin',
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

  /*
  $items['sirius/cron/list'] = array(
    'title' => 'List',
    'description' => '',
    'page callback' => 'sirius_steward_page_scan_list',
    'page arguments' => array(),
    'access arguments' => array('sirius admin'),
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'file' => 'sirius_steward.page_scan_list.inc',
    'weight' => 1,
  );
  */

  return $items;
}
