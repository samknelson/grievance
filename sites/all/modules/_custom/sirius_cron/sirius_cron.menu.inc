<?php


/**
 * Implements hook_sirius_node_tabs()
 */
 
function sirius_cron_node_tabs() {
  $node_tabs = array();

  $node_tabs['sirius_json_definition']['sirius_cron_scan_info'] = array(
    'title' => 'Info',
    'description' => '',
    'callback' => 'sirius_cron_node_tab_scan_info',
    'permissions' => array('sirius admin'),
    'weight' => 1,
    'file' => 'sirius_cron.page_scan_info.inc',
  );

  $node_tabs['sirius_json_definition']['sirius_cron_scan_run'] = array(
    'title' => 'Run',
    'description' => '',
    'callback' => 'sirius_cron_node_tab_scan_run',
    'permissions' => array('sirius admin'),
    'weight' => 2,
    'file' => 'sirius_cron.page_scan_run.inc',
  );

  sirius_node_tab_set_module('sirius_cron', $node_tabs);

  return $node_tabs;
}

/**
 * Implements hook_menu()
 */

function sirius_cron_menu() {
  $items = array();

  $items['sirius/cron'] = array(
    'title' => 'Cron Management',
    'description' => '',
    'page callback' => 'sirius_cron_page_scan_list',
    'page arguments' => array(),
    'access arguments' => array('sirius admin'),
    'type' => MENU_NORMAL_ITEM,
    'file' => 'sirius_cron.page_scan_list.inc',
  );

  /*
  $items['sirius/cron/list'] = array(
    'title' => 'List',
    'description' => '',
    'page callback' => 'sirius_cron_page_scan_list',
    'page arguments' => array(),
    'access arguments' => array('sirius admin'),
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'file' => 'sirius_cron.page_scan_list.inc',
    'weight' => 1,
  );
  */

  return $items;
}