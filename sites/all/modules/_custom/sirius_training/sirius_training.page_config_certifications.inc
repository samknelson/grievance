<?php

function sirius_training_config_certifications($form, &$form_state) {
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

  return system_settings_form($form);
}

function sirius_training_config_certification_definitions() {
  $domain_nid = sirius_domain_nid();
  $type = 'sirius_training:certification';
  return views_embed_view('sirius_json_definitions', 'block_1', $domain_nid, $type);
}

