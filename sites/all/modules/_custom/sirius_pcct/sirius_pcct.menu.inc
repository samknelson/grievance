<?php


/**
 * Implements hook_sirius_node_tabs()
 */
 
function sirius_pcct_node_tabs() {
  $node_tabs = array();

  $node_tabs['sirius_json_definition']['sirius_pcct_settings'] = array(
    'title' => 'PCCT',
    'description' => '',
    'callback' => 'sirius_pcct_node_tab_policy_settings',
    'permissions' => array('sirius pcct admin'),
    'file' => 'sirius_pcct.page_policy_settings.inc',
    'component' => 'pcct',
  );

  $node_tabs['sirius_trust_service']['sirius_trust_service_rates_by_level'] = array(
    'title' => 'Rates by Level',
    'description' => '',
    'callback' => 'sirius_pcct_node_tab_service_rates_by_level',
    'permissions' => array('sirius pcct admin'),
    'file' => 'sirius_pcct.page_service_rates_by_level.inc',
    'component' => 'pcct',
  );

  $node_tabs['sirius_trust_provider']['sirius_trust_provider_rates_by_service'] = array(
    'title' => 'Rates by Service',
    'description' => '',
    'callback' => 'sirius_pcct_node_tab_provider_rates_by_service',
    'permissions' => array('sirius pcct admin'),
    'file' => 'sirius_pcct.page_provider_rates_by_service.inc',
    'component' => 'pcct',
  );


  $node_tabs['sirius_trust_service']['sirius_pcct_service_public_list'] = array(
    'title' => 'Provider List',
    'description' => '',
    'callback' => 'sirius_pcct_node_tab_service_public_list',
    'permissions' => array(),
    'file' => 'sirius_pcct.page_service_public_list.inc',
    'component' => 'pcct',
    'skip_node_view_check' => TRUE,
    'weight' => 10,
  );

  $node_tabs['sirius_trust_service']['sirius_pcct_service_public_map'] = array(
    'title' => 'Provider Map',
    'description' => '',
    'callback' => 'sirius_pcct_node_tab_service_public_map',
    'permissions' => array(),
    'file' => 'sirius_pcct.page_service_public_map.inc',
    'component' => 'pcct',
    'skip_node_view_check' => TRUE,
    'weight' => 11,
  );

  $node_tabs['sirius_trust_provider']['sirius_pcct_provider_public'] = array(
    'title' => 'PCCT Public View',
    'description' => '',
    'callback' => 'sirius_pcct_node_tab_provider_public',
    'permissions' => array(),
    'file' => 'sirius_pcct.page_provider_public.inc',
    'component' => 'pcct',
    'skip_node_view_check' => TRUE,
  );

  $node_tabs['sirius_worker']['sirius_worker_pcct'] = array(
    'title' => 'PCCT',
    'description' => '',
    'callback' => 'sirius_pcct_node_tab_worker',
    'permissions' => array(),
    'file' => 'sirius_pcct.page_worker.inc',
    'component' => 'pcct',
  );

  sirius_node_tab_set_module('sirius_pcct', $node_tabs);

  return $node_tabs;
}

/**
 * Implements hook_menu()
 */

function sirius_pcct_menu() {
  $items = array();

  $items['sirius/pcct/config/global'] = array(
    'title' => 'Global Settings',
    'description' => '',
    'page callback' => 'sirius_pcct_page_settings_global',
    'page arguments' => array(),
    'access arguments' => array('sirius admin'),
    'type' => MENU_NORMAL_ITEM,
    'file' => 'sirius_pcct.page_settings_global.inc',
    'weight' => 1,
  );

  $items['sirius/pcct/config'] = array(
    'title' => 'PCCT Settings',
    'description' => '',
    'page callback' => 'sirius_pcct_page_settings',
    'page arguments' => array(),
    'access arguments' => array('sirius pcct admin'),
    'type' => MENU_NORMAL_ITEM,
    'file' => 'sirius_pcct.page_settings.inc',
  );

  $items['sirius/pcct/config/general'] = array(
    'title' => 'PCCT Settings',
    'description' => '',
    'page callback' => 'sirius_pcct_page_settings',
    'page arguments' => array(),
    'access arguments' => array('sirius pcct admin'),
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'file' => 'sirius_pcct.page_settings.inc',
    'weight' => 1,
  );


  $items['sirius/pcct/config/landing'] = array(
    'title' => 'Landing Page',
    'description' => '',
    'page callback' => 'sirius_pcct_page_settings_landing',
    'page arguments' => array(),
    'access arguments' => array('sirius pcct'),
    'type' => MENU_LOCAL_TASK,
    'file' => 'sirius_pcct.page_settings_landing.inc',
    'weight' => 2,
  );

  /*
  $items['sirius/pcct/domain-list'] = array(
    'title' => 'Choose Your Plan',
    'description' => '',
    'page callback' => 'sirius_pcct_page_domain_list',
    'page arguments' => array(),
    'access callback' => TRUE,
    'access arguments' => array(),
    'type' => MENU_NORMAL_ITEM,
    'file' => 'sirius_pcct.page_domain_list.inc',
    'weight' => 1,
  );
  */

  $items['sirius/pcct'] = array(
    'title' => 'My Health Calculator',
    'description' => '',
    'page callback' => 'sirius_pcct_page_home',
    'page arguments' => array(),
    'access callback' => TRUE,
    'access arguments' => array(),
    'type' => MENU_NORMAL_ITEM,
    'file' => 'sirius_pcct.page_home.inc',
    'weight' => 1,
  );

  $items['sirius/pcct/landing'] = array(
    'title' => 'My Health Calculator',
    'description' => '',
    'page callback' => 'sirius_pcct_page_landing',
    'page arguments' => array(),
    'access callback' => TRUE,
    'access arguments' => array(),
    'type' => MENU_NORMAL_ITEM,
    'file' => 'sirius_pcct.page_landing.inc',
    'weight' => 1,
  );

  $items['pcct/%'] = array(
    'title' => 'My Health Calculator',
    'description' => '',
    'page callback' => 'sirius_pcct_page_launch',
    'page arguments' => array(1),
    'access callback' => TRUE,
    'access arguments' => array(),
    'type' => MENU_NORMAL_ITEM,
    'file' => 'sirius_pcct.page_launch.inc',
    'weight' => 1,
  );

  return $items;
}
