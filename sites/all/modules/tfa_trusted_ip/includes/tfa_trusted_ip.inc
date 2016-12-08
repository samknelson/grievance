<?php

/**
 * @file classes for TFA basic plugin
 */

/**
 * Class TfaTrustedBrowser
 */
class TfaTrustedIP extends TfaBasePlugin implements TfaLoginPluginInterface {
  public function __construct(array $context) {
    parent::__construct($context);
  }

  public function loginAllowed() {
    $ip_text = variable_get('tfa_trusted_ip', '');
    $ip_regexs = explode('/\s+/', $ip_text);
    $ip_address = ip_address();

    foreach ($ip_regexs as $ip_regex) {
      if (preg_match("/$ip_regex/", $ip_address)) { 
        watchdog('tfa_trusted_ip', "Allowing login from $ip_address because it matches whitelisted regular expression $ip_regex.");
        return true;
      }
    }

    return FALSE;
  }
}

/**
 * Class TfaTrustedBrowserSetup
 */
class TfaTrustedIPSetup extends TfaTrustedBrowser implements TfaSetupPluginInterface {

  public function __construct(array $context) {
    parent::__construct($context);
  }

  public function getSetupForm(array $form, array &$form_state) { return array(); }
  public function submitSetupForm(array $form, array &$form_state) { }
  public function validateSetupForm(array $form, array &$form_state) { return TRUE; }
}
