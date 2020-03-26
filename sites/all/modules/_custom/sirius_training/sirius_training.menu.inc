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
    'permissions' => array('sirius'),
    'weight' => 1,
  );

  /*
  $node_tabs['sirius_worker']['sirius_worker_tc/overview'] = array(
    'title' => 'Overview',
    'description' => '',
    'callback' => 'sirius_training_node_tab_worker_overview',
    'permissions' => array('sirius'),
    'weight' => 1,
  );
  */

  $node_tabs['sirius_worker']['sirius_worker_tc/level'] = array(
    'title' => 'Level',
    'description' => '',
    'callback' => 'sirius_training_node_tab_worker_level',
    'permissions' => array('sirius'),
    'weight' => 2,
  );

  $node_tabs['sirius_worker']['sirius_worker_tc/evaluations'] = array(
    'title' => 'Evaluations',
    'description' => '',
    'callback' => 'sirius_training_node_tab_worker_evaluations',
    'permissions' => array('sirius training admin'),
    'weight' => 3,
  );

  $node_tabs['sirius_worker']['sirius_worker_tc/evaluations_request'] = array(
    'title' => 'Request an Evaluation',
    'description' => '',
    'callback' => 'sirius_training_node_tab_worker_evaluations_request',
    'permissions' => array('sirius'),
    'weight' => 4,
  );

  $node_tabs['sirius_worker']['sirius_worker_tc/log'] = array(
    'title' => 'Log',
    'description' => '',
    'callback' => 'sirius_training_node_tab_worker_log',
    'permissions' => array('sirius training admin'),
    'weight' => 10,
  );

  $node_tabs['sirius_log']['sirius_log_tc/evaluation_edit'] = array(
    'title' => 'Evaluation',
    'description' => '',
    'callback' => 'sirius_training_node_tab_evaluation_edit',
    'permissions' => array('sirius training admin'),
    'weight' => 10,
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

  $items['sirius/config/training'] = array(
    'title' => 'Training Center Configuration',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('sirius_training_config'),
    'access arguments' => array('sirius training admin'),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/config/training/general'] = array(
    'title' => 'General',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('sirius_training_config'),
    'access arguments' => array('sirius training admin'),
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => 1,
  );

  $items['sirius/config/training/evaluations'] = array(
    'title' => 'Evaluations',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('sirius_training_config_evaluations'),
    'access arguments' => array('sirius training admin'),
    'type' => MENU_LOCAL_TASK,
    'weight' => 2,
  );

  $items['sirius/config/training/evaluations/general'] = array(
    'title' => 'General Settings',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('sirius_training_config_evaluations'),
    'access arguments' => array('sirius training admin'),
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => 1,
  );

  $items['sirius/config/training/evaluations/form'] = array(
    'title' => 'Form Edit',
    'page callback' => 'sirius_training_config_evaluations_edit',
    'access arguments' => array('sirius training admin'),
    'type' => MENU_LOCAL_TASK,
    'weight' => 2,
  );

  $items['sirius/config/training/evaluations/preview'] = array(
    'title' => 'Form Preview',
    'page callback' => 'sirius_training_config_evaluations_preview',
    'access arguments' => array('sirius training admin'),
    'type' => MENU_LOCAL_TASK,
    'weight' => 3,
  );

  $items['sirius/training/evaluation'] = array(
    'title' => 'Submit an Evaluation',
    'page callback' => 'sirius_training_evaluation_evaluate_launch',
    'access callback' => 'sirius_training_evaluation_evaluate_launch_access',
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/training/evaluation/%'] = array(
    'title' => 'Submit an Evaluation',
    'page callback' => 'sirius_training_evaluation_evaluate',
    'page arguments' => array(3),
    'access callback' => 'sirius_training_evaluation_evaluate_access',
    'access arguments' => array(3),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/training/evaluations/queue'] = array(
    'title' => 'Evaluations Queue',
    'page callback' => 'sirius_training_page_evaluations_queue',
    'access arguments' => array('sirius training admin'),
    'type' => MENU_NORMAL_ITEM,
  );

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
