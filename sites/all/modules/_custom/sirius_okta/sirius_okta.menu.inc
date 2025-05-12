<?php


/**
 * Implements hook_sirius_node_tabs()
 */
 
function sirius_okta_node_tabs() {
  $node_tabs = array();

  /*
  $node_tabs['sirius_json_definition']['sirius_json_definition_cardcheck_definition'] = array(
    'title' => 'Cardcheck Definition',
    'description' => '',
    'callback' => 'sirius_node_tab_json_definition_cardcheck',
    'permissions' => array('sirius cardcheck admin'),
    'weight' => 1,
    'file' => 'sirius_cardcheck.page_definition_edit.inc',
    'component' => 'cardcheck',
  );
  */

  sirius_node_tab_set_module('sirius_okta', $node_tabs);

  return $node_tabs;
}

/**
 * Implements hook_menu()
 */

function sirius_okta_menu() {
  $items = array();

  $items['sirius/okta'] = array(
    'title' => 'Okta',
    'description' => '',
    'page callback' => 'sirius_okta_page_settings',
    'page arguments' => array(),
    'access callback' => 'sirius_menu_access_callback',
    'access arguments' => array(
      array(
        'permission' => 'sirius okta admin',
      ),
    ),
    'type' => MENU_NORMAL_ITEM,
    'file' => 'sirius_okta.page_settings.inc',
  );

  $items['sirius/okta/settings'] = array(
    'title' => 'Settings',
    'description' => '',
    'page callback' => 'sirius_okta_page_settings',
    'page arguments' => array(),
    'access callback' => 'sirius_menu_access_callback',
    'access arguments' => array(
      array(
        'permission' => 'sirius okta admin',
      ),
    ),
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'file' => 'sirius_okta.page_settings.inc',
    'weight' => 1,
  );

  $items['sirius/okta/test'] = array(
    'title' => 'Test',
    'description' => '',
    'page callback' => 'sirius_okta_page_test',
    'page arguments' => array(),
    'access callback' => 'sirius_menu_access_callback',
    'access arguments' => array(
      array(
        'permission' => 'sirius okta admin',
      ),
    ),
    'type' => MENU_LOCAL_TASK,
    'file' => 'sirius_okta.page_test.inc',
    'weight' => 1,
  );

  $items['user/%/sirius_okta'] = array(
    'title' => 'Okta',
    'page callback' => 'sirius_okta_page_user',
    'page arguments' => array(1),
    'access callback' => 'sirius_menu_access_callback',
    'access arguments' => array(
      array(
        'permission' => 'sirius okta admin',
        'access' => array(
          'type' => 'okta',
          'action' => 'user_sync',
          'context' => array(
          ),
        )
      ),
    ),
    'type' => MENU_LOCAL_TASK,
    'file' => 'sirius_okta.page_user.inc',
    'weight' => 2,
  );

  return $items;
}
