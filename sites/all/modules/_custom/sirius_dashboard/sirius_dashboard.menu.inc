<?php

function sirius_dashboard_node_tabs() {
	$node_tabs = array();

  $node_tabs['sirius_worker']['sirius_worker_dashboard'] = array(
    'title' => 'Dashboard',
    'description' => '',
    'callback' => 'sirius_dashboard_node_tab_worker',
    'permissions' => array('sirius dashboard user'),
    'file' => 'sirius_dashboard.page_worker.inc',
    'weight' => -1,
    'component' => 'dashboard:worker',
  );

  sirius_node_tab_set_module('sirius_dashboard', $node_tabs);
	return $node_tabs;
}

function sirius_dashboard_term_tabs() {
  $term_tabs = array();

  /*
  $term_tabs['sirius_event_type']['sirius_event_type_policy'] = array(
    'title' => 'Event Access',
    'description' => '',
    'callback' => 'sirius_event_term_tab_policy',
    'permissions' => array('sirius event admin'),
    'weight' => 1,
  );
  */

  sirius_node_tab_set_module('sirius_dashboard', $term_tabs);
  return $term_tabs;
}

/***
 * Implements hook_menu
 */

function sirius_dashboard_menu() {
  $items = array();

  $items['sirius/dashboard'] = array(
    'title' => 'Dashboard',
    'description' => '',
    'page callback' => 'sirius_dashboard_page_dashboard',
    'page arguments' => array(),
    'access arguments' => array('sirius dashboard user'),
    'type' => MENU_NORMAL_ITEM,
    'file' => 'sirius_dashboard.page_dashboard.inc',
  );

  $items['sirius/dashboard/content'] = array(
    'title' => 'Dashboard Content',
    'description' => '',
    'page callback' => 'sirius_dashboard_page_dashboard_content',
    'page arguments' => array(),
    'access arguments' => array('sirius dashboard user'),
    'type' => MENU_CALLBACK,
    'file' => 'sirius_dashboard.page_dashboard.inc',
  );

  $items['sirius/dashboard/items_by_role'] = array(
    'title' => 'Dashboard Items by Role',
    'description' => '',
    'page callback' => 'sirius_dashboard_page_items_byrole',
    'page arguments' => array(),
    'access arguments' => array('sirius dashboard admin'),
    'type' => MENU_NORMAL_ITEM,
    'file' => 'sirius_dashboard.page_items_byrole.inc',
  );

  $items['sirius/dashboard/items_by_role/%'] = array(
    'title' => 'Dashboard Items by Role',
    'description' => '',
    'page callback' => 'sirius_dashboard_page_items',
    'page arguments' => array(3),
    'access arguments' => array('sirius dashboard admin'),
    'type' => MENU_NORMAL_ITEM,
    'file' => 'sirius_dashboard.page_items.inc',
  );

  $items['sirius/dashboard/items_by_role/%/%'] = array(
    'title' => 'Dashboard Item',
    'description' => '',
    'page callback' => 'sirius_dashboard_page_item',
    'page arguments' => array(3,4),
    'access arguments' => array('sirius dashboard admin'),
    'type' => MENU_NORMAL_ITEM,
    'file' => 'sirius_dashboard.page_item.inc',
  );

  $items['sirius/dashboard/items/user'] = array(
    'title' => 'Items',
    'description' => '',
    'page callback' => 'sirius_dashboard_page_items',
    'page arguments' => array('user'),
    'access arguments' => array('sirius dashboard admin'),
    'type' => MENU_NORMAL_ITEM,
    'file' => 'sirius_dashboard.page_items.inc',
  );


  return $items;
}

/**
 * Implments hook_modal_paths
 */

function sirius_dashboard_modal_paths() {
  $paths = array();

  /*
  $paths['sirius/modal/event/map/%/%'] = array(
    'style' => 'sirius-event-modal-style',
  );
  */

  return $paths;
}
