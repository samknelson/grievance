<?php

function sirius_smf_page_employer_kaiser_settings_form($form, &$form_state, $employer_node) {
	$form_state['employer_nid'] = $employer_node->nid;
  $form['#tree'] = TRUE;

	$employer_json = sirius_json_get($employer_node);

	$form['kaiser']['immediate_start_date'] = array(
		'#title' => t('Immediate Eligibility Start Date'),
		'#type' => 'textfield',
	);

	$form['kaiser']['immediate_end_date'] = array(
		'#title' => t('Immediate Eligibility End Date'),
		'#type' => 'textfield',
	);

	sirius_jsonfield_util_set_defaults($form['kaiser'], $employer_json['smf']['kaiser']);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_smf_page_employer_kaiser_settings_form_submit($form, &$form_state) {
	$employer_node = node_load($form_state['employer_nid']);
	$employer_json = sirius_json_get($employer_node);
	$employer_json['smf']['kaiser'] = $form_state['values']['kaiser'];
	sirius_json_set($employer_node, $employer_json);
	node_save($employer_node);
	drupal_set_message("Settings saved.");
}