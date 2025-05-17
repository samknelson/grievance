<?php

/**
 * Implements hook_node_tabs
 */

function sirius_worker_tos_node_tabs() {
  $node_tabs = array();

  $node_tabs['sirius_worker']['sirius_worker_schedule/tos'] = array(
    'title' => 'Call In / Out',
    'description' => '',
    'callback' => 'sirius_worker_tos_node_tab_worker',
    'permissions' => array(),
    'file' => 'sirius_worker_tos.page_worker.inc',
    'skip_node_view_check' => TRUE,
    'component' => 'worker:tos',
    'weight' => 4,
  );

  $node_tabs['sirius_worker']['sirius_worker_schedule/tos-archive'] = array(
    'title' => 'Call In / Out History',
    'description' => '',
    'callback' => 'sirius_worker_tos_node_tab_worker_archive',
    'permissions' => array(),
    'file' => 'sirius_worker_tos.page_worker_archive.inc',
    'skip_node_view_check' => TRUE,
    'component' => 'worker:tos',
    'weight' => 5,
  );

  $node_tabs['sirius_log']['sirius_worker_tos'] = array(
    'title' => 'Call In / Out',
    'description' => '',
    'callback' => 'sirius_worker_tos_node_tab_edit',
    'permissions' => array(),
    'file' => 'sirius_worker_tos.page_edit.inc',
    'skip_node_view_check' => TRUE,
    'component' => 'worker:tos',
    'weight' => 4,
  );

  $node_tabs['grievance_shop']['sirius_employer_dispatch/tos'] = array(
    'title' => 'Call In / Out',
    'description' => '',
    'callback' => 'sirius_worker_tos_page_employer_list',
    'permissions' => array(),
    'file' => 'sirius_worker_tos.page_employer_list.inc',
    'skip_node_view_check' => TRUE,
    'component' => 'worker:tos',
    'weight' => 4,
  );

  sirius_node_tab_set_module('sirius_worker_tos', $node_tabs);

  return $node_tabs;
}

function sirius_worker_tos_menu() {
  $items = array();

  $items['sirius/worker/tos'] = array(
    'title' => 'Call In / Out',
    'page callback' => 'sirius_worker_tos_page_list',
    'page arguments' => array(),
    'access callback' => 'sirius_menu_access_callback',
    'access arguments' => array(
      array(
        'component' => 'worker:tos',
        'access' => array(
          'type' => 'worker_tos',
          'action' => 'list',
          'context' => array(),
        ),
      ),
    ),
    'type' => MENU_NORMAL_ITEM,
    'file' => 'sirius_worker_tos.page_list.inc',
  );

  $items['sirius/worker/tos/list'] = array(
    'title' => 'List',
    'page callback' => 'sirius_worker_tos_page_list',
    'page arguments' => array(),
    'access callback' => 'sirius_menu_access_callback',
    'access arguments' => array(
      array(
        'component' => 'worker:tos',
        'access' => array(
          'type' => 'worker_tos',
          'action' => 'list',
          'context' => array(),
        ),
      ),
    ),
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'file' => 'sirius_worker_tos.page_list.inc',
    'weight' => 1,
  );

  $items['sirius/worker/tos/conf'] = array(
    'title' => 'Config',
    'page callback' => 'sirius_worker_tos_page_config',
    'page arguments' => array(),
    'access callback' => 'sirius_menu_access_callback',
    'access arguments' => array(
      array(
        'component' => 'worker:tos',
        'access' => array(
          'type' => 'worker_tos',
          'action' => 'config',
          'context' => array(),
        ),
      ),
    ),
    'type' => MENU_LOCAL_TASK,
    'file' => 'sirius_worker_tos.page_config.inc',
    'weight' => 2,
  );

  return $items;
}
