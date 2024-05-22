<?php

function sirius_postal_menu() {
  $items = array();

  $items['sirius/postal/config'] = array(
    'title' => 'Postal',
    'page callback' => 'sirius_postal_page_config',
    'page arguments' => array(),
    'access callback' => 'sirius_postal_page_config_access',
    'access arguments' => array(),
    'file' => 'sirius_postal.page_config.inc',
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/postal/config/general'] = array(
    'title' => 'General',
    'page callback' => 'sirius_postal_page_config',
    'page arguments' => array(),
    'access callback' => 'sirius_postal_page_config_access',
    'access arguments' => array(),
    'file' => 'sirius_postal.page_config.inc',
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => 1,
  );

  $items['sirius/postal/config/cred'] = array(
    'title' => 'Credentials',
    'page callback' => 'sirius_postal_page_config_cred',
    'page arguments' => array(),
    'access callback' => 'sirius_postal_page_config_access',
    'access arguments' => array(),
    'type' => MENU_LOCAL_TASK,
    'file' => 'sirius_postal.page_config_cred.inc',
    'weight' => 2,
  );

  $items['sirius/postal/config/test'] = array(
    'title' => 'Test',
    'page callback' => 'sirius_postal_page_config_test',
    'page arguments' => array(),
    'access callback' => 'sirius_postal_page_config_access',
    'access arguments' => array(),
    'type' => MENU_LOCAL_TASK,
    'file' => 'sirius_postal.page_config_test.inc',
    'weight' => 3,
  );

  return $items;
}

function sirius_postal_page_config_access() {
  // if (!sirius_domain_nid()) { return FALSE; }
  if (!user_access('sirius postal admin')) { return FALSE; }
  if (!sirius_domain_component_enabled('postal')) { return FALSE; }

  return TRUE;
}