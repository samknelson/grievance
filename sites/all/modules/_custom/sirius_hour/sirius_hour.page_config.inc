<?php

function sirius_hour_config($form, &$form_state) {
  $hour_types = sirius_taxonomy_load('sirius_hour_type', 'tid', 'name', TRUE);
  $departments = sirius_taxonomy_load('grievance_department', 'tid', 'name', TRUE);

  $name = sirius_domain_variable_name('sirius_hour_default_type');
  $form[$name] = array(
    '#type' => 'select',
    '#title' => t('Default Hour Type'),
    '#options' => $hour_types,
    '#default_value' => variable_get($name, ''),
  );

  $name = sirius_domain_variable_name('sirius_hour_default_department');
  $form[$name] = array(
    '#type' => 'select',
    '#title' => t('Default Deparment'),
    '#options' => $departments,
    '#default_value' => variable_get($name, ''),
  );

  return system_settings_form($form);
}
