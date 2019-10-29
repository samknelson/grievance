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

  //
  // Node Tabs
  // 

  $node_tabs = sirius_hour_node_tabs();
  foreach ($node_tabs as $node_type => $node_record) {
    foreach ($node_record as $tab_name => $tab_record) {
      $items['node/%node/' . $tab_name] = array(
        'title' => $tab_record['title'],
        'description' => $tab_record['description'],
        'page callback' => 'sirius_hour_node_tab_content',
        'page arguments' => array(1, $tab_name),
        'access callback' => 'sirius_hour_node_tab_access',
        'access arguments' => array(1, $tab_name),
        'type' => MENU_LOCAL_TASK,
      );
      if ($tab_record['weight']) {
        $items['node/%node/' . $tab_name]['weight'] = $tab_record['weight'];
      }
      if ($tab_record['page arguments']) {
        $items['node/%node/' . $tab_name]['page_arguments'] = array_merge($items['node/%node/' . $tab_name]['page_arguments'], $tab_record['page arguments']);
      }
    }
  }


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

function sirius_hour_node_tab_content($node, $in_tab_name) {
  $node_tabs = sirius_hour_node_tabs();
  foreach ($node_tabs as $node_type => $node_record) {
    if ($node->type == $node_type) {
      foreach ($node_record as $tab_name => $tab_record) {
        if ($tab_name == $in_tab_name) {
          $callback = $tab_record['callback'];
          $args = array($node);
          if ($tab_record['callback arguments']) {
            $args = array_merge($args, $tab_record['callback arguments']);
          }
          return call_user_func_array($callback, $args);
        }
      }
    }
  }
  
  drupal_set_message("Bad tab: $node->type, $in_tab_name");
  
  return "";
}

function sirius_hour_node_tab_access($node, $in_tab_name) {
  // Check generic permissions
  $node_tabs = sirius_hour_node_tabs();
  foreach ($node_tabs as $node_type => $node_record) {
    if ($node->type == $node_type) {
      foreach ($node_record as $tab_name => $tab_record) {
        if ($tab_name == $in_tab_name) {
          // Check basic permission
          foreach ($tab_record['permissions'] as $permission) {
            if (!user_access($permission)) { return false; }
          }

          // Check for a custom access permission
          $access_function_name = $tab_record['callback'] . '_access';
          if (function_exists($access_function_name)) {
            if (!$access_function_name($node)) {
              return false;
            }
          }
          
          return true;
        }
      }
    }
  }
  
  return false;
}
