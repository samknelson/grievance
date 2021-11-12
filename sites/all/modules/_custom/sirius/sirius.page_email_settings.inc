<?php

function sirius_email_settings_form() {
	$domain_nid = sirius_domain_nid();
	if ($domain_nid < 1) {
		drupal_set_message("You are not currently in a domain. Wouldn't you rather switch into a domain, or use the global settings page?", 'warning');
	}

	$form['smtp_server'] = array(
		'#type' => 'fieldset',
		'#title' => 'SMTP Server',
		'#description' => 'Enter the information for connecting to your SMTP server. If this section is left blank, the system-wide SMTP settings will be used.',
	);

	$name = sirius_domain_variable_name('smtp_username');
	$form['smtp_server'][$name] = array(
		'#type' => 'textfield',
		'#title' => t('SMTP Username'),
		'#default_value' => variable_get($name, ''),
	);

	$name = sirius_domain_variable_name('smtp_password');
	$form['smtp_server'][$name] = array(
		'#type' => 'textfield',
		'#title' => t('SMTP Password'),
		'#default_value' => variable_get($name, ''),
	);

	$name = sirius_domain_variable_name('smtp_host');
	$form['smtp_server'][$name] = array(
		'#type' => 'textfield',
		'#title' => t('SMTP Host'),
		'#default_value' => variable_get($name, ''),
	);

	$name = sirius_domain_variable_name('smtp_hostbackup');
	$form['smtp_server'][$name] = array(
		'#type' => 'textfield',
		'#title' => t('SMTP Backup Host'),
		'#default_value' => variable_get($name, ''),
	);

	$name = sirius_domain_variable_name('smtp_port');
	$form['smtp_server'][$name] = array(
		'#type' => 'textfield',
		'#title' => t('SMTP Port'),
		'#default_value' => variable_get($name, ''),
	);

	$name = sirius_domain_variable_name('smtp_from');
	$form['smtp_server'][$name] = array(
		'#type' => 'textfield',
		'#title' => t('SMTP From Address'),
		'#default_value' => variable_get($name, ''),
	);

	$name = sirius_domain_variable_name('smtp_fromname');
	$form['smtp_server'][$name] = array(
		'#type' => 'textfield',
		'#title' => t('SMTP From Name'),
		'#default_value' => variable_get($name, ''),
	);

  $form['#submit'][] = 'sirius_email_settings_form_submit';

  return system_settings_form($form);
}

function sirius_email_settings_form_validate($form, &$form_state) {
	$name = sirius_domain_variable_name('smtp_from');
	if ($form_state['values'][$name]) {
		if (!sirius_email_validate($form_state['values'][$name])) {
			form_set_error($name, 'Please enter a valid email address.');
		}
	}
}

function sirius_email_settings_form_submit($form, &$form_state) {
}
