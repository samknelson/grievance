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
    'component' => 'hour:transmittal',
  );

  $node_tabs['sirius_payment']['sirius_payment_update'] = array(
    'title' => 'Update',
    'description' => '',
    'callback' => 'sirius_ledger_node_tab_payment_update',
    'permissions' => array('sirius ledger admin'),
    'weight' => 3,
  );

  $node_tabs['sirius_ledger_account']['sirius_ledger_account_ar'] = array(
    'title' => 'A/R',
    'description' => '',
    'callback' => 'sirius_ledger_node_tab_account_ar',
    'permissions' => array('sirius ledger admin'),
    'weight' => 1,
  );

  $node_tabs['sirius_ledger_account']['sirius_ledger_account_payments'] = array(
    'title' => 'Payments',
    'description' => '',
    'callback' => 'sirius_ledger_node_tab_account_payments',
    'permissions' => array('sirius ledger admin'),
    'weight' => 2,
  );

  $node_tabs['sirius_ledger_account']['sirius_ledger_account_settings'] = array(
    'title' => 'Settings',
    'description' => '',
    'callback' => 'sirius_ledger_node_tab_account_settings',
    'permissions' => array('sirius ledger admin'),
    'weight' => 3,
  );

  $node_tabs['sirius_ledger_account']['sirius_ledger_account_reset'] = array(
    'title' => 'Reset',
    'description' => '',
    'callback' => 'sirius_ledger_node_tab_account_reset',
    'permissions' => array('sirius ledger admin'),
    'weight' => 11,
    'file' => 'sirius_ledger.page_account_reset.inc',
  );

  $node_tabs['sirius_ledger_account']['sirius_ledger_account_purge'] = array(
    'title' => 'Purge',
    'description' => '',
    'callback' => 'sirius_ledger_node_tab_account_purge',
    'permissions' => array('sirius ledger admin'),
    'weight' => 11,
    'file' => 'sirius_ledger.page_account_purge.inc',
  );

  $items['sirius/config/ledger/purge'] = array(
    'title' => 'Purge',
    'page callback' => 'sirius_ledger_page_ar_purge',
    'access arguments' => array('sirius admin'),
    'type' => MENU_LOCAL_TASK,
    'weight' => 4,
    'file' => 'sirius_ledger.page_purge.inc',
  );


  foreach (array('sirius_worker', 'sirius_employer') as $content_type) {
    $node_tabs[$content_type][$content_type . '_ledger'] = array(
      'title' => 'A/R',
      'description' => '',
      'callback' => 'sirius_ledger_node_tab_entity_accounts',
      'permissions' => array('sirius'),
      'weight' => 2,
      'file' => 'sirius_ledger.page_entity_accounts.inc',
    );

    $node_tabs[$content_type][$content_type . '_ledger/ar'] = array(
      'title' => 'Accounts',
      'description' => '',
      'callback' => 'sirius_ledger_node_tab_entity_accounts',
      'permissions' => array('sirius'),
      'weight' => 1,
      'type' => MENU_DEFAULT_LOCAL_TASK,
      'file' => 'sirius_ledger.page_entity_accounts.inc',
    );
  }

  $node_tabs['sirius_worker']['sirius_worker_ledger/monthly'] = array(
    'title' => 'Monthly Charge',
    'description' => '',
    'callback' => 'sirius_ledger_node_tab_worker_monthly',
    'permissions' => array('sirius ledger admin'),
    'weight' => 3,
  );

  $node_tabs['grievance_shop']['sirius_employer_ledger/stripe_paymentmethods'] = array(
    'title' => 'Payment Methods',
    'description' => '',
    'callback' => 'sirius_ledger_node_tab_entity_stripe_paymentmethods',
    'permissions' => array('sirius'),
    'file' => 'sirius_ledger.page_entity_stripe_paymentmethods.inc',
    'weight' => 10,
    'component' => 'ledger:stripe',
  );

  $node_tabs['grievance_shop']['sirius_employer_ledger/stripe_customer'] = array(
    'title' => 'Stripe Customer',
    'description' => '',
    'callback' => 'sirius_ledger_node_tab_entity_stripe_customer',
    'permissions' => array('sirius'),
    'file' => 'sirius_ledger.page_entity_stripe_customer.inc',
    'weight' => 10,
    'component' => 'ledger:stripe',
  );

  $node_tabs['grievance_shop']['sirius_employer_ledger/ar'] = array(
    'title' => 'A/R',
    'callback' => 'sirius_ledger_node_tab_employer_ar',
    'permissions' => array('sirius'),
    'weight' => 1,
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
    'component' => 'hour:transmittal',
  );

  $node_tabs['grievance_contract_template']['sirius_employer_ledger_policy'] = array(
    'title' => 'A/R',
    'callback' => 'sirius_ledger_node_tab_entity_policy',
    'permissions' => array('sirius ledger admin'),
    'weight' => 3,
  );

  $node_tabs['sirius_json_definition']['sirius_ledger_policy_charge_plugins'] = array(
    'title' => 'Charge Plugins',
    'callback' => 'sirius_node_tab_entity_plugins',
    'permissions' => array('sirius ledger admin'),
    'weight' => 10,
    'file' => 'sirius.page_entity_plugins.inc',
    'callback arguments' => array('ledger_charge'),
    'module' => 'sirius',
  );

  $node_tabs['grievance_shop']['sirius_employer_ledger/charge_plugin_settings'] = array(
    'title' => 'Charge Plugin Settings',
    'callback' => 'sirius_ledger_node_tab_employer_charge_plugin_settings',
    'permissions' => array('sirius ledger admin'),
    'weight' => 10,
    'file' => 'sirius_ledger.page_employer_charge_plugin_settings.inc',
  );
  
  $node_tabs['sirius_event']['sirius_event_charge_plugin_settings'] = array(
    'title' => 'Fees',
    'callback' => 'sirius_ledger_node_tab_event_charge_plugin_settings',
    'permissions' => array('sirius ledger admin'),
    'weight' => 10,
    'component' => 'ledger:charge:event',
    'file' => 'sirius_ledger.page_event_charge_plugin_settings.inc',
  );

  $node_tabs['sirius_event_participant']['sirius_ledger_event_participant_test'] = array(
    'title' => 'Ledger Tester',
    'callback' => 'sirius_ledger_node_tab_event_participant_test',
    'permissions' => array('sirius ledger admin'),
    'weight' => 11,
    'file' => 'sirius_ledger.page_event_participant_test.inc',
    'component' => 'ledger:charge:event',
  );

  $node_tabs['sirius_payperiod']['sirius_payperiod_ledger_test'] = array(
    'title' => 'Ledger Tester',
    'callback' => 'sirius_ledger_node_tab_payperiod_test',
    'permissions' => array('sirius ledger admin'),
    'weight' => 11,
    'file' => 'sirius_ledger.page_payperiod_test.inc',
    'component' => 'ledger:worker:charge:hourly',
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

  $node_tabs['sirius_log']['sirius_ea_ar'] = array(
    'title' => 'A/R',
    'callback' => 'sirius_ledger_node_tab_ea_ar',
    'permissions' => array('sirius'),
    'file' => 'sirius_ledger.page_ea_ar.inc',
    'weight' => 1,
    'skip_node_view_check' => TRUE,
  );

  $node_tabs['sirius_log']['sirius_ea_invoices'] = array(
    'title' => 'Invoices',
    'callback' => 'sirius_ledger_node_tab_ea_invoices',
    'permissions' => array('sirius'),
    'file' => 'sirius_ledger.page_ea_invoices.inc',
    'weight' => 1,
    'skip_node_view_check' => TRUE,
  );

  $node_tabs['sirius_log']['sirius_ea_payments'] = array(
    'title' => 'Payments and Adjustments',
    'callback' => 'sirius_ledger_node_tab_ea_payments',
    'permissions' => array('sirius'),
    'file' => 'sirius_ledger.page_ea_payments.inc',
    'weight' => 2,
    'skip_node_view_check' => TRUE,
  );

  $node_tabs['sirius_log']['sirius_ea_payment_create'] = array(
    'title' => 'Add an Offline Payment',
    'callback' => 'sirius_ledger_node_tab_ea_payment_create',
    'permissions' => array('sirius'),
    'file' => 'sirius_ledger.page_ea_payment_create.inc',
    'weight' => 3,
    'skip_node_view_check' => TRUE,
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

  $term_tabs['sirius_event_type']['sirius_event_type_charge_plugin_settings'] = array(
    'title' => 'Fees',
    'callback' => 'sirius_ledger_term_tab_event_type_charge_plugin_settings',
    'permissions' => array('sirius ledger admin'),
    'weight' => 10,
    'component' => 'ledger:charge:event',
    'file' => 'sirius_ledger.page_event_type_charge_plugin_settings.inc',
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

  $items['sirius/config/ledger/invoices'] = array(
    'title' => 'Invoices',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('sirius_ledger_config_invoices_form'),
    'access arguments' => array('sirius ledger admin'),
    'type' => MENU_LOCAL_TASK,
    'weight' => 2,
    'file' => 'sirius_ledger.page_config_invoices.inc',
  );

  $items['sirius/config/ledger/policies'] = array(
    'title' => 'Policies',
    'page callback' => 'sirius_ledger_page_policies',
    'access arguments' => array('sirius ledger admin'),
    'type' => MENU_LOCAL_TASK,
    'weight' => 2,
    'file' => 'sirius_ledger.page_policies.inc'
  );

  $items['sirius/config/ledger/test-monthly'] = array(
    'title' => 'Tester - Monthly Charges',
    'page callback' => 'sirius_ledger_page_monthly_test',
    'type' => MENU_LOCAL_TASK,
    'weight' => 2,
    'file' => 'sirius_ledger.page_monthly_test.inc',
    'access callback' => 'sirius_menu_access_callback',
    'access arguments' => array(
      array(
        'permission' => 'sirius ledger admin',
        'component' => 'ledger:worker:charge:monthly',
      ),
    ),
  );

  $items['sirius/config/ledger/transmittal'] = array(
    'title' => 'Transmittal Sheets',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('sirius_ledger_config_transmittal'),
    'access callback' => 'sirius_menu_access_callback',
    'access arguments' => array(
      array(
        'permission' => 'sirius ledger admin',
        'component' => 'hour:transmittal',
      ),
    ),
    'type' => MENU_LOCAL_TASK,
    'weight' => 3,
  );

  $items['sirius/config/ledger/dev'] = array(
    'title' => 'Dev Tools',
    'page callback' => 'sirius_ledger_page_dev',
    'access arguments' => array('sirius admin'),
    'type' => MENU_LOCAL_TASK,
    'weight' => 4,
    'file' => 'sirius_ledger.page_dev.inc',
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

 $items['sirius/ledger/transmittal'] = array(
    'title' => 'Transmittal Sheets',
    'description' => '',
    'access callback' => 'sirius_menu_access_callback',
    'access arguments' => array(
      array(
        'permission' => 'sirius ledger admin',
        'component' => 'hour:transmittal',
      ),
    ),
    'page callback' => 'sirius_ledger_page_transmittal',
    'page arguments' => array(),
    'type' => MENU_NORMAL_ITEM,
  );

 $items['sirius/ledger/transmittal/generate'] = array(
    'title' => 'Generate PDFs',
    'description' => '',
    'access callback' => 'sirius_menu_access_callback',
    'access arguments' => array(
      array(
        'permission' => 'sirius ledger admin',
        'component' => 'hour:transmittal',
      ),
    ),
    'page callback' => 'sirius_ledger_page_transmittal',
    'page arguments' => array(),
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => 1,
  );

  $items['sirius/ledger/transmittal/byemployer'] = array(
    'title' => 'List By Month / Employer',
    'description' => '',
    'access callback' => 'sirius_menu_access_callback',
    'access arguments' => array(
      array(
        'permission' => 'sirius ledger admin',
        'component' => 'hour:transmittal',
      ),
    ),
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

  $items['sirius/ledger/balance/search/worker'] = array(
    'title' => 'Balance Search - Workers',
    'description' => '',
    'page callback' => 'sirius_ledger_page_balance_search_worker',
    'page arguments' => array(),
    'access callback' => 'sirius_menu_access_callback',
    'access arguments' => array(
      array(
        'permission' => 'sirius ledger admin',
        // 'component' => '...',
      ),
    ),
    'file' => 'sirius_ledger.page_balance_search_worker.inc',
  );


  $items['sirius/modal/ledger/ar/view/%'] = array(
    'title' => 'AR View',
    'description' => '',
    'access arguments' => array('sirius admin'),
    'page callback' => 'sirius_ledger_page_ar_view',
    'page arguments' => array(5),
    'file' => 'sirius_ledger.page_ar_view.inc',
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

/**
 * Implments hook_modal_paths
 */

function sirius_ledger_modal_paths() {
  $paths = array();

  $paths['sirius/modal/ledger/ar/view/%'] = array('style' => 'sirius-modal-style');

  return $paths;
}
