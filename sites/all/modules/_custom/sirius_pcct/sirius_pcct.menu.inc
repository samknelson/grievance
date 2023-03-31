<?php


/**
 * Implements hook_sirius_node_tabs()
 */
 
function sirius_pcct_node_tabs() {
  $node_tabs = array();

  /*
  $node_tabs['sirius_worker']['sirius_worker_pcct'] = array(
    'title' => 'PCCT',
    'description' => '',
    'callback' => 'sirius_pcct_node_tab_worker_checkin_list',
    'permissions' => array('sirius'),
    'file' => 'sirius_pcct.page_worker_checkin_list.inc',
    'component' => 'pcct',
    'skip_node_view_check' => TRUE,
  );
  */

  sirius_node_tab_set_module('sirius_pcct', $node_tabs);

  return $node_tabs;
}

/**
 * Implements hook_menu()
 */

function sirius_pcct_menu() {
  $items = array();

  $items['sirius/pcct/config'] = array(
    'title' => 'PCCT Settings',
    'description' => '',
    'page callback' => 'sirius_pcct_page_settings',
    'page arguments' => array(),
    'access arguments' => array('sirius pcct admin'),
    'type' => MENU_NORMAL_ITEM,
    'file' => 'sirius_pcct.page_settings.inc',
  );

  $items['sirius/pcct/config/global'] = array(
    'title' => 'Global Settings',
    'description' => '',
    'page callback' => 'sirius_pcct_page_settings',
    'page arguments' => array(),
    'access arguments' => array('sirius admin'),
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
    'weight' => 1,
  );




  $items['sirius/pcct/domain-list'] = array(
    'title' => 'My Health Calculator',
    'description' => '',
    'page callback' => 'sirius_pcct_page_domain_list',
    'page arguments' => array(),
    'access callback' => TRUE,
    'access arguments' => array(),
    'type' => MENU_NORMAL_ITEM,
    'file' => 'sirius_pcct.page_domain_list.inc',
    'weight' => 1,
  );

  

  return $items;
}
