<?php

function sirius_btu_page_csg_settings() {
	return drupal_get_form('sirius_btu_page_csg_settings_form');
}

function sirius_btu_page_csg_settings_form($form, &$form_state) {
  $name = sirius_domain_variable_name('sirius_btu_csg_notify_emails');
  $form[$name] = array(
  	'#title' => t('Notification Email Addresses'),
    '#type' => 'textarea',
    '#default_value' => variable_get($name, ''),
    '#description' => t('Enter the email addresses which should be notified when new class size grievances are submitted. One email address per line.')
  );

	return system_settings_form($form);
}
