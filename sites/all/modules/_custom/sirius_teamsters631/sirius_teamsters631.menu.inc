<?php

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

  return $items;
}

