<?php

function sirius_training_node_tabs() {
  $node_tabs = array();

  $node_tabs['grievance_shop']['sirius_employer_tc'] = array(
    'title' => 'Training Center',
    'description' => '',
    'callback' => 'sirius_training_node_tab_employer',
    'permissions' => array('sirius employer user manager'),
    'weight' => 1,
    'component' => 'training',
  );

  $node_tabs['sirius_worker']['sirius_worker_tc'] = array(
    'title' => 'Training Center',
    'description' => '',
    'callback' => 'sirius_training_node_tab_worker',
    'permissions' => array('sirius'),
    'weight' => 1,
    'component' => 'training',
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
    'component' => 'training',
  );

  $node_tabs['sirius_worker']['sirius_worker_tc/customfields'] = array(
    'title' => 'Custom Fields',
    'description' => '',
    'callback' => 'sirius_training_node_tab_worker_customfields',
    'permissions' => array('sirius'),
    'weight' => 3,
    'component' => 'training',
  );

  $node_tabs['sirius_worker']['sirius_worker_tc/certifications'] = array(
    'title' => 'Certifications',
    'description' => '',
    'callback' => 'sirius_training_node_tab_worker_certifications',
    'permissions' => array('sirius training certification view'),
    'weight' => 3,
    'component' => 'training',
  );

  $node_tabs['sirius_worker']['sirius_worker_tc/evaluations'] = array(
    'title' => 'Evaluations',
    'description' => '',
    'callback' => 'sirius_training_node_tab_worker_evaluations',
    'permissions' => array('sirius training admin'),
    'weight' => 4,
    'component' => 'training',
  );

  $node_tabs['sirius_worker']['sirius_worker_tc/evaluations_request'] = array(
    'title' => 'Request an Evaluation',
    'description' => '',
    'callback' => 'sirius_training_node_tab_worker_evaluations_request',
    'permissions' => array('sirius'),
    'weight' => 5,
    'component' => 'training',
  );

  $node_tabs['sirius_worker']['sirius_worker_tc/classes'] = array(
    'title' => 'Classes',
    'description' => '',
    'callback' => 'sirius_training_node_tab_worker_classes',
    'permissions' => array('sirius training admin'),
    'weight' => 6,
    'component' => 'training',
  );

  /*
  $node_tabs['sirius_worker']['sirius_worker_tc/log'] = array(
    'title' => 'Log',
    'description' => '',
    'callback' => 'sirius_training_node_tab_worker_log',
    'permissions' => array('sirius training admin'),
    'weight' => 10,
  );
  */

  $node_tabs['sirius_log']['sirius_log_tc/evaluation_edit'] = array(
    'title' => 'Evaluation',
    'description' => '',
    'callback' => 'sirius_training_node_tab_evaluation_edit',
    'permissions' => array('sirius training admin'),
    'weight' => 10,
    'component' => 'training',
  );

  $node_tabs['sirius_log']['sirius_log_tc/certification_view'] = array(
    'title' => 'Certification View',
    'description' => '',
    'callback' => 'sirius_training_node_tab_certification_view',
    'permissions' => array('sirius'),
    'weight' => 10,
    'component' => 'training',
  );

  $node_tabs['sirius_log']['sirius_log_tc/certification_edit'] = array(
    'title' => 'Certification Edit',
    'description' => '',
    'callback' => 'sirius_training_node_tab_certification_edit',
    'permissions' => array('sirius'),
    'weight' => 11,
    'component' => 'training',
  );

  $node_tabs['sirius_log']['sirius_log_tc/certification_signatures'] = array(
    'title' => 'Certification Signatures',
    'description' => '',
    'callback' => 'sirius_training_node_tab_certification_signatures',
    'permissions' => array('sirius'),
    'weight' => 12,
    'component' => 'training',
  );

  $node_tabs['sirius_log']['sirius_log_tc/certification_attachments'] = array(
    'title' => 'Certification Attachments',
    'description' => '',
    'callback' => 'sirius_training_node_tab_certification_attachments',
    'permissions' => array('sirius'),
    'weight' => 13,
    'file' => 'sirius_training.page_certification_attachments.inc',
    'component' => 'training',
  );

  $node_tabs['sirius_log']['sirius_log_tc/certification_grant'] = array(
    'title' => 'Certification Grant',
    'description' => '',
    'callback' => 'sirius_training_node_tab_certification_grant',
    'permissions' => array('sirius'),
    'weight' => 13,
    'component' => 'training',
  );

  $node_tabs['sirius_json_definition']['certification_definition_skills'] = array(
    'title' => 'Skills',
    'description' => '',
    'callback' => 'sirius_training_node_tab_certification_definition_skills',
    'permissions' => array('sirius training admin'),
    'weight' => 11,
    'component' => 'training',
  );

  sirius_node_tab_set_module('sirius_training', $node_tabs);

  return $node_tabs;
}

