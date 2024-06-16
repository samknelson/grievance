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

  $name = sirius_domain_variable_name('sirius_ledger_monthly_batch_size');
  $form[$name] = array(
    '#type' => 'textfield',
    '#title' => 'Batch size for monthly charges',
    '#default_value' => variable_get($name, 1000),
    '#size' => 4,
    '#description' => 'Each time the monthly dues are processed (up to once an hour), process at most this many entries. Selecting a value that is too large could result in degraded server performance.',
  );

  return system_settings_form($form);
}
