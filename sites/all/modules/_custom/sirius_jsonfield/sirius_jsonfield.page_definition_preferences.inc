<?php

function sirius_json_definition_preferences_form($form, &$form_state, $definition_nid) {
	global $user;
	$user_full = user_load($user->uid);
	$user_json = sirius_jsonfield_util_user_json_get($user_full);

	$form['#tree'] = TRUE;

	$form['sirius']['jsonfield']['form-builder']['preferences']['richtext'] = array(
		'#title' => t('Use Rich-Text Editor'),
		'#type' => 'select',
		'#options' => array(
			NULL => t('-- Select --'),
			'Yes' => t('Yes'),
			'No' => t('No'),
		),
		'#description' => t('In the form builder, should the rich-text editor be used? Select "No" if you prefer to edit the form using HTML code. Select "Yes" if you prefer to use the integrated rich-text editor. Note that selecting "Yes" will slow down the load time of the form editor page.'),
		'#default_value' => $user_json['sirius']['jsonfield']['form-builder']['preferences']['richtext'],
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_json_definition_preferences_form_submit($form, &$form_state) {
	global $user;
	$user_full = user_load($user->uid);
	$user_json = sirius_jsonfield_util_user_json_get($user_full);
	$user_json['sirius']['jsonfield']['form-builder']['preferences'] = $form_state['values']['sirius']['jsonfield']['form-builder']['preferences'];
	sirius_jsonfield_util_user_json_set($user_full, $user_json);
	user_save($user_full);
	drupal_set_message(t('Preferences saved.'));
}