<?php

function sirius_smf_term_tab_industry_threshold_form($form, &$form_state, $industry_term) {
	$form_state['industry_tid'] = $industry_term->tid;

	$industry_json = sirius_json_get($industry_term);

	$form['#tree'] = TRUE;

	$form['smf']['threshold']['hrs'] = array(
		'#type' => 'textfield',
		'#size' => 4,
		'#title' => t('Hours Threshold'),
	);

	sirius_jsonfield_util_set_defaults($form['smf']['threshold'], $industry_json['smf']['threshold']);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_smf_term_tab_industry_threshold_form_submit($form, &$form_state) {
	$industry_term = taxonomy_term_load($form_state['industry_tid']);

	$industry_json = sirius_json_get($industry_term);
	$industry_json['smf']['threshold'] = $form_state['values']['smf']['threshold'];
	sirius_json_set($industry_term, $industry_json);
	taxonomy_term_save($industry_term);
	drupal_set_message("Settings saved.");
}