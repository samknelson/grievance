<?php

function grievance_uh_menu() {
  $items = array();

  $items['grievance/uh/config'] = array(
    'title' => 'Grievance - UH',
    'page callback' => 'grievance_uh_page_config',
    'page arguments' => array(),
    'access arguments' => array('grievance admin'),
    'file' => 'grievance_uh.page_config.inc',
    'type' => MENU_NORMAL_ITEM,
  );

  $items['grievance/uh/snapshot'] = array(
    'title' => 'Snapshot',
    'page callback' => 'grievance_uh_page_snapshot',
    'page arguments' => array(),
    'access arguments' => array('grievance admin'),
    'type' => MENU_NORMAL_ITEM,
    'file' => 'grievance_uh.page_snapshot.inc',
  );

  return $items;
}
