<?php

/**
 * Implements hook_menu()
 */

function sirius_flood_menu() {
  $items = array();

  $items['sirius/flood'] = array(
    'title' => 'Flood Management',
    'description' => '',
    'page callback' => 'sirius_flood_page_unblock',
    'page arguments' => array(),
    'access arguments' => array('sirius admin'),
    'type' => MENU_NORMAL_ITEM,
    'file' => 'sirius_flood.page_unblock.inc',
  );

  $items['sirius/flood/unblock'] = array(
    'title' => 'Unblock',
    'description' => '',
    'page callback' => 'sirius_flood_page_unblock',
    'page arguments' => array(),
    'access arguments' => array('sirius admin'),
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'file' => 'sirius_flood.page_unblock.inc',
    'weight' => 1,
  );

  $items['sirius/flood/settings'] = array(
    'title' => 'Settings',
    'description' => '',
    'page callback' => 'sirius_flood_page_settings',
    'page arguments' => array(),
    'access arguments' => array('sirius admin'),
    'type' => MENU_LOCAL_TASK,
    'file' => 'sirius_flood.page_settings.inc',
    'weight' => 2,
  );

  $items['sirius/flood/test'] = array(
    'title' => 'Test',
    'description' => '',
    'page callback' => 'sirius_flood_page_test',
    'page arguments' => array(),
    'access arguments' => array('sirius admin'),
    'type' => MENU_LOCAL_TASK,
    'file' => 'sirius_flood.page_test.inc',
    'weight' => 5,
  );

  return $items;
}
