<?php

function sirius_hour_config($form, &$form_state) {
  $name = sirius_domain_variable_name('sirius_hour_enable_simple_monthly');
  $form[$name] = array(
    '#type' => 'select',
    '#options' => array(
    	NULL => t('-- Select --'),
    	'No' => t('No'),
    	'Yes' => t('Yes'),
    ),
    '#title' => t('Enable Simple Monthly Hours'),
    '#default_value' => variable_get($name, ''),
    '#description' => t('Simple monthly hours allows a single form for each employer recording the hours for the month. For more complicated frameworks (i.e. with many workers per employer or with many entries per worker) simple monthly hours should not be enabled.'),
  );

  return system_settings_form($form);
}
