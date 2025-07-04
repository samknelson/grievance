<?php

/**
 * Tabs: Which node type has which tabs. 
 *
 * Much of the functionality is via local tasks in nodes.  For instance, nodes of type "worker" have a local tab called "housing" which 
 * shows housing-related functions. 
 */
 
function sirius_hour_node_tabs() {
  $node_tabs = array();

  $node_tabs['sirius_payperiod']['sirius_payperiod_simple'] = array(
    'title' => 'Hours',
    'description' => '',
    'callback' => 'sirius_hour_node_tab_payperiod_simple',
    'permissions' => array('sirius'),
    'weight' => 0,
    'component' => 'hour:simple',
    'file' => 'sirius_hour.page_payperiod_simple.inc',
  );

  $node_tabs['sirius_payperiod']['sirius_payperiod_entries_summary'] = array(
    'title' => 'Totals',
    'description' => '',
    'callback' => 'sirius_hour_node_tab_payperiod_entries_summary',
    'permissions' => array('sirius hour user'),
    'weight' => 0,
    'component' => 'hour:complex',
  );

  $node_tabs['sirius_payperiod']['sirius_payperiod_entries_view'] = array(
    'title' => 'List Entries',
    'description' => '',
    'callback' => 'sirius_hour_node_tab_payperiod_entries_view',
    'permissions' => array('sirius hour user'),
    'weight' => 1,
    'component' => 'hour:complex', 
  );

  $node_tabs['sirius_payperiod']['sirius_payperiod_entries_edit'] = array(
    'title' => 'Edit Entries',
    'description' => '',
    'callback' => 'sirius_hour_node_tab_payperiod_entries_edit',
    'permissions' => array('sirius hour admin'),
    'weight' => 2,
    'component' => 'hour:complex',
  );

  $node_tabs['sirius_payperiod']['sirius_payperiod_entries_reconcile'] = array(
    'title' => 'Reconcile',
    'description' => '',
    'callback' => 'sirius_hour_node_tab_payperiod_entries_reconcile',
    'permissions' => array('sirius hour admin'),
    'weight' => 3,
    'component' => 'hour:reconcile',
  );

  $node_tabs['sirius_worker']['sirius_worker_hours'] = array(
    'title' => 'Hours',
    'description' => '',
    'callback' => 'sirius_hour_node_tab_worker_hours',
    'permissions' => array('sirius'),
    'component' => 'hour',
  );

  $node_tabs['grievance_shop']['sirius_employer_hours'] = array(
    'title' => 'Transmittal Sheet Hours',
    'description' => '',
    'callback' => 'sirius_hour_node_tab_employer_hours',
    'permissions' => array('sirius hour admin'),
    'component' => 'hour:transmittal',
  );

  sirius_node_tab_set_module('sirius_hour', $node_tabs);

  return $node_tabs;
}

/**
 * Implements hook_menu
 */

function sirius_hour_menu() {
  $items = array();

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

  $items['sirius/hour/search/month'] = array(
    'title' => 'Hours - Search - Monthly',
    'description' => '',
    'page callback' => 'sirius_hour_page_search_month',
    'page arguments' => array(),
    'type' => MENU_NORMAL_ITEM,
    'file' => 'sirius_hour.page_search_bymonth.inc',
    'access callback' => 'sirius_menu_access_callback',
    'access arguments' => array(
      array(
        'permission' => 'sirius hour admin',
        'component' => 'hour',
      ),
    ),
  );

  $items['sirius/hour/config'] = array(
    'title' => 'Hour Configuration',
    'description' => '',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('sirius_hour_config'),
    'type' => MENU_NORMAL_ITEM,
    'access callback' => 'sirius_menu_access_callback',
    'access arguments' => array(
      array(
        'permission' => 'sirius hour admin',
        'component' => 'hour',
      ),
    ),
  );

  return $items;
}

