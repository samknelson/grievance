<?php

function sirius_ledger_config($form, &$form_state) {
  /*
  $name = sirius_domain_variable_name('sirius_training_certifications_enabled');
  $form[$name] = array(
    '#type' => 'select',
    '#title' => t('Certifications Enabled?'),
    '#options' => array(
      'No' => t('No'),
      'Yes' => t('Yes'),
    ),
    '#description' => t('Select "Yes" to enable worker certifications.'),
    '#default_value' => variable_get($name, ''),
  );
  */

  $policies = sirius_ledger_policy_list();
  $policy_options = array(NULL => t('-- Select --'));
  foreach ($policies as $policy_nid => $policy_node) { $policy_options[$policy_nid] = $policy_node->title; }

  $name = sirius_domain_variable_name('sirius_ledger_default_policy');
  $form[$name] = array(
    '#type' => 'select',
    '#options' => $policy_options,
    '#title' => 'Default Policy',
    '#default_value' => variable_get($name, ''),
    '#description' => t('The default policy will be used globally, but may be overridden by the policy for a specific employer.')
  );

  $name = sirius_domain_variable_name('sirius_ledger_monthly_enabled');
  $form[$name] = array(
    '#type' => 'select',
    '#options' => array(
      'No' => t('No'),
      'Yes' => t('Yes'),
    ),
    '#title' => 'Enable Monthly Charges?',
    '#default_value' => variable_get($name, ''),
  );

  $name = sirius_domain_variable_name('sirius_ledger_hourly_enabled');
  $form[$name] = array(
    '#type' => 'select',
    '#options' => array(
      'No' => t('No'),
      'Yes' => t('Yes'),
    ),
    '#title' => 'Enable Hourly Charges?',
    '#default_value' => variable_get($name, ''),
  );

  return system_settings_form($form);
}

function sirius_ledger_config_policies() {
  $domain_nid = sirius_domain_nid();
  $type = 'sirius_ledger:policy';
  return views_embed_view('sirius_json_definitions', 'block_2', $domain_nid, $type);
}

