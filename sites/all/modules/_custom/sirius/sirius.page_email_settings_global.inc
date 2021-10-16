<?php

function sirius_email_settings_global_form() {
	$form['smtp_server'] = array(
		'#type' => 'fieldset',
		'#title' => 'SMTP Server',
		'#description' => 'Enter the information for connecting to your SMTP server. If this section is left blank, the system-wide SMTP settings will be used.',
	);
	
	$name = 'smtp_username';
	$form['smtp_server'][$name] = array(
		'#type' => 'textfield',
		'#title' => t('SMTP Username'),
		'#default_value' => variable_get($name, ''),
	);

	$name = 'smtp_password';
	$form['smtp_server'][$name] = array(
		'#type' => 'textfield',
		'#title' => t('SMTP Password'),
		'#default_value' => variable_get($name, ''),
	);

	$name = 'smtp_host';
	$form['smtp_server'][$name] = array(
		'#type' => 'textfield',
		'#title' => t('SMTP Host'),
		'#default_value' => variable_get($name, ''),
	);

	$name = 'smtp_hostbackup';
	$form['smtp_server'][$name] = array(
		'#type' => 'textfield',
		'#title' => t('SMTP Backup Host'),
		'#default_value' => variable_get($name, ''),
	);

	$name = 'smtp_port';
	$form['smtp_server'][$name] = array(
		'#type' => 'textfield',
		'#title' => t('SMTP Port'),
		'#default_value' => variable_get($name, ''),
	);

	$name = 'smtp_from';
	$form['smtp_server'][$name] = array(
		'#type' => 'textfield',
		'#title' => t('SMTP From Address'),
		'#default_value' => variable_get($name, ''),
	);

	$name = 'smtp_fromname';
	$form['smtp_server'][$name] = array(
		'#type' => 'textfield',
		'#title' => t('SMTP From Name'),
		'#default_value' => variable_get($name, ''),
	);

  $form["sirius_ses_configuration_set"] = array(
    '#type' => 'textfield',
    '#title' => 'Amazon SES Configuration Set Name',
    '#default_value' => variable_get("sirius_ses_configuration_set", ''),
  );

  $form["sirius_email_global_disable"] = array(
    '#type' => 'select',
    '#title' => t('Global Disable'),
    '#options' => array(
      'No' => t('No'),
      'Yes' => t('Yes'),
    ),
    '#description' => t('Select "Yes" to globally disable email on this server.'),
    '#default_value' => variable_get("sirius_email_global_disable", ''),
  );

  $form["sirius_email_global_disable_whitelist"] = array(
    '#type' => 'textarea',
    '#title' => t('Whitelist'),
    '#description' => t('When global disable is on, use the whitelist to indicate which email address <strong>can</strong> receive emails. Enter patters one per line. Regular expressions are permitted, such as ".*@domain.com"'),
    '#default_value' => variable_get("sirius_email_global_disable_whitelist", ''),
    '#states' => array(
      'visible' => array(
        array(':input[name="sirius_email_global_disable"]' => array('value' => 'Yes')),
      )
    ),
  );

	$form["sirius_email_replyto_address"] = array(
		'#type' => 'textfield',
		'#title' => 'Global Reply-To Email',
		'#default_value' => variable_get("sirius_email_replyto_address", ''),
    '#description' => t('If set, ALL emails sent from the system will use this reply-to addres. If left blank, the reply-to address will be the name of the current user.')
	);

	$form["sirius_email_replyto_name"] = array(
		'#type' => 'textfield',
		'#title' => 'Global Reply-To Name',
		'#default_value' => variable_get("sirius_email_replyto_name", ''),
	);

  $form["sirius_ses_configuration_set"] = array(
    '#type' => 'textfield',
    '#title' => 'Amazon SES Configuration Set Name',
    '#default_value' => variable_get("sirius_ses_configuration_set", ''),
  );

  $form['#submit'][] = 'sirius_email_settings_global_submit';

  return system_settings_form($form);
}


function sirius_email_settings_global_form_validate($form, &$form_state) {
	$name = 'smtp_from';
	if ($form_state['values'][$name]) {
		if (!sirius_email_validate($form_state['values'][$name])) {
			form_set_error($name, 'Please enter a valid email address.');
		}
	}
}

function sirius_email_settings_global_submit($form, &$form_state) {
  foreach (array('site_mail', 'mimemail_mail') as $variable) {
    variable_set($variable, $form_state['values']['smtp_from']);
  }
}
