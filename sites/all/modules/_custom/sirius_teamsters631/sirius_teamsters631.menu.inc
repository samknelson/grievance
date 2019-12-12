<?php

function sirius_teamsters631_node_tabs() {
  $node_tabs = array();

  $node_tabs['sirius_worker']['sirius_teamsters631_badge'] = array(
    'title' => 'Badge',
    'description' => '',
    'callback' => 'sirius_teamsters631_badge',
    'permissions' => array('sirius dispatch staff'),
  );


  return $node_tabs;
}

/**
 * Implements hook_menu
 */

function sirius_teamsters631_menu() {
  $items = array();

  $items['sirius/admin/teamsters631/ebnotify'] = array(
    'title' => 'Teamsters 631 Extraboard Notifications',
    'description' => '',
    'access arguments' => array('sirius teamsters631 admin'),
    'page callback' => 'drupal_get_form',
    'page arguments' => array('sirius_teamsters631_ebnotify_admin'),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/teamsters631/badge/%node/front'] = array(
    'title' => 'Teamsters 631 Badge',
    'description' => '',
    'access callback' => TRUE,
    'page callback' => 'sirius_teamsters631_badge_front',
    'page arguments' => array(3),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/teamsters631/badge/%node/contents'] = array(
    'title' => 'Teamsters 631 Badge',
    'description' => '',
    'access callback' => 'sirius_teamsters631_badge_contents_access',
    'page callback' => 'sirius_teamsters631_badge_contents',
    'page arguments' => array(3),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/teamsters631/badge/%node/headshot'] = array(
    'title' => 'Teamsters 631 Badge',
    'description' => '',
    'access callback' => 'sirius_teamsters631_badge_contents_access',
    'page callback' => 'sirius_teamsters631_badge_headshot',
    'page arguments' => array(3),
    'type' => MENU_NORMAL_ITEM,
  );

  return $items;
}
