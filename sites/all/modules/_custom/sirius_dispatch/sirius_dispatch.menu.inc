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
  );

  $node_tabs['sirius_dispatch_job']['sirius_dispatch_job_attachments'] = array(
    'title' => 'Attachments',
    'description' => '',
    'callback' => 'sirius_node_tab_dispatch_job_attachments',
    'permissions' => array('sirius dispatch employer'),
    'weight' => 1,
  );


  $node_tabs['sirius_dispatch_job']['sirius_dispatch_bullpen'] = array(
    'title' => 'Bullpen Details',
    'description' => '',
    'callback' => 'sirius_node_tab_dispatch_bullpen',
    'permissions' => array('sirius dispatch bullpen view'),
    'weight' => 1,
  );

  $node_tabs['sirius_dispatch_job']['sirius_dispatch_bullpen_signin_self'] = array(
    'title' => 'Bullpen EBA Signin',
    'description' => '',
    'callback' => 'sirius_node_tab_dispatch_bullpen_signin_self',
    'permissions' => array('sirius dispatch bullpen signin'),
    'weight' => 2,
  );

  $node_tabs['sirius_dispatch_job']['sirius_dispatch_bullpen_signin_others'] = array(
    'title' => 'Bullpen Signin',
    'description' => '',
    'callback' => 'sirius_node_tab_dispatch_bullpen_signin_others',
    'permissions' => array('sirius dispatch bullpen signin view'),
    'weight' => 3,
  );

  $node_tabs['sirius_dispatch_job']['sirius_dispatch_bullpen_signin_others'] = array(
    'title' => 'Bullpen List',
    'description' => '',
    'callback' => 'sirius_node_tab_dispatch_bullpen_signin_others',
    'permissions' => array('sirius dispatch bullpen signin view'),
    'weight' => 4,
  );

  /*
  $node_tabs['sirius_dispatch_job']['sirius_dispatch_bullpen_open'] = array(
    'title' => 'Dispatch',
    'description' => '',
    'callback' => 'sirius_node_tab_dispatch_bullpen',
    'permissions' => array('sirius dispatch bullpen view'),
    'weight' => 4,
  );

  $node_tabs['sirius_dispatch_job']['sirius_dispatch_bullpen_cbn'] = array(
    'title' => 'Dispatch by Name',
    'description' => '',
    'callback' => 'sirius_node_tab_dispatch_bullpen',
    'permissions' => array('sirius dispatch bullpen view'),
    'weight' => 5,
  );
  */

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

/**
 * Implements hook_menu
 */

function sirius_dispatch_menu() {
  $items = array();

  //
  // Node Tabs
  // 

  $node_tabs = sirius_dispatch_node_tabs();
  foreach ($node_tabs as $node_type => $node_record) {
    foreach ($node_record as $tab_name => $tab_record) {
      $items['node/%node/' . $tab_name] = array(
        'title' => $tab_record['title'],
        'description' => $tab_record['description'],
        'page callback' => 'sirius_dispatch_node_tab_content',
        'page arguments' => array(1, $tab_name),
        'access callback' => 'sirius_dispatch_node_tab_access',
        'access arguments' => array(1, $tab_name),
        'type' => MENU_LOCAL_TASK,
      );
      if ($tab_record['weight']) {
        $items['node/%node/' . $tab_name]['weight'] = $tab_record['weight'];
      }
    }
  }

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

  $items['sirius/dispatch/config/bullpen'] = array(
    'title' => 'Bullpen Settings',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('sirius_dispatch_bullpen_admin'),
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

  $items['sirius/ajax/bullpen-quicksearch'] = array(
    'page callback' => 'grievance_quicksearch_autocomplete',
    'page arguments' => array(),
    'access arguments' => array('sirius dispatch bullpen signin others'),
    'type' => MENU_CALLBACK,
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

  return $items;
}

function sirius_dispatch_node_tab_content($node, $in_tab_name) {
  $node_tabs = sirius_dispatch_node_tabs();
  foreach ($node_tabs as $node_type => $node_record) {
    if ($node->type == $node_type) {
      foreach ($node_record as $tab_name => $tab_record) {
        if ($tab_name == $in_tab_name) {
          $callback = $tab_record['callback'];
          return $callback($node);
        }
      }
    }
  }
  
  drupal_set_message("Bad tab: $node->type, $in_tab_name");
  
  return "";
}

function sirius_dispatch_node_tab_access($node, $in_tab_name) {
  // Check generic permissions
  $node_tabs = sirius_dispatch_node_tabs();
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
