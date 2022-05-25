<?php

function sirius_service_menu() {
  $items = array();

  $items['sirius/service/config'] = array(
    'title' => 'Service',
    'page callback' => 'sirius_service_page_config',
    'page arguments' => array(),
    'access callback' => 'sirius_service_page_config_access',
    'access arguments' => array('client'),
    'file' => 'sirius_service.page_config.inc',
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/service/config/general'] = array(
    'title' => 'General',
    'page callback' => 'sirius_service_page_config',
    'page arguments' => array(),
    'access callback' => 'sirius_service_page_config_access',
    'access arguments' => array('general'),
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'file' => 'sirius_service.page_config.inc',
    'weight' => 1,
  );

  $items['sirius/service/config/client'] = array(
    'title' => 'Client',
    'page callback' => 'sirius_service_page_config_client',
    'page arguments' => array(),
    'access callback' => 'sirius_service_page_config_access',
    'access arguments' => array('client'),
    'type' => MENU_LOCAL_TASK,
    'file' => 'sirius_service.page_config_client.inc',
    'weight' => 2,
  );

  $items['sirius/service/config/client/general'] = array(
    'title' => 'Settings',
    'page callback' => 'sirius_service_page_config_client',
    'page arguments' => array(),
    'access callback' => 'sirius_service_page_config_access',
    'access arguments' => array('client'),
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'file' => 'sirius_service.page_config_client.inc',
    'weight' => 1,
  );

  $items['sirius/service/config/client/test'] = array(
    'title' => 'Test',
    'page callback' => 'sirius_service_page_config_client_test',
    'page arguments' => array(),
    'access callback' => 'sirius_service_page_config_access',
    'access arguments' => array('client'),
    'type' => MENU_LOCAL_TASK,
    'file' => 'sirius_service.page_config_client_test.inc',
    'weight' => 2,
  );

  $items['sirius/service/config/server'] = array(
    'title' => 'Server',
    'page callback' => 'sirius_service_page_config_server',
    'page arguments' => array(),
    'access callback' => 'sirius_service_page_config_access',
    'access arguments' => array('server'),
    'type' => MENU_LOCAL_TASK,
    'file' => 'sirius_service.page_config_server.inc',
    'weight' => 3,
  );

  return $items;
}

function sirius_service_page_config_access($which) {
  // if (!sirius_domain_nid()) { return FALSE; }
  if (!user_access('sirius service admin')) { return FALSE; }
  if (!sirius_domain_component_enabled('service')) { return FALSE; }

  if ($which == 'server' && !sirius_domain_component_enabled('service:server')) { return FALSE; }
  if ($which == 'client' && !sirius_domain_component_enabled('service:client')) { return FALSE; }

  return TRUE;
}