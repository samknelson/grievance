<?php

function sirius_page_sms_optin_config() {
	return drupal_get_form('sirius_page_sms_optin_config_form');
}

function sirius_page_sms_optin_config_form($form, &$form_state) {
	$form['#tree'] = TRUE;

	$name = sirius_domain_variable_name('sirius_sms_optin_disclaimer_nid');
  $form[$name] = array(
		'#title' => t('SMS Opt-in Disclaimer'),
		'#type' => 'select',
		'#options' => array(NULL => t('-- Select --')) + sirius_disclaimer_list(),
		'#default_value' => variable_get($name),
	);

  return system_settings_form($form);
}


