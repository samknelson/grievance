<?php

function sirius_denorm_admin() {
  $form = array();

  /*
	$name = sirius_domain_variable_name('sirius_denorm_whatever');
  $form[$name] = array(
    '#title' => t('Enabled Plugins'),
    '#type' => 'checkboxes',
    '#options' => $options,
    '#default_value' => variable_get($name, array()),
  );
  */

  return system_settings_form($form);
}
