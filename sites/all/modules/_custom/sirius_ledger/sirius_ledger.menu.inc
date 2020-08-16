<?php

function sirius_ledger_node_tabs() {
  $node_tabs = array();

  $node_tabs['sirius_payment']['sirius_payment_allocate'] = array(
    'title' => 'A/R',
    'description' => '',
    'callback' => 'sirius_ledger_node_tab_payment_allocate',
    'permissions' => array('sirius ledger admin'),
    'weight' => 1,
  );

  $node_tabs['sirius_payment']['sirius_payment_allocate/view'] = array(
    'title' => 'View',
    'description' => '',
    'callback' => 'sirius_ledger_node_tab_payment_allocate',
    'permissions' => array('sirius ledger admin'),
    'weight' => 1,
    'type' => MENU_DEFAULT_LOCAL_TASK,
  );

  $node_tabs['sirius_payment']['sirius_payment_allocate/simple'] = array(
    'title' => 'Simple',
    'description' => '',
    'callback' => 'sirius_ledger_node_tab_payment_allocate_simple',
    'permissions' => array('sirius ledger admin'),
    'weight' => 2,
  );

  $node_tabs['sirius_payment']['sirius_payment_allocate/manual'] = array(
    'title' => 'Manual',
    'description' => '',
    'callback' => 'sirius_ledger_node_tab_payment_allocate_manual',
    'permissions' => array('sirius ledger admin'),
    'weight' => 3,
  );

  $node_tabs['sirius_payment']['sirius_payment_allocate/upload'] = array(
    'title' => 'Upload',
    'description' => '',
    'callback' => 'sirius_ledger_node_tab_payment_allocate_upload',
    'permissions' => array('sirius ledger admin'),
    'weight' => 4,
  );

  $node_tabs['sirius_payment']['sirius_payment_allocate/transmittal'] = array(
    'title' => 'Transmittal',
    'description' => '',
    'callback' => 'sirius_ledger_node_tab_payment_allocate_transmittal',
    'permissions' => array('sirius ledger admin'),
    'weight' => 4,
  );

  $node_tabs['sirius_payment']['sirius_payment_post'] = array(
    'title' => 'Post',
    'description' => '',
    'callback' => 'sirius_ledger_node_tab_payment_post',
    'permissions' => array('sirius ledger admin'),
    'weight' => 2,
  );

  $node_tabs['sirius_payment']['sirius_payment_update'] = array(
    'title' => 'Update',
    'description' => '',
    'callback' => 'sirius_ledger_node_tab_payment_update',
    'permissions' => array('sirius ledger admin'),
    'weight' => 3,
  );

  $node_tabs['sirius_ledger_account']['sirius_ledger_account_payments'] = array(
    'title' => 'Payments',
    'description' => '',
    'callback' => 'sirius_ledger_node_tab_account_payments',
    'permissions' => array('sirius ledger admin'),
    'weight' => 2,
  );

  $node_tabs['sirius_ledger_account']['sirius_ledger_account_ar'] = array(
    'title' => 'A/R',
    'description' => '',
    'callback' => 'sirius_ledger_node_tab_account_ar',
    'permissions' => array('sirius ledger admin'),
    'weight' => 2,
  );

  $node_tabs['sirius_ledger_account']['sirius_ledger_account_settings'] = array(
    'title' => 'Settings',
    'description' => '',
    'callback' => 'sirius_ledger_node_tab_account_settings',
    'permissions' => array('sirius ledger admin'),
    'weight' => 2,
  );

  $node_tabs['sirius_contact']['sirius_contact_payments'] = array(
    'title' => 'Payments',
    'description' => '',
    'callback' => 'sirius_ledger_node_tab_contact_payments',
    'permissions' => array('sirius ledger admin'),
    'weight' => 2,
  );


  $node_tabs['sirius_worker']['sirius_worker_ledger'] = array(
    'title' => 'A/R',
    'description' => '',
    'callback' => 'sirius_ledger_node_tab_worker_ar',
    'permissions' => array('sirius ledger admin'),
    'weight' => 2,
  );

  $node_tabs['sirius_worker']['sirius_worker_ledger/ar'] = array(
    'title' => 'A/R',
    'description' => '',
    'callback' => 'sirius_ledger_node_tab_worker_ar',
    'permissions' => array('sirius ledger admin'),
    'weight' => 1,
    'type' => MENU_DEFAULT_LOCAL_TASK,
  );

  $node_tabs['sirius_worker']['sirius_worker_ledger/payments'] = array(
    'title' => 'Payments and Adjustments',
    'description' => '',
    'callback' => 'sirius_ledger_node_tab_worker_payments',
    'permissions' => array('sirius ledger admin'),
    'weight' => 2,
  );

  $node_tabs['sirius_worker']['sirius_worker_ledger/monthly'] = array(
    'title' => 'Monthly Charge',
    'description' => '',
    'callback' => 'sirius_ledger_node_tab_worker_monthly',
    'permissions' => array('sirius ledger admin'),
    'weight' => 3,
  );

  $node_tabs['grievance_shop']['sirius_employer_ledger'] = array(
    'title' => 'A/R',
    'callback' => 'sirius_ledger_node_tab_employer_ar',
    'permissions' => array('sirius ledger admin'),
    'weight' => 2,
  );

  $node_tabs['grievance_shop']['sirius_employer_ledger/ar'] = array(
    'title' => 'A/R',
    'callback' => 'sirius_ledger_node_tab_employer_ar',
    'permissions' => array('sirius ledger admin'),
    'weight' => 1,
    'type' => MENU_DEFAULT_LOCAL_TASK,
  );

  $node_tabs['grievance_shop']['sirius_employer_ledger/payments'] = array(
    'title' => 'Payments',
    'callback' => 'sirius_ledger_node_tab_employer_payments',
    'permissions' => array('sirius ledger admin'),
    'weight' => 2,
  );

  $node_tabs['grievance_shop']['sirius_employer_ledger/policy'] = array(
    'title' => 'Policy',
    'callback' => 'sirius_ledger_node_tab_entity_policy',
    'permissions' => array('sirius ledger admin'),
    'weight' => 3,
  );

  $node_tabs['grievance_shop']['sirius_employer_ledger/transmittal'] = array(
    'title' => 'Transmittal',
    'callback' => 'sirius_ledger_node_tab_employer_transmittal',
    'permissions' => array('sirius ledger admin'),
    'weight' => 4,
  );

  $node_tabs['grievance_contract_template']['sirius_employer_ledger_policy'] = array(
    'title' => 'A/R',
    'callback' => 'sirius_ledger_node_tab_entity_policy',
    'permissions' => array('sirius ledger admin'),
    'weight' => 3,
  );


  $node_tabs['sirius_json_definition']['sirius_ledger_policy_definition'] = array(
    'title' => 'Policy',
    'callback' => 'sirius_ledger_node_tab_policy_definition',
    'permissions' => array('sirius ledger admin'),
    'weight' => 2,
  );
  $node_tabs['sirius_json_definition']['sirius_ledger_policy_definition/definition'] = array(
    'title' => 'Settings',
    'callback' => 'sirius_ledger_node_tab_policy_definition',
    'permissions' => array('sirius ledger admin'),
    'weight' => 1,
    'type' => MENU_DEFAULT_LOCAL_TASK,
  );
  $node_tabs['sirius_json_definition']['sirius_ledger_policy_definition/rates'] = array(
    'title' => 'Rates',
    'callback' => 'sirius_ledger_node_tab_policy_definition_rates',
    'permissions' => array('sirius ledger admin'),
    'weight' => 2,
  );
  $node_tabs['sirius_json_definition']['sirius_ledger_policy_definition/exim'] = array(
    'title' => 'Export / Import',
    'callback' => 'sirius_ledger_node_tab_policy_definition_exim',
    'permissions' => array('sirius ledger admin'),
    'weight' => 3,
  );

  return $node_tabs;
}


