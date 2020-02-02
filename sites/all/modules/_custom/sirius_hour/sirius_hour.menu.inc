<?php

/**
 * Tabs: Which node type has which tabs. 
 *
 * Much of the functionality is via local tasks in nodes.  For instance, nodes of type "worker" have a local tab called "housing" which 
 * shows housing-related functions. 
 */
 
function sirius_hour_node_tabs() {
  $node_tabs = array();

  $node_tabs['sirius_payperiod']['sirius_payperiod_entries'] = array(
    'title' => 'Entries',
    'description' => '',
    'callback' => 'sirius_hour_node_tab_payperiod_entries',
    'permissions' => array('sirius hour user'),
    'weight' => 0,
  );

  $node_tabs['sirius_payperiod']['sirius_payperiod_entries/list'] = array(
    'title' => 'View',
    'description' => '',
    'callback' => 'sirius_hour_node_tab_payperiod_entries_list',
    'permissions' => array('sirius hour user'),
    'weight' => 1,
  );

  $node_tabs['sirius_payperiod']['sirius_payperiod_entries/edit'] = array(
    'title' => 'Edit',
    'description' => '',
    'callback' => 'sirius_hour_node_tab_payperiod_entries_edit',
    'permissions' => array('sirius hour admin'),
    'weight' => 2,
  );

  $node_tabs['sirius_payperiod']['sirius_payperiod_entries/totals'] = array(
    'title' => 'Totals',
    'description' => '',
    'callback' => 'sirius_hour_node_tab_payperiod_entries_totals',
    'permissions' => array('sirius hour user'),
    'weight' => 3,
  );

  $node_tabs['sirius_payperiod']['sirius_payperiod_entries/monthly'] = array(
    'title' => 'Monthly',
    'description' => '',
    'callback' => 'sirius_hour_node_tab_payperiod_entries_monthly',
    'permissions' => array('sirius hour user'),
    'weight' => 4,
  );

  $node_tabs['sirius_payperiod']['sirius_payperiod_entries/daily'] = array(
    'title' => 'Daily',
    'description' => '',
    'callback' => 'sirius_hour_node_tab_payperiod_entries_daily',
    'permissions' => array('sirius hour user'),
    'weight' => 5,
  );

  $node_tabs['sirius_worker']['sirius_worker_hours'] = array(
    'title' => 'Hours',
    'description' => '',
    'callback' => 'sirius_hour_node_tab_worker_hours',
    'permissions' => array('sirius hour admin'),
  );

  /*
  $node_tabs['sirius_payperiod']['sirius_payperiod_entries/accrual'] = array(
    'title' => 'Accrual',
    'description' => '',
    'callback' => 'sirius_hour_node_tab_payperiod_entries_accrual',
    'permissions' => array('sirius hour user'),
    'weight' => 6,
  );
  */

  return $node_tabs;
}

/**
 * Implements hook_menu
 */

function sirius_hour_menu() {
  $items = array();

  $items['sirius/hour/config'] = array(
    'title' => 'Hour Configuration',
    'description' => '',
    'access arguments' => array('sirius hour admin'),
    'page callback' => 'drupal_get_form',
    'page arguments' => array('sirius_hour_config'),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/hour/payperiod/requested'] = array(
    'title' => 'Payperiods - Pending',
    'description' => '',
    'page callback' => 'sirius_hour_payperiod_list_requested',
    'page arguments' => array(),
    'access arguments' => array('sirius hour admin'),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/hour/payperiod/all'] = array(
    'title' => 'Payperiods - All',
    'description' => '',
    'page callback' => 'sirius_hour_payperiod_list_all',
    'page arguments' => array(),
    'access arguments' => array('sirius hour admin'),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/hour/payperiod/mine'] = array(
    'title' => 'Payperiods - Mine',
    'description' => '',
    'page callback' => 'sirius_hour_payperiod_list_mine',
    'page arguments' => array(),
    'access arguments' => array('sirius hour user'),
    'type' => MENU_NORMAL_ITEM,
  );

  return $items;
}

