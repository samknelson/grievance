<?php

function sirius_fastload_node_tabs() {
	$node_tabs = array();

  /*
  $node_tabs['sirius_log']['sirius_event_intake_application_manage'] = array(
    'title' => 'Manage',
    'description' => '',
    'callback' => 'sirius_node_tab_event_intake_application_manage',
    'permissions' => array('sirius'),
    'file' => 'sirius_event.page_intake_application_manage.inc',
    'weight' => 4,
  );
  */

  sirius_node_tab_set_module('sirius_fastload', $node_tabs);
	return $node_tabs;
}

function sirius_fastload_term_tabs() {
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

  sirius_node_tab_set_module('sirius_fastload', $term_tabs);
  return $term_tabs;
}

/***
 * Implements hook_menu
 */

function sirius_fastload_menu() {
  $items = array();

  $items['sirius/fastload'] = array(
    'title' => 'Fastload',
    'description' => '',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('sirius_fastload_page_config'),
    'access arguments' => array('sirius fastload admin'),
    'type' => MENU_NORMAL_ITEM,
    'file' => 'sirius_fastload.page_config.inc',
  );

  $items['sirius/fastload/config'] = array(
    'title' => 'Config',
    'description' => '',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('sirius_fastload_page_config'),
    'access arguments' => array('sirius fastload admin'),
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'file' => 'sirius_fastload.page_config.inc',
    'weight' => 1,
  );

  $items['sirius/fastload/status'] = array(
    'title' => 'Status',
    'description' => '',
    'page callback' => 'sirius_fastload_page_status',
    'page arguments' => array(),
    'access arguments' => array('sirius fastload admin'),
    'type' => MENU_LOCAL_TASK,
    'file' => 'sirius_fastload.page_status.inc',
    'weight' => 2,
  );

  $items['sirius/fastload/generate'] = array(
    'title' => 'Generate',
    'description' => '',
    'page callback' => 'sirius_fastload_page_generate',
    'page arguments' => array(),
    'access arguments' => array('sirius fastload admin'),
    'type' => MENU_LOCAL_TASK,
    'file' => 'sirius_fastload.page_generate.inc',
    'weight' => 3,
  );

  return $items;
}

/**
 * Implments hook_modal_paths
 */

function sirius_fastload_modal_paths() {
  $paths = array();

  /*
  $paths['sirius/modal/event/map/%/%'] = array(
    'style' => 'sirius-event-modal-style',
  );
  */

  return $paths;
}
