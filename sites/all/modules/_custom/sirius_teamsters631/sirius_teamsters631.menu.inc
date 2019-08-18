<?php

function sirius_teamsters631_node_tabs() {
  $node_tabs = array();

  $node_tabs['sirius_worker']['sirius_teamsters631_badge'] = array(
    'title' => 'Badge',
    'description' => '',
    'callback' => 'sirius_teamsters631_badge',
    'permissions' => array('sirius dispatch staff'),
  );


  return $node_tabs;
}

/**
 * Implements hook_menu
 */

function sirius_teamsters631_menu() {
  $items = array();

  //
  // Node Tabs
  // 

  $node_tabs = sirius_teamsters631_node_tabs();
  foreach ($node_tabs as $node_type => $node_record) {
    foreach ($node_record as $tab_name => $tab_record) {
      $items['node/%node/' . $tab_name] = array(
        'title' => $tab_record['title'],
        'description' => $tab_record['description'],
        'page callback' => 'sirius_teamsters631_node_tab_content',
        'page arguments' => array(1, $tab_name),
        'access callback' => 'sirius_teamsters631_node_tab_access',
        'access arguments' => array(1, $tab_name),
        'type' => MENU_LOCAL_TASK,
      );
      if ($tab_record['weight']) {
        $items['node/%node/' . $tab_name]['weight'] = $tab_record['weight'];
      }
    }
  }

  $items['sirius/admin/teamsters631/ebnotify'] = array(
    'title' => 'Teamsters 631 Extraboard Notifications',
    'description' => '',
    'access arguments' => array('sirius teamsters631 admin'),
    'page callback' => 'drupal_get_form',
    'page arguments' => array('sirius_teamsters631_ebnotify_admin'),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/teamsters631/badge/%node/front'] = array(
    'title' => 'Teamsters 631 Badge',
    'description' => '',
    'access callback' => TRUE,
    'page callback' => 'sirius_teamsters631_badge_front',
    'page arguments' => array(3),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/teamsters631/badge/%node/contents'] = array(
    'title' => 'Teamsters 631 Badge',
    'description' => '',
    'access callback' => 'sirius_teamsters631_badge_contents_access',
    'page callback' => 'sirius_teamsters631_badge_contents',
    'page arguments' => array(3),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/teamsters631/badge/%node/headshot'] = array(
    'title' => 'Teamsters 631 Badge',
    'description' => '',
    'access callback' => 'sirius_teamsters631_badge_contents_access',
    'page callback' => 'sirius_teamsters631_badge_headshot',
    'page arguments' => array(3),
    'type' => MENU_NORMAL_ITEM,
  );

  return $items;
}


function sirius_teamsters631_node_tab_content($node, $in_tab_name) {
  $node_tabs = sirius_teamsters631_node_tabs();
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

function sirius_teamsters631_node_tab_access($node, $in_tab_name) {
  // Check generic permissions
  $node_tabs = sirius_teamsters631_node_tabs();
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
