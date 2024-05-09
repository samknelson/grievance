<?php

/**
 * Tabs: Which node type has which tabs. 
 *
 * Much of the functionality is via local tasks in nodes.  For instance, nodes of type "worker" have a local tab called "housing" which 
 * shows housing-related functions. 
 */
 
function sirius_denorm_node_tabs() {
  $node_tabs = array();

  /*
  $node_tabs['sirius_operator']['sirius_operator_settings'] = array(
    'title' => 'Settings',
    'description' => '',
    'callback' => 'sirius_office_node_tab_operator_settings',
    'permissions' => array('sirius office operator admin'),
    'weight' => 1,
    'file' => 'sirius_office.page_operator_settings.inc',
  );
  */

  sirius_node_tab_set_module('sirius_denorm', $node_tabs);

  return $node_tabs;
}

function sirius_denorm_menu() {
  $items = array();

  $items['sirius/denorm'] = array(
    'title' => 'Denormalize',
    'description' => '',
    'page callback' => 'sirius_denorm_page_settings',
    'page arguments' => array(),
    'access arguments' => array('sirius denorm admin'),
    'type' => MENU_NORMAL_ITEM,
    'file' => 'sirius_denorm.page_settings.inc',
  );

  $items['sirius/denorm/settings'] = array(
    'title' => 'Settings',
    'description' => '',
    'page callback' => 'sirius_denorm_page_settings',
    'page arguments' => array(),
    'access arguments' => array('sirius denorm admin'),
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'file' => 'sirius_denorm.page_settings.inc',
    'weight' => 1,
  );

  $items['sirius/denorm/queue_summary'] = array(
    'title' => 'Queue Summary',
    'description' => '',
    'page callback' => 'sirius_denorm_page_queue_summary',
    'page arguments' => array(),
    'access arguments' => array('sirius denorm admin'),
    'type' => MENU_LOCAL_TASK,
    'file' => 'sirius_denorm.page_queue_summary.inc',
    'weight' => 2,
  );

  $items['sirius/denorm/queue'] = array(
    'title' => 'Queue Details',
    'description' => '',
    'page callback' => 'sirius_denorm_page_queue',
    'page arguments' => array(),
    'access arguments' => array('sirius denorm admin'),
    'type' => MENU_LOCAL_TASK,
    'file' => 'sirius_denorm.page_queue.inc',
    'weight' => 3,
  );

  return $items;
}
