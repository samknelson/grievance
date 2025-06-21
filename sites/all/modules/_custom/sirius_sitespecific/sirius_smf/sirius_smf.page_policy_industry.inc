<?php

function sirius_smf_node_tab_policy_industry_access($policy_node) {
	if ($policy_node->field_sirius_type['und'][0]['value'] != 'sirius_ledger:policy') { return FALSE; }
	return TRUE;
}

function sirius_smf_node_tab_policy_industry_form($form, &$form_state, $policy_node) {
	$policy_json = sirius_json_get($policy_node);

	$form_state['policy_nid'] = $policy_node->nid;
	$form['#tree'] = TRUE;

	$form['industry'] = array(
		'#type' => 'select',
		'#title' => t('Industry'),
		'#options' => array(NULL => t('-- Select --')) + sirius_taxonomy_load('sirius_industry', 'tid', 'name'),
		'#default_value' => $policy_json['smf']['industry'],
	);


	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_smf_node_tab_policy_industry_form_submit($form, &$form_state) {
	$policy_node = node_load($form_state['policy_nid']);
	$policy_json = sirius_json_get($policy_node);
	$policy_json['smf']['industry'] = $form_state['values']['industry'];
	sirius_json_set($policy_node, $policy_json);
	node_save($policy_node);
	drupal_set_message("Inudstry saved.");
}
