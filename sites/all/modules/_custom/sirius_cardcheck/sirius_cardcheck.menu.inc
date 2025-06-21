<?php


/**
 * Implements hook_sirius_node_tabs()
 */
 
function sirius_cardcheck_node_tabs() {
  $node_tabs = array();

  $node_tabs['sirius_json_definition']['sirius_json_definition_cardcheck_definition'] = array(
    'title' => 'Cardcheck Definition',
    'description' => '',
    'callback' => 'sirius_node_tab_json_definition_cardcheck',
    'permissions' => array('sirius cardcheck admin'),
    'weight' => 1,
    'file' => 'sirius_cardcheck.page_definition_edit.inc',
    'component' => 'cardcheck',
  );

  $node_tabs['sirius_json_definition']['sirius_json_definition_cardcheck_definition/edit'] = array(
    'title' => 'Edit',
    'description' => '',
    'callback' => 'sirius_node_tab_json_definition_cardcheck',
    'permissions' => array('sirius cardcheck admin'),
    'weight' => 1,
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'file' => 'sirius_cardcheck.page_definition_edit.inc',
    'component' => 'cardcheck',
  );

  $node_tabs['sirius_json_definition']['sirius_json_definition_cardcheck_definition/preview'] = array(
    'title' => 'Preview',
    'description' => '',
    'callback' => 'sirius_node_tab_json_definition_cardcheck_preview',
    'permissions' => array('sirius cardcheck admin'),
    'weight' => 2,
    'file' => 'sirius_cardcheck.page_definition_preview.inc',
    'component' => 'cardcheck',
  );

  $node_tabs['sirius_worker']['sirius_worker_cardchecks'] = array(
    'title' => 'Cardchecks',
    'description' => '',
    'callback' => 'sirius_cardcheck_node_tab_worker',
    'permissions' => array('sirius'),
    'file' => 'sirius_cardcheck.page_worker.inc',
    'component' => 'cardcheck',
  );

  $node_tabs['sirius_log']['sirius_log_cardcheck'] = array(
    'title' => 'Cardcheck',
    'description' => '',
    'callback' => 'sirius_cardcheck_node_tab_cardcheck',
    'permissions' => array('sirius'),
    'file' => 'sirius_cardcheck.page_cardcheck.inc',
    'component' => 'cardcheck',
    'skip_node_view_check' => TRUE,
  );

  sirius_node_tab_set_module('sirius_cardcheck', $node_tabs);

  return $node_tabs;
}

/**
 * Implements hook_menu()
 */

function sirius_cardcheck_menu() {
  $items = array();

  $items['sirius/cardcheck'] = array(
    'title' => 'Card Check Settings',
    'description' => '',
    'page callback' => 'sirius_cardcheck_page_settings',
    'page arguments' => array(),
    'access arguments' => array('sirius cardcheck admin'),
    'type' => MENU_NORMAL_ITEM,
    'file' => 'sirius_cardcheck.page_settings.inc',
  );

  $items['sirius/cardcheck/settings'] = array(
    'title' => 'Settings',
    'description' => '',
    'page callback' => 'sirius_cardcheck_page_settings',
    'page arguments' => array(),
    'access arguments' => array('sirius cardcheck admin'),
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'file' => 'sirius_cardcheck.page_settings.inc',
    'weight' => 1,
  );

  $items['sirius/cardcheck/definitions'] = array(
    'title' => 'List',
    'description' => '',
    'page callback' => 'sirius_cardcheck_page_definition_list',
    'page arguments' => array(),
    'access arguments' => array('sirius cardcheck admin'),
    'type' => MENU_LOCAL_TASK,
    'file' => 'sirius_cardcheck.page_definition_list.inc',
    'weight' => 2,
  );

  $items['sirius/cardcheck/definitions/add'] = array(
    'title' => 'Add a Card Check',
    'description' => '',
    'page callback' => 'sirius_cardcheck_page_definition_add',
    'page arguments' => array(),
    'access arguments' => array('sirius cardchecks admin'),
    'file' => 'sirius_cardcheck.page_definition_add.inc',
    'type' => MENU_NORMAL_ITEM,
  );

  return $items;
}
