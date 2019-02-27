<?php

function sirius_worker_config() {
  $form = array();

  $form['sirius_worker_dummy_domain'] = array(
    '#type' => 'textfield',
    '#title' => t('Dummy Domain'),
    '#default_value' => variable_get('sirius_worker_dummy_domain', ''),
    '#description' => t('Workers without an email address will be assigned an address at this domain.'),
  );

	$role_options = user_roles(TRUE);
  $form['sirius_worker_roles'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Worker Roles'),
    '#options' => $role_options,
    '#default_value' => variable_get('sirius_worker_roles', array()),
    '#description' => t('The checked roles are used for workers. Workers are assigned to, or removed from, roles within this list, based on the settings in their member status.'),
  );

  return system_settings_form($form);
}


