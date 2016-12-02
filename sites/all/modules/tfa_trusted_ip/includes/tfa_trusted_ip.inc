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

  public function getForm(array $form, array &$form_state) { 
    watchdog('Whatever', 'getForm');

    $form = array();

    $form['ignore'] = array(
      '#type' => 'textfield',
      '#title' => t('Ignore me.'),
    );
    return $form;
  }

  public function validateForm(array $form, array &$form_state) {
    watchdog('Whatever', 'validateForm');
    return TRUE;
  }

  public function submitForm(array $form, array &$form_state) {
    watchdog('Whatever', 'validateForm');
    return TRUE;
  }

  public function finalize() {
    watchdog('Whatever', 'finalize');
    $name = $this->getAgent();
    $this->setTrusted($this->generateBrowserId(), $name);
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
