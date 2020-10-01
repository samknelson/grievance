<?php

function sirius_domain_page_list_prefs() {
	return drupal_get_form('sirius_domain_page_list_prefs_form');
}

function sirius_domain_page_list_prefs_form($form, &$form_state) {
	global $user;
	$user_full = user_load($user->uid);
	$json = sirius_jsonfield_util_user_json_get($user_full);

	$form['#tree'] = TRUE;

	$stats_info_all = sirius_domain_stats_info_all();
	$stats_options = array(NULL => t('-- Select --'));
	foreach ($stats_info_all as $key => $info) {
		$stats_options[$key] = $info['title_full'];
	}

	$form['sirius']['domain']['stats'] = array(
		'#type' => 'fieldset',
		'#title' => t('Statistics'),
	);
	$form['sirius']['domain']['stats']['include_subdomains'] = array(
		'#type' => 'select',
		'#title' => 'Include subdomains?',
		'#options' => array(
			NULL => t('-- Select --'),
			'Yes' => t('Yes'),
			'No' => t('No'),
		),
		'#default_value' => $json['sirius']['domain']['stats']['include_subdomains'],
		'#description' => t('When presenting key stats, should the value for a domain include all subdomains, or only the domain itself?'),
	);

	$form['sirius']['domain']['stats']['key'] = array(
		'#type' => 'fieldset',
		'#title' => t('Key Stats'),
		'#description' => t('Select up to 5 "key" stats that will appear on the dashboard.'),
	);
	for ($i=0; $i<5; ++$i) {
		$form['sirius']['domain']['stats']['key'][$i] = array(
			'#type' => 'select',
			'#title' => 'Key Stat #' . ($i+1),
			'#options' => $stats_options,
			'#default_value' => $json['sirius']['domain']['stats']['key'][$i],
		);
	}

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_domain_page_list_prefs_form_submit($form, &$form_state) {
	global $user;
	$user_full = user_load($user->uid);
	$json = sirius_jsonfield_util_user_json_get($user_full);
	$json['sirius']['domain'] = $form_state['values']['sirius']['domain'];
	sirius_jsonfield_util_user_json_set($user_full, $json);
	user_save($user_full);
	drupal_set_message("Preferences saved.");
}