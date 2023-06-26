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

  $items['sirius/denorm/queue'] = array(
    'title' => 'Denormalize Queue',
    'description' => '',
    'page callback' => 'sirius_denorm_page_queue',
    'page arguments' => array(3),
    'access arguments' => array('sirius denorm admin'),
    'type' => MENU_NORMAL_ITEM,
    'file' => 'sirius_denorm.page_queue.inc',
  );

  return $items;
}
