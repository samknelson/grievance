<?php

function sirius_okta_page_settings() {
	return drupal_get_form('sirius_okta_page_settings_form');
}

function sirius_okta_page_settings_form($form, &$form_state) {
	$form['sirius_okta_active'] = array(
		'#title' => t('Okta Active?'),
		'#type' => 'select',
		'#options' => array(
			NULL => t('-- Select --'),
			'No' => t('No'),
			'Yes' => t('Yes'),
		),
		'#default_value' => variable_get('sirius_okta_active', ''),
	);

	$form['sirius_okta_autosync_active'] = array(
		'#title' => t('Auto-Sync Active?'),
		'#type' => 'select',
		'#options' => array(
			NULL => t('-- Select --'),
			'No' => t('No'),
			'Yes' => t('Yes'),
		),
		'#default_value' => variable_get('sirius_okta_autosync_active', ''),
		'#description' => t('If "Yes", than any user with the appropriate role will be automatically sync\'d to Okta whenever the user record is saved.'),
	);

	$form['sirius_okta_role'] = array(
		'#type' => 'select',
		'#title' => 'Okta Role',
		'#default_value' => variable_get('sirius_okta_role', ''),
		'#options' => array(NULL => t('-- Select --')) + user_roles(TRUE),
		'#description' => t('Users with this role may be sync\'d to Okta.'),
	);

	$form['sirius_okta_domain'] = array(
		'#type' => 'textfield',
		'#title' => 'Okta Domain',
		'#default_value' => variable_get('sirius_okta_domain', ''),
		'#description' => t('Usually "yourdomain.okta.com"'),
	);

	$form['sirius_okta_domain_admin'] = array(
		'#type' => 'textfield',
		'#title' => 'Okta Admin Domain',
		'#default_value' => variable_get('sirius_okta_domain_admin', ''),
		'#description' => t('Usually "yourdomain-admin.okta.com"'),
	);

	$form['sirius_okta_token'] = array(
		'#type' => 'textfield',
		'#title' => 'Okta Token',
		'#default_value' => variable_get('sirius_okta_token', ''),
	);

	$roles =  user_roles(TRUE);

	$form['rolepush'] = array(
		'#type' => 'fieldset',
		'#title' => 'Role Push',
		'#description' => '<p>' .
			'For the fields below:<ul>' . 
			'<li><strong>Drupal Role:</strong> A Drupal role such as "Employer" or "Member"</li>' .
			'<li><strong>User Type:</strong> An Okta user type ID (e.g. "00askjahdfad8")</li>' .
			'<li><strong>Group:</strong> An Okta group type ID (e.g. "00askjahdfad8")</li>' .
			'</ul>' .
			'Note that these values are set <em>only on user creation</em>. When the user is updated, the type and group <em>are not updated</em>.' .
			'</p>',
	);
	for ($i=0; $i<5; ++$i) {
		$form['rolepush']["sirius_okta_rolepush_$i"] = array(
			'#type' => 'fieldset',
			// '#title' => t('Role push #') . ($i+1),
		);

		$form['rolepush']["sirius_okta_rolepush_$i"]["sirius_okta_rolepush_${i}_from"] = array(
	    '#type' => 'select',
	    '#title' => t('When a user is in this Drupal role ...'),
	    '#options' => array(NULL => t('-- Select --')) + $roles,
	    '#default_value' => variable_get("sirius_okta_rolepush_${i}_from", ""),
		);

		$form['rolepush']["sirius_okta_rolepush_$i"]["sirius_okta_rolepush_${i}_to_type"] = array(
	    '#type' => 'textfield',
	    '#title' => t('... Give them this user type'),
	    '#default_value' => variable_get("sirius_okta_rolepush_${i}_to_type", ""),
		);

		$form['rolepush']["sirius_okta_rolepush_$i"]["sirius_okta_rolepush_${i}_to_group"] = array(
	    '#type' => 'textfield',
	    '#title' => t('... And put them in this group'),
	    '#default_value' => variable_get("sirius_okta_rolepush_${i}_to_group", ""),
		);
	}

	return system_settings_form($form);
}