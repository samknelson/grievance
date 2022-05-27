<?php

function sirius_flood_menu() {
  $items = array();

  $items['sirius/flood'] = array(
    'title' => 'Flood Management',
    'description' => '',
    'page callback' => 'sirius_flood_page_unblock',
    'page arguments' => array(),
    'access arguments' => array('sirius admin'),
    'type' => MENU_NORMAL_ITEM,
    'file' => 'sirius_flood.page_unblock.inc',
  );

  return $items;
}