/**
 * Implements hook_menu
 */

function sirius_ledger_menu() {
  $items = array();

  $items['sirius/ledger/config'] = array(
    'title' => 'Ledger Configuration',
    'description' => '',
    'access arguments' => array('sirius ledger admin'),
    'page callback' => 'drupal_get_form',
    'page arguments' => array('sirius_ledger_config'),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/config/ledger'] = array(
    'title' => 'Ledger Configuration',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('sirius_ledger_config'),
    'access arguments' => array('sirius ledger admin'),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/config/ledger/general'] = array(
    'title' => 'General',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('sirius_ledger_config'),
    'access arguments' => array('sirius ledger admin'),
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => 1,
  );

  $items['sirius/config/ledger/policies'] = array(
    'title' => 'Policies',
    'page callback' => 'sirius_ledger_config_policies',
    'access arguments' => array('sirius ledger admin'),
    'type' => MENU_LOCAL_TASK,
    'weight' => 2,
  );

  $items['sirius/config/ledger/test-monthly'] = array(
    'title' => 'Tester - Monthly Charges',
    'page callback' => 'sirius_ledger_page_monthly_test',
    'access arguments' => array('sirius ledger admin'),
    'type' => MENU_LOCAL_TASK,
    'weight' => 2,
  );

  $items['sirius/config/ledger/transmittal'] = array(
    'title' => 'Transmittal Sheets',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('sirius_ledger_config_transmittal'),
    'access arguments' => array('sirius ledger admin'),
    'type' => MENU_LOCAL_TASK,
    'weight' => 3,
  );


 $items['sirius/ledger/payment/create'] = array(
    'title' => 'Create a Payment',
    'description' => '',
    'access arguments' => array('sirius ledger admin'),
    'page callback' => 'sirius_ledger_page_payment_create',
    'page arguments' => array(),
    'type' => MENU_NORMAL_ITEM,
  );


 $items['sirius/ledger/transmittal'] = array(
    'title' => 'Transmittal Sheets',
    'description' => '',
    'access arguments' => array('sirius ledger admin'),
    'page callback' => 'sirius_ledger_page_transmittal',
    'page arguments' => array(),
    'type' => MENU_NORMAL_ITEM,
  );

 $items['sirius/ledger/transmittal/render'] = array(
    'title' => 'Transmittal Sheets',
    'description' => '',
    'access arguments' => array('sirius ledger admin'),
    'page callback' => 'sirius_ledger_page_transmittal_render',
    'page arguments' => array(),
    'type' => MENU_NORMAL_ITEM,
  );

  return $items;
}

