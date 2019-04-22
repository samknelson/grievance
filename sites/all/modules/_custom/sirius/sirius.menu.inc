<?php

/**
 * Tabs: Which node type has which tabs. 
 *
 * Much of the functionality is via local tasks in nodes.  For instance, nodes of type "worker" have a local tab called "housing" which 
 * shows housing-related functions. 
 */
 
function sirius_node_tabs() {
  $node_tabs = array();

  $node_tabs['sirius_worker']['sirius_worker_log'] = array(
    'title' => 'Log',
    'description' => '',
    'callback' => 'sirius_node_tab_entity_log',
    'permissions' => array('sirius dispatch staff'),
  );

  $node_tabs['sirius_dispatch']['sirius_dispatch_log'] = array(
    'title' => 'Log',
    'description' => '',
    'callback' => 'sirius_node_tab_entity_log',
    'permissions' => array('sirius dispatch staff'),
  );

  $node_tabs['sirius_dispatch_job']['sirius_dispatch_job_log'] = array(
    'title' => 'Log',
    'description' => '',
    'callback' => 'sirius_node_tab_entity_log',
    'permissions' => array('sirius dispatch staff'),
  );

  $node_tabs['sirius_dispatch_facility']['sirius_dispatch_facility_log'] = array(
    'title' => 'Log',
    'description' => '',
    'callback' => 'sirius_node_tab_entity_log',
    'permissions' => array('sirius dispatch staff'),
  );

  $node_tabs['sirius_dispatch_facility_hall']['sirius_dispatch_facility_hall_log'] = array(
    'title' => 'Log',
    'description' => '',
    'callback' => 'sirius_node_tab_entity_log',
    'permissions' => array('sirius dispatch staff'),
  );

  $node_tabs['grievance_shop']['grievance_shop_log'] = array(
    'title' => 'Log',
    'description' => '',
    'callback' => 'sirius_node_tab_entity_log',
    'permissions' => array('sirius dispatch staff'),
  );

  $node_tabs['sirius_feed']['sirius_feed_log'] = array(
    'title' => 'Log',
    'description' => '',
    'callback' => 'sirius_node_tab_entity_log',
    'permissions' => array('sirius dispatch staff'),
  );


  $node_tabs['sirius_feed']['sirius_feed_process'] = array(
    'title' => 'Process',
    'description' => '',
    'callback' => 'sirius_node_tab_feed_process',
    'permissions' => array('sirius admin'),
  );

  return $node_tabs;
}

/**
 * Implements hook_menu
 */

function sirius_menu() {
  $items = array();

  //
  // Node Tabs
  // 

  $node_tabs = sirius_node_tabs();
  foreach ($node_tabs as $node_type => $node_record) {
    foreach ($node_record as $tab_name => $tab_record) {
      $items['node/%node/' . $tab_name] = array(
        'title' => $tab_record['title'],
        'description' => $tab_record['description'],
        'page callback' => 'sirius_node_tab_content',
        'page arguments' => array(1, $tab_name),
        'access callback' => 'sirius_node_tab_access',
        'access arguments' => array(1, $tab_name),
        'type' => MENU_LOCAL_TASK,
      );
      if ($tab_record['weight']) {
        $items['node/%node/' . $tab_name]['weight'] = $tab_record['weight'];
      }
    }
  }

  $items['sirius/news'] = array(
    'title' => 'News',
    'description' => '',
    'page callback' => 'sirius_page_news',
    'page arguments' => array(),
    'access arguments' => array('sirius'),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/admin/banner'] = array(
    'title' => 'Banner Configuration ',
    'description' => '',
    'access arguments' => array('sirius admin'),
    'page callback' => 'drupal_get_form',
    'page arguments' => array('sirius_banner_config'),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/dispatch/threestrikes'] = array(
    'title' => 'Three Strikes Tester',
    'description' => '',
    'access arguments' => array('sirius dispatch staff'),
    'page callback' => 'sirius_dispatch_threestrikes_page',
    'type' => MENU_NORMAL_ITEM,
  );

  $items['d/%/%'] = array(
    'title' => 'Dispatch shortlink',
    'page callback' => 'sirius_aatlink',
    'page arguments' => array(1,2),
    'access callback' => TRUE,
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/config/specialtid'] = array(
    'title' => 'Special Terms',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('sirius_specialtid_admin'),
    'access arguments' => array('sirius admin'),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/config/specialrid'] = array(
    'title' => 'Special Roles',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('sirius_specialrid_admin'),
    'access arguments' => array('sirius admin'),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/config/email'] = array(
    'title' => 'Email Configuration',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('sirius_email_admin'),
    'access arguments' => array('sirius admin'),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/config/sms'] = array(
    'title' => 'SMS Configuration',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('sirius_sms_admin'),
    'access arguments' => array('sirius admin'),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/voice/forward-to-operator'] = array(
    'title' => 'Incoming Calls',
    'page callback' => 'sirius_page_voice_forward_to_operator',
    'page arguments' => array(),
    'access arguments' => array('sirius log view'),
    'type' => MENU_NORMAL_ITEM,
  );

  return $items;
}

function sirius_aatlink($nid, $aat) {
  drupal_goto('node/' . $nid . '/sirius_dispatch_worker_view', array('query' => array('aat' => $aat)));
}


function sirius_node_tab_content($node, $in_tab_name) {
  $node_tabs = sirius_node_tabs();
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

function sirius_node_tab_access($node, $in_tab_name) {
  // Check generic permissions
  $node_tabs = sirius_node_tabs();
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
