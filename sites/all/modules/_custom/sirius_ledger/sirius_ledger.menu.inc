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

  /*
  $node_tabs['sirius_payment']['sirius_payment_post'] = array(
    'title' => 'Post',
    'description' => '',
    'callback' => 'sirius_ledger_node_tab_payment_post',
    'permissions' => array('sirius ledger admin'),
    'weight' => 2,
  );
  */

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

  $node_tabs['sirius_worker']['sirius_worker_ledger/stripe'] = array(
    'title' => 'Stripe',
    'description' => '',
    'callback' => 'sirius_ledger_node_tab_entity_stripe',
    'permissions' => array('sirius ledger admin'),
    'file' => 'sirius_ledger.page_entity_stripe.inc',
    'weight' => 4,
    'component' => 'ledger:stripe',
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
    'title' => 'Billing Policy',
    'callback' => 'sirius_ledger_node_tab_policy_definition',
    'permissions' => array('sirius ledger admin'),
    'weight' => 2,
  );

  $node_tabs['sirius_json_definition']['sirius_ledger_policy_definition/definition'] = array(
    'title' => 'General Settings',
    'callback' => 'sirius_ledger_node_tab_policy_definition',
    'permissions' => array('sirius ledger admin'),
    'weight' => 1,
    'type' => MENU_DEFAULT_LOCAL_TASK,
  );

  $node_tabs['sirius_json_definition']['sirius_ledger_policy_definition/monthly'] = array(
    'title' => 'Monthly Settings',
    'callback' => 'sirius_ledger_node_tab_policy_definition_monthly',
    'permissions' => array('sirius ledger admin'),
    'weight' => 2,
  );

  $node_tabs['sirius_json_definition']['sirius_ledger_policy_definition/rates'] = array(
    'title' => 'Rates',
    'callback' => 'sirius_ledger_node_tab_policy_definition_rates',
    'permissions' => array('sirius ledger admin'),
    'weight' => 2,
    'file' => 'sirius_ledger.page_policy_definition_rates.inc',
  );

  $node_tabs['sirius_json_definition']['sirius_ledger_policy_definition/rates_new'] = array(
    'title' => 'Rates (New)',
    'callback' => 'sirius_ledger_node_tab_policy_rates_new',
    'permissions' => array('sirius ledger admin'),
    'weight' => 2,
    'file' => 'sirius_ledger.page_policy_rates_new.inc',
  );

  $node_tabs['sirius_json_definition']['sirius_ledger_policy_definition/rates_test'] = array(
    'title' => 'Rates Test',
    'callback' => 'sirius_ledger_node_tab_policy_rates_test',
    'permissions' => array('sirius ledger admin'),
    'weight' => 3,
    'file' => 'sirius_ledger.page_policy_rates_test.inc',
  );

  $node_tabs['sirius_json_definition']['sirius_ledger_policy_definition/exim'] = array(
    'title' => 'Export / Import',
    'callback' => 'sirius_ledger_node_tab_policy_definition_exim',
    'permissions' => array('sirius ledger admin'),
    'weight' => 10,
  );

  $node_tabs['sirius_payperiod']['sirius_payperiod_ledger_test'] = array(
    'title' => 'Ledger Tester',
    'callback' => 'sirius_ledger_node_tab_payperiod_test',
    'permissions' => array('sirius ledger admin'),
    'weight' => 11,
  );

  foreach (array('sirius_ledger_account', 'sirius_event') as $content_type) {
    $node_tabs[$content_type][$content_type . '_auths'] = array(
      'title' => 'Authorizations',
      'callback' => 'sirius_ledger_node_tab_entity_auths',
      'permissions' => array('sirius ledger admin'),
      'weight' => 10,
      'file' => 'sirius_ledger.page_entity_auths.inc',
    );
  }

  $node_tabs['sirius_log']['sirius_log_auth'] = array(
    'title' => 'Authorization',
    'callback' => 'sirius_ledger_node_tab_auth',
    'permissions' => array('sirius ledger admin'),
    'file' => 'sirius_ledger.page_auth.inc',
    'weight' => 1,
  );

  $node_tabs['sirius_log']['sirius_log_auth_settings'] = array(
    'title' => 'Settings',
    'callback' => 'sirius_ledger_node_tab_auth_settings',
    'permissions' => array('sirius ledger admin'),
    'file' => 'sirius_ledger.page_auth_settings.inc',
    'weight' => 1,
  );

  $node_tabs['sirius_log']['sirius_log_auth_accounts'] = array(
    'title' => 'Accounts',
    'callback' => 'sirius_ledger_node_tab_auth_accounts',
    'permissions' => array('sirius ledger admin'),
    'file' => 'sirius_ledger.page_auth_accounts.inc',
    'weight' => 2,
  );

  $node_tabs['sirius_log']['sirius_log_auth_events'] = array(
    'title' => 'Events',
    'callback' => 'sirius_ledger_node_tab_auth_events',
    'permissions' => array('sirius ledger admin'),
    'file' => 'sirius_ledger.page_auth_events.inc',
    'weight' => 3,
  );

  $node_tabs['sirius_log']['sirius_log_auth_items'] = array(
    'title' => 'Items',
    'callback' => 'sirius_ledger_node_tab_auth_items',
    'permissions' => array('sirius ledger admin'),
    'file' => 'sirius_ledger.page_auth_items.inc',
    'weight' => 4,
  );

  sirius_node_tab_set_module('sirius_ledger', $node_tabs);
  return $node_tabs;
}