function sirius_training_term_tabs() {
  $term_tabs = array();

  $term_tabs['sirius_member_status']['sirius_tc'] = array(
    'title' => 'Training Center',
    'description' => '',
    'callback' => 'sirius_training_term_tab_ms',
    'permissions' => array('sirius training admin'),
    'component' => 'training',
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

  $items['sirius/config/training/customfields'] = array(
    'title' => 'Custom Fields',
    'page callback' => 'sirius_training_config_customfields_edit',
    'access arguments' => array('sirius training admin'),
    'type' => MENU_LOCAL_TASK,
    'weight' => 2,
  );

  $items['sirius/config/training/customfields/form'] = array(
    'title' => 'Form Edit',
    'page callback' => 'sirius_training_config_customfields_edit',
    'access arguments' => array('sirius training admin'),
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => 1,
  );

  $items['sirius/config/training/customfields/preview'] = array(
    'title' => 'Form Preview',
    'page callback' => 'sirius_training_config_customfields_preview',
    'access arguments' => array('sirius training admin'),
    'type' => MENU_LOCAL_TASK,
    'weight' => 2,
  );

  $items['sirius/config/training/customfields/exim'] = array(
    'title' => 'Form Export / Import',
    'page callback' => 'sirius_training_config_customfields_exim',
    'access arguments' => array('sirius training admin'),
    'type' => MENU_LOCAL_TASK,
    'weight' => 3,
  );

  $items['sirius/config/training/evaluations'] = array(
    'title' => 'Evaluations',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('sirius_training_config_evaluations'),
    'access arguments' => array('sirius training admin'),
    'type' => MENU_LOCAL_TASK,
    'weight' => 3,
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

  $items['sirius/config/training/evaluations/exim'] = array(
    'title' => 'Form Export / Import',
    'page callback' => 'sirius_training_config_evaluations_exim',
    'access arguments' => array('sirius training admin'),
    'type' => MENU_LOCAL_TASK,
    'weight' => 4,
  );

  $items['sirius/config/training/cetifications'] = array(
    'title' => 'Certifications',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('sirius_training_config_certifications'),
    'access arguments' => array('sirius training admin'),
    'type' => MENU_LOCAL_TASK,
    'weight' => 4,
  );

  $items['sirius/config/training/cetifications/general'] = array(
    'title' => 'General Settings',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('sirius_training_config_certifications'),
    'access arguments' => array('sirius training admin'),
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => 1,
  );

  $items['sirius/config/training/cetifications/definitions'] = array(
    'title' => 'Forms',
    'page callback' => 'sirius_training_config_certification_definitions',
    'access arguments' => array('sirius training admin'),
    'type' => MENU_LOCAL_TASK,
    'weight' => 2,
  );

  $items['sirius/config/training/level-denorm-resync'] = array(
    'title' => 'Re-Denormalize Levels',
    'page callback' => 'sirius_training_page_level_denorm_resync',
    'page arguments' => array(),
    'access arguments' => array('sirius admin'),
    'type' => MENU_LOCAL_TASK,
    'weight' => 20,
    'file' => 'sirius_training.page_level_denorm_resync.inc',
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

  $items['sirius/training/certification/%/%'] = array(
    'title' => 'Submit a Certification',
    'page callback' => 'sirius_training_certification_page_add',
    'page arguments' => array(3,4),
    'access callback' => 'sirius_training_certification_page_add_access',
    'access arguments' => array(3,4),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/training/certification/%/%/%'] = array(
    'title' => 'Submit a Certification',
    'page callback' => 'sirius_training_certification_page_add',
    'page arguments' => array(3,4,5),
    'access callback' => 'sirius_training_certification_page_add_access',
    'access arguments' => array(3,4,5),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/training/evaluations/queue'] = array(
    'title' => 'Evaluations',
    'page callback' => 'sirius_training_page_evaluations_queue',
    'page arguments' => array('draft'),
    'access arguments' => array('sirius training admin'),
    'type' => MENU_NORMAL_ITEM,
  );

  $options = sirius_training_evaluation_status_options();
  $weight = 0;
  foreach ($options as $key => $name) { 
    $items["sirius/training/evaluations/queue/$key"] = array(
      'title' => $name,
      'page callback' => 'sirius_training_page_evaluations_queue',
      'page arguments' => array($key),
      'access arguments' => array('sirius training admin'),
      'type' => ($weight==0) ? MENU_DEFAULT_LOCAL_TASK : MENU_LOCAL_TASK,
      'weight' => $weight,
    );

    ++$weight;
  }
  
  return $items;
}
