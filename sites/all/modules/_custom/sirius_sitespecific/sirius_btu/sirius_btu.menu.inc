<?php

/**
 * Implements hook_node_tabs
 */

function sirius_btu_node_tabs() {
  $node_tabs = array();

  $node_tabs['sirius_log']['sirius_btu_csg_view'] = array(
    'title' => 'CSG View',
    'description' => '',
    'callback' => 'sirius_btu_node_tab_csg_view',
    'permissions' => array('sirius'),
    'file' => 'sirius_btu.page_csg_view.inc',
    'component' => 'btu:csg',
    'skip_node_view_check' => TRUE,
    'weight' => 1,
  );

  $node_tabs['sirius_log']['sirius_btu_csg_edit'] = array(
    'title' => 'CSG Edit',
    'description' => '',
    'callback' => 'sirius_btu_node_tab_csg_edit',
    'permissions' => array('sirius trust staff'),
    'file' => 'sirius_btu.page_csg_edit.inc',
    'component' => 'btu:csg',
    'skip_node_view_check' => TRUE,
    'weight' => 2,
  );

  $node_tabs['sirius_worker']['sirius_worker_grievances/list'] = array(
    'title' => 'Standard Grievances',
    'callback' => 'sirius_node_tab_worker_grievances',
    'permissions' => array('grievance view'),
    'file' => 'sirius_worker.page_grievances.inc',
    'module' => 'sirius_worker',
    'weight' => 1,
    'type' => MENU_DEFAULT_LOCAL_TASK,
  );


  $node_tabs['sirius_worker']['sirius_worker_grievances/csg'] = array(
    'title' => 'Class Size Grievances',
    'callback' => 'sirius_btu_node_tab_worker_csg',
    'permissions' => array('sirius trust staff'),
    'file' => 'sirius_btu.page_worker_csg.inc',
    'component' => 'btu:csg',
    'weight' => 2,
  );

  sirius_node_tab_set_module('sirius_btu', $node_tabs);

  return $node_tabs;
}

/**
 * Implements hook_menu
 */

function sirius_btu_menu() {
  $items = array();

  $items['sirius/btu/csg'] = array(
    'title' => 'Class Size Grievances',
    'page callback' => 'sirius_btu_page_csg',
    'page arguments' => array(),
    'access callback' => 'sirius_menu_access_callback',
    'access arguments' => array(
      array(
        'permission' => 'sirius',
        'component' => 'btu:csg',
        'access' => array(
          'type' => 'btu:csg',
          'action' => 'list',
          'context' => array(),
        ),
      ),
    ),
    'type' => MENU_NORMAL_ITEM,
    'file' => 'sirius_btu.page_csg.inc',
  );

  $items['sirius/btu/csg/home'] = array(
    'title' => 'List',
    'page callback' => 'sirius_btu_page_csg',
    'page arguments' => array(),
    'access callback' => 'sirius_menu_access_callback',
    'access arguments' => array(
      array(
        'permission' => 'sirius trust staff',
        'component' => 'btu:csg',
      ),
    ),
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'file' => 'sirius_btu.page_csg.inc',
    'weight' => 1,
  );

  $items['sirius/btu/csg/add'] = array(
    'title' => 'Add',
    'page callback' => 'sirius_btu_page_csg_add',
    'page arguments' => array(),
    'access callback' => 'sirius_menu_access_callback',
    'access arguments' => array(
      array(
        'permission' => 'sirius trust staff',
        'component' => 'btu:csg',
      ),
    ),
    'type' => MENU_LOCAL_TASK,
    'file' => 'sirius_btu.page_csg_add.inc',
    'weight' => 2,
  );

  $items['sirius/btu/csg/settings'] = array(
    'title' => 'Settings',
    'page callback' => 'sirius_btu_page_csg_settings',
    'page arguments' => array(),
    'access callback' => 'sirius_menu_access_callback',
    'access arguments' => array(
      array(
        'permission' => 'sirius admin',
        'component' => 'btu:csg',
      ),
    ),
    'type' => MENU_LOCAL_TASK,
    'file' => 'sirius_btu.page_csg_settings.inc',
    'weight' => 2,
  );

  $items['sirius/btu/csg/global'] = array(
    'title' => 'Public Form Settings',
    'page callback' => 'sirius_btu_page_csg_global',
    'page arguments' => array(),
    'access callback' => 'sirius_menu_access_callback',
    'access arguments' => array(
      array(
        'permission' => 'sirius admin',
        'component' => 'btu:csg',
      ),
    ),
    'type' => MENU_LOCAL_TASK,
    'file' => 'sirius_btu.page_csg_global.inc',
    'weight' => 10,
  );

  $items['csg'] = array(
    'title' => 'Submit a Class Size Grievance',
    'page callback' => 'sirius_btu_page_csg_public',
    'page arguments' => array(),
    'access callback' => TRUE,
    'type' => MENU_NORMAL_ITEM,
    'file' => 'sirius_btu.page_csg_public.inc',
  );

  $items['csg/thanks'] = array(
    'title' => 'Submit a Class Size Grievance',
    'page callback' => 'sirius_btu_page_csg_public_thanks',
    'page arguments' => array(),
    'access callback' => TRUE,
    'type' => MENU_NORMAL_ITEM,
    'file' => 'sirius_btu.page_csg_public_thanks.inc',
  );

  $items['btu/worker-export'] = array(
    'title' => 'Contact List Export Utility',
    'page callback' => 'sirius_btu_page_worker_export',
    'page arguments' => array(),
    'access callback' => 'sirius_menu_access_callback',
    'access arguments' => array(
      array(
        'permission' => 'sirius worker admin',
        'component' => 'btu:csg',
      ),
    ),
    'type' => MENU_NORMAL_ITEM,
    'file' => 'sirius_btu.page_worker_export.inc',
  );

  return $items;
}
