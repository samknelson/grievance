<?php

/**
 * Implements hook_menu
 */

function sirius_help_menu() {
  $items['sirius/modal/help-text-view/%node'] = array(
    'title' => 'Full Help Text',
    'page callback'    => 'sirius_help_modal_view',
    'page arguments'   => array(3),
    'access callback'  => 'sirius_help_modal_view_access',
    'access arguments' => array(3),
    'type'             => MENU_LOCAL_ACTION,
    'context'          => MENU_CONTEXT_INLINE,
    'modal'            => TRUE,
  );

  $items['sirius/help/publish'] = array(
    'title' => 'Publish Help to Sirius Code',
    'page callback'    => 'sirius_help_publish',
    'page arguments'   => array(),
    'access arguments' => array('sirius admin'),
    'type'             => MENU_NORMAL_ITEM,
  );

  $items['sirius/help/config'] = array(
    'title' => 'Help Settings',
    'page callback' => 'sirius_help_page_config',
    'page arguments' => array(),
    'access arguments' => array('sirius help admin'),
    'type' => MENU_NORMAL_ITEM,
    'file' => 'sirius_help.page_config.inc',
  );

  return $items;
}

