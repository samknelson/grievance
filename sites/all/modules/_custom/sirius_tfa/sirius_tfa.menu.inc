<?php

function sirius_tfa_menu() {
  $items = array();

  $items['sirius/tfa/reauthenticate'] = array(
    'title' => 'Verify access to secure page',
    'page callback' => 'sirius_tfa_page_reauthenticate',
    'page arguments' => array(),
    'access arguments' => array('sirius'),
    'type' => MENU_NORMAL_ITEM,
    'file' => 'sirius_tfa.page_reauthenticate.inc'
  );

  $items['user/%user/sirius-tfa'] = array(
    'title' => 'Two-Factor Authentication',
    'page callback' => 'sirius_tfa_page_overview',
    'page arguments' => array(1),
    'access callback' => 'sirius_tfa_plugin_access',
    'access arguments' => array(1, 'view'),
    'type' => MENU_LOCAL_TASK,
    'file' => 'sirius_tfa.page_overview.inc',
  );

  $items['user/%user/sirius-tfa/view'] = array(
    'title' => 'Overview',
    'page callback' => 'sirius_tfa_page_overview',
    'page arguments' => array(1),
    'access callback' => 'sirius_tfa_plugin_access',
    'access arguments' => array(1, 'view'),
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'file' => 'sirius_tfa.page_overview.inc',
    'weight' => 0,
  );

  $items['user/%user/sirius-tfa/sms-setup'] = array(
    'title' => 'SMS',
    'page callback' => 'sirius_tfa_page_provision',
    'page arguments' => array(1, 'tfa_basic_sms'),
    'access callback' => 'sirius_tfa_plugin_access',
    'access arguments' => array(1, 'tfa_basic_sms'),
    'type' => MENU_LOCAL_TASK,
    'file' => 'sirius_tfa.page_provision.inc',
    'weight' => 1,
  );

  $items['user/%user/sirius-tfa/app-setup'] = array(
    'title' => 'TOTP',
    'page callback' => 'sirius_tfa_page_provision',
    'page arguments' => array(1, 'tfa_basic_totp'),
    'access callback' => 'sirius_tfa_plugin_access',
    'access arguments' => array(1, 'tfa_basic_totp'),
    'type' => MENU_LOCAL_TASK,
    'file' => 'sirius_tfa.page_provision.inc',
    'weight' => 1,
  );

  $items['user/%user/sirius-tfa/trusted-browsers'] = array(
    'title' => 'Trusted Browsers',
    'page callback' => 'sirius_tfa_page_provision',
    'page arguments' => array(1, 'tfa_basic_trusted_browser'),
    'access callback' => 'sirius_tfa_plugin_access',
    'access arguments' => array(1, 'tfa_basic_trusted_browser'),
    'type' => MENU_LOCAL_TASK,
    'file' => 'sirius_tfa.page_provision.inc',
    'weight' => 2,
  );

  $items['user/%user/sirius-tfa/recovery-codes-list'] = array(
    'title' => 'View Recovery Codes',
    'page callback' => 'sirius_tfa_page_provision',
    'page arguments' => array(1, 'recovery_codes_list'),
    'access callback' => 'sirius_tfa_plugin_access',
    'access arguments' => array(1, 'tfa_basic_recovery_code'),
    'type' => MENU_LOCAL_TASK,
    'file' => 'sirius_tfa.page_provision.inc',
    'weight' => 3,
  );

  $items['user/%user/sirius-tfa/recovery-codes'] = array(
    'title' => 'Generate New Recovery Codes',
    'page callback' => 'sirius_tfa_page_provision',
    'page arguments' => array(1, 'tfa_basic_recovery_code'),
    'access callback' => 'sirius_tfa_plugin_access',
    'access arguments' => array(1, 'tfa_basic_recovery_code'),
    'type' => MENU_LOCAL_TASK,
    'file' => 'sirius_tfa.page_provision.inc',
    'weight' => 4,
  );

  $items['user/%user/sirius-tfa/disable'] = array(
    'title' => 'Disable TFA',
    'page callback' => 'sirius_tfa_page_disable',
    'page arguments' => array(1),
    'access callback' => 'sirius_tfa_plugin_access',
    'access arguments' => array(1, 'disable'),
    'type' => MENU_LOCAL_TASK,
    'file' => 'sirius_tfa.page_disable.inc',
    'weight' => 10,
  );


  $items['admin/config/people/tfa/default'] = array(
    'title' => 'Global Settings',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('tfa_admin_settings'),
    'access arguments' => array('admin tfa settings'),
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'file' => 'sirius_tfa.page_config.inc',
    'weight' => 0,
  );

  $items['admin/config/people/tfa/sirius'] = array(
    'title' => 'Sirius-Specific Settings',
    'page callback' => 'sirius_tfa_page_config',
    'page arguments' => array(),
    'access arguments' => array('admin tfa settings'),
    'type' => MENU_LOCAL_TASK,
    'file' => 'sirius_tfa.page_config.inc',
    'weight' => 1,
  );

  return $items;
}

function sirius_tfa_plugin_access($account, $plugin) {
  // Must know which account
  if (!$account || !$account->uid) { return FALSE; }

  // Must have access to that account
  $account_access = FALSE;
  $my_account = ($GLOBALS['user']->uid == $account->uid);
  if ($my_account && user_access('sirius tfa setup own')) { $account_access = TRUE; }
  if (user_access('administer users')) { $account_access = TRUE; }
  if (!$account_access) { return FALSE; }

  // We can always "view" or "setup"
  if (in_array($plugin, array('view', 'setup'))) { return TRUE; }

  // Which plugins are enabled?
  $user_tfa = tfa_basic_get_tfa_data($account);
  $enabled = $user_tfa['status'];
  $validate_plugin = variable_get('tfa_validate_plugin', '');

  // "Disable" is available only if TFA is enabled
  if ( ($plugin == 'disable') && $enabled) { return TRUE; }

  // Validate plugin is always enabled
  if ($plugin == $validate_plugin) { return TRUE; }

  // Fallback and login plugins are enabled only if TFA is enabled
  if ($enabled) {
    if (in_array($plugin, variable_get('tfa_fallback_plugins', array()))) { return TRUE; }
    if (in_array($plugin, variable_get('tfa_login_plugins', array()))) { return TRUE; }
  }

  // Oh well.
  return FALSE;
}
