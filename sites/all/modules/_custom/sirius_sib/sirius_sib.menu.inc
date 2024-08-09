<?php

/**
 * Implements hook_sirius_node_tabs()
 */
 
function sirius_sib_node_tabs() {
  $node_tabs = array();

  $node_tabs['sirius_worker']['sirius_worker_sib'] = array(
    'title' => 'Restrictions',
    'description' => '',
    'file' => 'sirius_sib.page_entity_sib.inc',
    'callback' => 'sirius_sib_node_tab_entity_sib',
    'callback arguments' => array('sib'),
    'component' => 'sib',
    'permissions' => array('sirius sib assign'),
    'weight' => 12,
  );

  $node_tabs['sirius_worker']['sirius_worker_sib/list'] = array(
    'title' => 'List',
    'description' => '',
    'file' => 'sirius_sib.page_entity_sib.inc',
    'callback' => 'sirius_sib_node_tab_entity_sib',
    'callback arguments' => array('sib'),
    'component' => 'sib',
    'permissions' => array('sirius sib assign'),
    'weight' => 1,
    'type' => MENU_DEFAULT_LOCAL_TASK,
  );

  $node_tabs['sirius_worker']['sirius_worker_sib/assign'] = array(
    'title' => 'Assign',
    'description' => '',
    'file' => 'sirius_sib.page_entity_sib_assign.inc',
    'callback' => 'sirius_sib_node_tab_entity_sib_assign',
    'callback arguments' => array('sib'),
    'component' => 'sib',
    'permissions' => array('sirius sib assign'),
    'weight' => 2,
  );

  $node_tabs['sirius_worker']['sirius_worker_sib/cancel'] = array(
    'title' => 'Cancel',
    'description' => '',
    'file' => 'sirius_sib.page_entity_sib_cancel.inc',
    'callback' => 'sirius_sib_node_tab_entity_sib_cancel',
    'callback arguments' => array('sib'),
    'component' => 'sib',
    'permissions' => array('sirius sib assign'),
    'weight' => 2,
  );

  sirius_node_tab_set_module('sirius_sib', $node_tabs);

  return $node_tabs;
}


/**
 * Implements hook_sirius_term_tabs()
 */
 
function sirius_sib_term_tabs() {
  $term_tabs = array();

  $term_tabs['sirius_dispatch_sib']['sirius_sib_settings'] = array(
    'title' => 'Access Restrictions',
    'description' => '',
    'file' => 'sirius_sib.term_tab_settings.inc',
    'callback' => 'sirius_sib_term_tab_settings',
    'component' => 'sib',
    'permissions' => array('sirius sib admin'),
    'weight' => 6,
  );

  sirius_node_tab_set_module('sirius_sib', $term_tabs);

  return $term_tabs;
}

/**
 * Implements hook_menu()
 */

function sirius_sib_menu() {
  $items = array();

  /*
  $items['sirius/sib'] = array(
    'title' => 'Settings',
    'description' => '',
    'page callback' => 'sirius_cardcheck_page_settings',
    'page arguments' => array(),
    'access arguments' => array('sirius cardcheck admin'),
    'type' => MENU_NORMAL_ITEM,
    'file' => 'sirius_cardcheck.page_settings.inc',
  );
  */

  return $items;
}
