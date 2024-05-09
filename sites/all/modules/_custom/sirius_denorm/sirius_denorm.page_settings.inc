<?php

function sirius_denorm_page_settings() {
	return drupal_get_form('sirius_denorm_page_settings_form');
}

function sirius_denorm_page_settings_form($form, &$form_state) {

	$form['sirius_denorm_batch_size'] = array(
		'#title' => t('Batch Size'),
		'#type' => 'textfield',
		'#size' => 4,
		'#default_value' => variable_get('sirius_denorm_batch_size', 1000),
	);

	$form['sirius_denorm_interval'] = array(
		'#title' => t('Interval'),
		'#type' => 'textfield',
		'#size' => 4,
		'#description' => t('Interval between batch runs (in seconds)'),
		'#default_value' => variable_get('sirius_denorm_interval', 60*10),
	);

	$form['sirius_denorm_cleanup_after'] = array(
		'#title' => t('Cleanup After'),
		'#type' => 'textfield',
		'#size' => 4,
		'#default_value' => variable_get('sirius_denorm_cleanup_after', 60*60),
		'#description' => t('Remove completed items from the queue after this many seconds.')
	);

	return system_settings_form($form);
}