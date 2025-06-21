<?php

function sirius_smf_term_tab_ms_threshold_form($form, &$form_state, $ms_term) {
	$ms_json = sirius_json_get($ms_term);

	$form_state['ms_tid'] = $ms_term->tid;
	$form['#tree'] = TRUE;

	$form['threshold'] = array(
		'#type' => 'textfield',
		'#size' => 4,
		'#title' => t('Hours Threshold'),
		'#default_value' => $ms_json['smf']['threshold'],
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_smf_term_tab_ms_threshold_form_submit($form, &$form_state) {
	$ms_term = taxonomy_term_load($form_state['ms_tid']);
	$ms_json = sirius_json_get($ms_term);
	$ms_json['smf']['threshold'] = $form_state['values']['threshold'];
	sirius_json_set($ms_term, $ms_json);
	taxonomy_term_save($ms_term);
	drupal_set_message("Threshold saved.");
}
