<?php

/**
 * Implements hook_node_tabs
 */

function sirius_worker_rating_node_tabs() {
  $node_tabs = array();

  $node_tabs['sirius_worker']['sirius_worker_rating'] = array(
    'title' => 'Rating',
    'description' => '',
    'callback' => 'sirius_worker_rating_node_tab_worker',
    'permissions' => array(),
    'file' => 'sirius_worker_rating.page_worker.inc',
    'skip_node_view_check' => TRUE,
    'component' => 'worker:rating',
    'weight' => 4,
  );

  sirius_node_tab_set_module('sirius_worker_rating', $node_tabs);

  return $node_tabs;
}

function sirius_worker_rating_menu() {
  $items = array();

  $items['sirius/worker/rating'] = array(
    'title' => 'Ratings',
    'page callback' => 'sirius_worker_rating_page_list',
    'page arguments' => array(),
    'access callback' => 'sirius_menu_access_callback',
    'access arguments' => array(
      array(
        'component' => 'worker:rating',
        'access' => array(
          'type' => 'worker_rating',
          'action' => 'list',
          'context' => array(),
        ),
      ),
    ),
    'type' => MENU_NORMAL_ITEM,
    'file' => 'sirius_worker_rating.page_list.inc',
  );

  $items['sirius/worker/rating/list'] = array(
    'title' => 'List',
    'page callback' => 'sirius_worker_rating_page_list',
    'page arguments' => array(),
    'access callback' => 'sirius_menu_access_callback',
    'access arguments' => array(
      array(
        'component' => 'worker:rating',
        'access' => array(
          'type' => 'worker_rating',
          'action' => 'list',
          'context' => array(),
        ),
      ),
    ),
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'file' => 'sirius_worker_rating.page_list.inc',
    'weight' => 1,
  );

  $items['sirius/worker/rating/conf'] = array(
    'title' => 'Config',
    'page callback' => 'sirius_worker_rating_page_config',
    'page arguments' => array(),
    'access callback' => 'sirius_menu_access_callback',
    'access arguments' => array(
      array(
        'component' => 'worker:rating',
        'access' => array(
          'type' => 'worker_rating',
          'action' => 'config',
          'context' => array(),
        ),
      ),
    ),
    'type' => MENU_LOCAL_TASK,
    'file' => 'sirius_worker_rating.page_config.inc',
    'weight' => 2,
  );

  return $items;
}
