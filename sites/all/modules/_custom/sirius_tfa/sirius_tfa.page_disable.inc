<?php

function sirius_tfa_page_disable($account) {
	// Require a recent password in order to continue
	sirius_tfa_reauthenticate();

	// Load tfa_basic functions
	sirius_require('tfa_basic', 'tfa_basic.pages.inc');

	// I'd really prefer to simply do this:
	// return drupal_get_form('tfa_basic_disable_form', $account);
	// But I don't like the way tfa_basic handles things, in particular the fact that you
	// have to re-enter your password for every page. So I'm copying-and-pasting.

	return drupal_get_form('sirius_tfa_page_disable_form', $account);
}

function sirius_tfa_page_disable_form($form, &$form_state, $account) {
  global $user;

  $form_state['storage']['account'] = $account;

  if ($account->uid != $user->uid && user_access('administer users')) {
    $preamble_desc = t('Are you sure you want to disable TFA on account %name?', array('%name' => $account->name));
    $notice_desc = t('TFA settings and data will be lost. %name can re-enable TFA again from their profile.', array('%name' => $account->name));
    if (tfa_basic_tfa_required($account)) {
      drupal_set_message(t("This account is required to have TFA enabled per the 'require TFA' permission on one of their roles. Disabling TFA will remove their ability to log back into the site. If you continue, consider also removing the role so they can authenticate and setup TFA again."), 'warning');
    }
  }
  else {
    $preamble_desc = t('Are you sure you want to disable your two-factor authentication setup?');
    $notice_desc = t("Your settings and data will be lost. You can re-enable two-factor authentication again from your profile.");
    if (tfa_basic_tfa_required($account)) {
      drupal_set_message(t('Your account must have at least one two-factor authentication method enabled. Continuing will disable your ability to log back into this site.'), 'warning');
      $notice_desc = t('Your settings and data will be lost and you will be unable to log back into the site. To regain access contact a site administrator.');
    }
  }
  $form['preamble'] = array(
    '#prefix' => '<p class="preamble">',
    '#suffix' => '</p>',
    '#markup' => $preamble_desc,
  );
  $form['notice'] = array(
    '#prefix' => '<p class="preamble">',
    '#suffix' => '</p>',
    '#markup' => $notice_desc,
  );
  $form['confirm'] = array(
  	'#type' => 'checkbox',
  	'#required' => TRUE,
  	'#title' => t('Yes, please disable two-factor authentication for this account, and remove all settings. This action cannot be undone.'),
  );
  $form['account']['mail'] = array(
    '#type' => 'value',
    '#value' => $account->mail,
  );
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Disable'),
  );
  $form['actions']['cancel'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel'),
    '#limit_validation_errors' => array(),
    '#submit' => array('tfa_basic_disable_form_submit'),
  );

  return $form;
}

/**
 * Disable form submit.
 */
function sirius_tfa_page_disable_form_submit($form, &$form_state) {
  $account = $form_state['storage']['account'];
  if ($form_state['values']['op'] === $form_state['values']['cancel']) {
    drupal_set_message(t('TFA disable canceled.'));
    $form_state['redirect'] = 'user/' . $account->uid . '/security/tfa';
    return;
  }
  $params = array('account' => $account);
  tfa_basic_setup_save_data($account, array('status' => FALSE));
  // Delete TOTP code.
  $totp = new TfaTotp(array('uid' => $account->uid));
  $totp->deleteSeed();
  // Delete recovery codes.
  $recovery = new TfaBasicRecoveryCodeSetup(array('uid' => $account->uid));
  $recovery->deleteCodes();
  // Delete trusted browsers.
  $trusted = new TfaTrustedBrowserSetup(array('uid' => $account->uid));
  $trusted->deleteTrustedBrowsers();

  watchdog('tfa_basic', 'TFA disabled for user @name UID !uid', array(
    '@name' => $account->name,
    '!uid' => $account->uid,
  ), WATCHDOG_NOTICE);

  // E-mail account to inform user that it has been disabled.
  drupal_mail('tfa_basic', 'tfa_basic_disabled_configuration', $account->mail, user_preferred_language($account), $params);

  drupal_set_message(t('TFA has been disabled.'));
  $form_state['redirect'] = 'user/' . $account->uid . '/sirius-tfa';
}