function sirius_ledger_term_tabs() {
  $term_tabs = array();

  $term_tabs['sirius_payment_type']['sirius_ledger_payment_type_settings'] = array(
    'title' => 'Settings',
    'description' => '',
    'callback' => 'sirius_ledger_term_tab_payment_type_settings',
    'permissions' => array('sirius ledger admin'),
    'weight' => 1,
  );

  $term_tabs['sirius_event_type']['sirius_ledger_event_type_auths'] = array(
    'title' => 'Authorizations',
    'description' => '',
    'callback' => 'sirius_ledger_term_tab_entity_type_auths',
    'permissions' => array('sirius ledger admin'),
    'weight' => 20,
    'file' => 'sirius_ledger.term_tab_entity_type_auths.inc',
  );

  sirius_node_tab_set_module('sirius_ledger', $term_tabs);
  return $term_tabs;
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

  $items['sirius/config/ledger/purge'] = array(
    'title' => 'Purge',
    'page callback' => 'sirius_ledger_page_ar_purge',
    'access arguments' => array('sirius admin'),
    'type' => MENU_LOCAL_TASK,
    'weight' => 4,
  );

  $items['sirius/ledger/stripe/config'] = array(
    'title' => 'Stripe',
    'page callback' => 'sirius_ledger_page_stripe_config',
    'page arguments' => array(),
    'access arguments' => array('sirius domain admin'),
    'type' => MENU_NORMAL_ITEM,
    'file' => 'sirius_ledger.page_stripe_config.inc',
  );

  $items['sirius/ledger/stripe/config/account'] = array(
    'title' => 'Account Information',
    'page callback' => 'sirius_ledger_page_stripe_config',
    'page arguments' => array(),
    'access arguments' => array('sirius domain admin'),
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'file' => 'sirius_ledger.page_stripe_config.inc',
    'weight' => 1,
  );

  $items['sirius/ledger/stripe/config/test'] = array(
    'title' => 'Test',
    'page callback' => 'sirius_ledger_page_stripe_test',
    'page arguments' => array(),
    'access arguments' => array('sirius domain admin'),
    'type' => MENU_LOCAL_TASK,
    'file' => 'sirius_ledger.page_stripe_test.inc',
    'weight' => 2,
  );

  $items['sirius/ledger/stripe/config/webhook'] = array(
    'title' => 'Webhook',
    'page callback' => 'sirius_ledger_page_stripe_config_webhook',
    'page arguments' => array(),
    'access arguments' => array('sirius domain admin'),
    'type' => MENU_LOCAL_TASK,
    'file' => 'sirius_ledger.page_stripe_config_webhook.inc',
    'weight' => 2,
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

 $items['sirius/ledger/transmittal/generate'] = array(
    'title' => 'Generate PDFs',
    'description' => '',
    'access arguments' => array('sirius ledger admin'),
    'page callback' => 'sirius_ledger_page_transmittal',
    'page arguments' => array(),
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => 1,
  );

  $items['sirius/ledger/transmittal/byemployer'] = array(
    'title' => 'List By Month / Employer',
    'description' => '',
    'access arguments' => array('sirius ledger admin'),
    'page callback' => 'sirius_ledger_page_transmittal_byemployer',
    'page arguments' => array(),
    'type' => MENU_LOCAL_TASK,
    'weight' => 2,
    'file' => 'sirius_ledger.page_transmittal_byemployer.inc',
  );

  /*
  $items['sirius/ledger/transmittal/balances'] = array(
    'title' => 'Balances',
    'description' => '',
    'access arguments' => array('sirius ledger admin'),
    'page callback' => 'sirius_ledger_page_transmittal_balances',
    'page arguments' => array(),
    'type' => MENU_LOCAL_TASK,
    'weight' => 3,
    'file' => 'sirius_ledger.page_transmittal_balances.inc',
  );
  */

  $items['sirius/ledger/delinquency'] = array(
    'title' => 'Delinquency Letters',
    'description' => '',
    'access arguments' => array('sirius ledger admin'),
    'page callback' => 'sirius_ledger_page_delinquency_employer',
    'page arguments' => array(),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['sirius/ledger/delinquency/employer'] = array(
    'title' => 'Employers',
    'description' => '',
    'access arguments' => array('sirius ledger admin'),
    'page callback' => 'sirius_ledger_page_delinquency_employer',
    'page arguments' => array(),
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => 2,
  );

  $items['sirius/ledger/delinquency/worker'] = array(
    'title' => 'Workers',
    'description' => '',
    'access arguments' => array('sirius ledger admin'),
    'page callback' => 'sirius_ledger_page_delinquency_worker',
    'page arguments' => array(),
    'type' => MENU_LOCAL_TASK,
    'weight' => 2,
  );

  $items['sirius/ledger/workers'] = array(
    'title' => 'Workers',
    'description' => '',
    'access arguments' => array('sirius ledger admin'),
    'page callback' => 'sirius_ledger_page_list_workers',
    'page arguments' => array(),
  );

  $items['sirius/ledger/ar/search'] = array(
    'title' => 'AR Search',
    'description' => '',
    'access arguments' => array('sirius ledger admin'),
    'page callback' => 'sirius_ledger_page_ar_search',
    'page arguments' => array(),
    'file' => 'sirius_ledger.page_ar_search.inc',
  );

  $items['sirius/ledger/stripe/paymentmethod/add'] = array(
    'title' => 'Add a Payment Method',
    'description' => '',
    'access arguments' => array('sirius'),
    'page callback' => 'sirius_ledger_page_stripe_paymentmethod_add',
    'page arguments' => array(),
    'file' => 'sirius_ledger.page_stripe_paymentmethod_add.inc',
  );


  $items['sirius/stripe/webhookendpoint/%'] = array(
    'title' => 'Webhook Endpoint',
    'description' => '',
    'access callback' => TRUE,
    'page callback' => 'sirius_ledger_stripe_webhook_handle',
    'page arguments' => array(3),
    'file' => 'sirius_ledger.stripe.inc',
    'type' => MENU_CALLBACK,
  );

  return $items;
}

