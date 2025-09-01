<?php

function sirius_trust_page_cobra_config() {
  return drupal_get_form('sirius_trust_page_cobra_config_form');
}

function sirius_trust_page_cobra_config_form($form, &$form_state) {
	$employer_options = sirius_employer_list();
  $variable_name = sirius_domain_variable_name('sirius_trust_cobra_employer_nid');
  $form[$variable_name] = array(
    '#type' => 'select',
    '#title' => t('Employer'),
    '#default_value' => variable_get($variable_name, NULL),
    '#options' => array(NULL => t('-- Select --')) + $employer_options,
    '#description' => t('Select the employer that will be used for COBRA designations.'),
  );

  $benefit_type_options = sirius_taxonomy_load('sirius_trust_benefit_type', 'tid', 'name');
  $variable_name = sirius_domain_variable_name('sirius_trust_cobra_benefit_type');
  $form[$variable_name] = array(
    '#type' => 'select',
    '#title' => t('Benefit Type'),
    '#default_value' => variable_get($variable_name, NULL),
    '#options' => array(NULL => t('-- Select --')) + $benefit_type_options,
    '#description' => t('Select the benefit type that will be used to determine COBRA eligibility. This will generall be "Medical".'),
  );

  $hour_type_options = sirius_taxonomy_load('sirius_hour_type', 'tid', 'name');
  $variable_name = sirius_domain_variable_name('sirius_trust_cobra_hour_type');
  $form[$variable_name] = array(
    '#type' => 'select',
    '#title' => t('Hour Type'),
    '#default_value' => variable_get($variable_name, NULL),
    '#options' => array(NULL => t('-- Select --')) + $hour_type_options,
    '#description' => t('Select the hour type that will be used to record COBRA statuc.'),
  );

  return system_settings_form($form);
}
