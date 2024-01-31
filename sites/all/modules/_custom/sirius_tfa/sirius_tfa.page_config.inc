<?php

sirius_require('tfa', 'tfa.admin.inc');

function sirius_tfa_page_config() { 
	return drupal_get_form('sirius_tfa_page_config_form');
}

function sirius_tfa_page_config_form($form, &$form_state) {

	$roles = user_roles(TRUE);

  $form['sirius_tfa_required_roles'] = array(
    '#type' => 'checkboxes',
    '#options' => $roles,
    '#title' => t('Required TFA'),
    '#default_value' => variable_get('sirius_tfa_required_roles', array()),
    '#prefix' => '<p>' . t('The global "require role" setting requires that TFA be provisioned BEFORE the account can be used. This prevents self-provisioning. If, instead, users are supposed to provision their own TFA accounts, make sure that no roles are checked on the global settings page, and instead use the setting here.') . '</p>',
  );

  $form['sirius_tfa_required_msg'] = array(
    '#type' => 'textarea',
    '#title' => t('Required TFA Message'),
    '#default_value' => variable_get('sirius_tfa_required_msg', 'Before accessing the site, please configure two-factor authentication.'),
    '#description' => t('Enter the message that required-tfa users receive when they are redirected.'),
  );


  return system_settings_form($form);
}
