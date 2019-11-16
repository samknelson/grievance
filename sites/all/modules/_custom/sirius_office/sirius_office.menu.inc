<?php

/**
 * Tabs: Which node type has which tabs. 
 *
 * Much of the functionality is via local tasks in nodes.  For instance, nodes of type "worker" have a local tab called "housing" which 
 * shows housing-related functions. 
 */
 
function sirius_office_node_tabs() {
  $node_tabs = array();

  /*
  $node_tabs['sirius_json_definition']['sirius_json_definition_edit'] = array(
    'title' => 'Definition',
    'description' => '',
    'callback' => 'sirius_json_definition_edit',
    'permissions' => array('sirius admin'),
    'weight' => 1,
  );
  */

  return $node_tabs;
}

function sirius_office_menu() {
  $items = array();

  $node_tabs = sirius_office_node_tabs();
  foreach ($node_tabs as $node_type => $node_record) {
    foreach ($node_record as $tab_name => $tab_record) {
      $items['node/%node/' . $tab_name] = array(
        'title' => $tab_record['title'],
        'description' => $tab_record['description'],
        'page callback' => 'sirius_office_node_tab_content',
        'page arguments' => array(1, $tab_name),
        'access callback' => 'sirius_office_node_tab_access',
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

  $items['sirius/office/iprestrict/admin'] = array(
    'title' => 'IP Restrictions',
    'description' => '',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('sirius_office_iprestrict_admin'),
    'access arguments' => array('sirius office iprestrict admin'),
    'type' => MENU_NORMAL_ITEM,
  );

  return $items;
}

function sirius_office_node_tab_content($node, $in_tab_name) {
  $node_tabs = sirius_office_node_tabs();
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

function sirius_office_node_tab_access($node, $in_tab_name) {
  // Check generic permissions
  $node_tabs = sirius_office_node_tabs();
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
