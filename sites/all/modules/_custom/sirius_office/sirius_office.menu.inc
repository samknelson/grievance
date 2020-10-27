<?php

/**
 * Tabs: Which node type has which tabs. 
 *
 * Much of the functionality is via local tasks in nodes.  For instance, nodes of type "worker" have a local tab called "housing" which 
 * shows housing-related functions. 
 */
 
function sirius_office_node_tabs() {
  $node_tabs = array();

  $node_tabs['sirius_operator']['sirius_operator_users'] = array(
    'title' => 'Users',
    'description' => '',
    'callback' => 'sirius_office_node_tab_operator_users',
    'permissions' => array('sirius office operator admin'),
    'weight' => 1,
  );

  $node_tabs['sirius_operator']['sirius_operator_status'] = array(
    'title' => 'Status',
    'description' => '',
    'callback' => 'sirius_office_node_tab_operator_status',
    'permissions' => array('sirius office operator user'),
    'weight' => 2,
  );

  $node_tabs['sirius_operator']['sirius_operator_calls'] = array(
    'title' => 'Incoming Calls',
    'description' => '',
    'callback' => 'sirius_office_node_tab_operator_calls',
    'permissions' => array('sirius office operator user'),
    'weight' => 3,
  );

  return $node_tabs;
}

function sirius_office_menu() {
  $items = array();

  $items['sirius/office/iprestrict/admin'] = array(
    'title' => 'IP Restrictions',
    'description' => '',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('sirius_office_iprestrict_admin'),
    'access arguments' => array('sirius office iprestrict admin'),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/office/timeoff'] = array(
    'title' => 'Timeoff Requests',
    'description' => '',
    'page callback' => 'sirius_office_page_timeoff_request',
    'page arguments' => array(),
    'access arguments' => array('sirius office timeoff request'),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/office/timeoff/pending'] = array(
    'title' => 'Pending',
    'description' => '',
    'page callback' => 'sirius_office_page_timeoff_request_list_pending',
    'page arguments' => array(),
    'access arguments' => array('sirius office timeoff admin'),
    'type' => MENU_LOCAL_TASK,
    'weight' => 0,
  );

  $items['sirius/office/timeoff/all'] = array(
    'title' => 'All',
    'description' => '',
    'page callback' => 'sirius_office_page_timeoff_request_list',
    'page arguments' => array(),
    'access arguments' => array('sirius office timeoff admin'),
    'type' => MENU_LOCAL_TASK,
    'weight' => 1,
  );

  $items['sirius/office/timeoff/mine'] = array(
    'title' => 'Mine',
    'description' => '',
    'page callback' => 'sirius_office_page_timeoff_request_list_mine',
    'page arguments' => array(),
    'access arguments' => array('sirius office timeoff request'),
    'type' => MENU_LOCAL_TASK,
    'weight' => 2,
  );

  $items['sirius/office/operator'] = array(
    'title' => 'Operators',
    'description' => '',
    'page callback' => 'sirius_office_page_operator',
    'page arguments' => array(),
    'access arguments' => array('sirius'),
    'type' => MENU_NORMAL_ITEM,
  );

  return $items;
}
