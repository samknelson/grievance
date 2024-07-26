<?php

function sirius_sib_term_tab_settings_form($form, &$form_state, $sib_term) {
	$form_state['sib_tid'] = $sib_term->tid;

	$sib_obj = Sirius_Sib::getInstance();

	$form['#tree'] = TRUE;

	$restrictions = $sib_obj->restriction_info_all();
	$options = array();
	foreach ($restrictions as $key => $info) { $options[$key] = $info['title']; }
	$form['settings']['restriction'] = array(
		'#type' => 'checkboxes',
		'#title' => t('Restrictions'),
		'#options' => $options,
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	$settings = $sib_obj->term_settings($sib_term);
	sirius_jsonfield_util_set_defaults($form['settings'], $settings);

	return $form;
}

function sirius_sib_term_tab_settings_form_submit($form, &$form_state) {
	$sib_term = taxonomy_term_load($form_state['sib_tid']);
	$sib_obj = Sirius_Sib::getInstance();
	$sib_obj->term_settings_set($sib_term, $form_state['values']['settings']);
	drupal_set_message("Settings saved.");
}