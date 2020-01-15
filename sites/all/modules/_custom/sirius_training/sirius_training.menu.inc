<?php

function sirius_training_node_tabs() {
  $node_tabs = array();

  $node_tabs['grievance_shop']['sirius_employer_tc'] = array(
    'title' => 'Training Center',
    'description' => '',
    'callback' => 'sirius_training_node_tab_employer',
    'permissions' => array('sirius employer user manager'),
    'weight' => 1,
  );

  $node_tabs['sirius_worker']['sirius_worker_tc'] = array(
    'title' => 'Training Center',
    'description' => '',
    'callback' => 'sirius_training_node_tab_worker',
    'permissions' => array('sirius training admin'),
    'weight' => 1,
  );

  $node_tabs['sirius_worker']['sirius_worker_tc/overview'] = array(
    'title' => 'Overview',
    'description' => '',
    'callback' => 'sirius_training_node_tab_worker_overview',
    'permissions' => array('sirius training admin'),
    'weight' => 1,
  );

  $node_tabs['sirius_worker']['sirius_worker_tc/log'] = array(
    'title' => 'Log',
    'description' => '',
    'callback' => 'sirius_training_node_tab_worker_log',
    'permissions' => array('sirius training admin'),
    'weight' => 2,
  );

  return $node_tabs;
}

function sirius_training_term_tabs() {
  $term_tabs = array();

  $term_tabs['sirius_member_status']['sirius_tc'] = array(
    'title' => 'Training Center',
    'description' => '',
    'callback' => 'sirius_training_term_tab_ms',
    'permissions' => array('sirius training admin'),
  );

  return $term_tabs;
}

function sirius_training_menu() {
  $items = array();

  /*
  $items['sirius/office/operator'] = array(
    'title' => 'Operators',
    'description' => '',
    'page callback' => 'sirius_office_page_operator',
    'page arguments' => array(),
    'access arguments' => array('sirius'),
    'type' => MENU_NORMAL_ITEM,
  );
  */

  return $items;
}
