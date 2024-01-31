<?php

function sirius_service_menu() {
  $items = array();

  $items['sirius/service/config'] = array(
    'title' => 'Service',
    'page callback' => 'sirius_service_page_config',
    'page arguments' => array(),
    'access callback' => 'sirius_service_page_config_access',
    'access arguments' => array('none'),
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

  $items['sirius/service/config/client/cred'] = array(
    'title' => 'Credentials',
    'page callback' => 'sirius_service_page_config_client_cred',
    'page arguments' => array(),
    'access callback' => 'sirius_service_page_config_access',
    'access arguments' => array('client'),
    'type' => MENU_LOCAL_TASK,
    'file' => 'sirius_service.page_config_client_cred.inc',
    'weight' => 2,
  );

  $items['sirius/service/config/client/test'] = array(
    'title' => 'Test',
    'page callback' => 'sirius_service_page_config_client_test',
    'page arguments' => array(),
    'access callback' => 'sirius_service_page_config_access',
    'access arguments' => array('client'),
    'type' => MENU_LOCAL_TASK,
    'file' => 'sirius_service.page_config_client_test.inc',
    'weight' => 3,
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

  $items['sirius/service/config/server/settings'] = array(
    'title' => 'Settings',
    'page callback' => 'sirius_service_page_config_server',
    'page arguments' => array(),
    'access callback' => 'sirius_service_page_config_access',
    'access arguments' => array('server'),
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'file' => 'sirius_service.page_config_server.inc',
    'weight' => 1,
  );

  $items['sirius/service/config/server/funcs'] = array(
    'title' => 'Enabled Functions',
    'page callback' => 'sirius_service_page_config_server_funcs',
    'page arguments' => array(),
    'access callback' => 'sirius_service_page_config_access',
    'access arguments' => array('server'),
    'type' => MENU_LOCAL_TASK,
    'file' => 'sirius_service.page_config_server_funcs.inc',
    'weight' => 2,
  );

  $items['sirius/service/proxy/client'] = array(
    'title' => 'Proxy',
    'page callback' => 'sirius_service_page_proxy_client',
    'page arguments' => array(),
    'access callback' => 'sirius_menu_access_callback',
    'access arguments' => array(
      array(
        'permission' => 'sirius service proxy client user',
        'component' => 'service:proxy:client',
      ),
    ),
    'type' => MENU_NORMAL_ITEM,
    'file' => 'sirius_service.page_proxy_client.inc',
  );

  $items['user/%/sirius_proxy_server'] = array(
    'title' => 'Proxy Server',
    'page callback' => 'sirius_service_user_tab_proxy_server',
    'page arguments' => array(1),
    'access callback' => 'sirius_menu_access_callback',
    'access arguments' => array(
      array(
        'permission' => 'sirius service admin',
        'component' => 'service:proxy:server',
      ),
    ),
    'type' => MENU_LOCAL_TASK,
    'weight' => 8,
    'file' => 'sirius_service.page_user_proxy_server.inc',
  );

  $items['user/%/sirius_proxy_client'] = array(
    'title' => 'Proxy Client',
    'page callback' => 'sirius_service_user_tab_proxy_client',
    'page arguments' => array(1),
    'access callback' => 'sirius_menu_access_callback',
    'access arguments' => array(
      array(
        'permission' => 'sirius service admin',
        'component' => 'service:proxy:client',
      ),
    ),
    'type' => MENU_LOCAL_TASK,
    'weight' => 8,
    'file' => 'sirius_service.page_user_proxy_client.inc',
  );

  return $items;
}

/*
function sirius_service_messing_with_proxy() {
  $args = func_get_args();
  $path = join('/', func_get_args());

  global $GLOBALS;
  $GLOBALS['base_path'] = '/i/am/a/proxy/';

  $result = menu_execute_active_handler($path, FALSE);
  dpm($result);
  return 'Boo';
}
*/

function sirius_service_page_config_access($which) {
  // if (!sirius_domain_nid()) { return FALSE; }
  if (!user_access('sirius service admin')) { return FALSE; }
  if (!sirius_domain_component_enabled('service')) { return FALSE; }

  if ($which == 'server' && !sirius_domain_component_enabled('service:server')) { return FALSE; }
  if ($which == 'client' && !sirius_domain_component_enabled('service:client')) { return FALSE; }

  return TRUE;
}