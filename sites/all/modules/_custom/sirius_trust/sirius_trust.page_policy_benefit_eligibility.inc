<?php

function sirius_trust_node_tab_policy_benefit_eligibility_access($policy_node, $benefit_node) {
	// Must be one of the offered benefits
	$offered_benefits = sirius_trust_policy_benefits_offered($policy_node);
	if (!$offered_benefits[$benefit_node->nid]) { return FALSE; }

	return TRUE;
}

function sirius_trust_node_tab_policy_benefit_eligibility_form($form, &$form_state, $policy_node, $benefit_node) {
	$form_state['policy_nid'] = $policy_node->nid;
	$form_state['benefit_nid'] = $benefit_node->nid;

	$policy_json = sirius_json_get($policy_node);

	$plugin_info_all = sirius_trust_eligibility_plugin_info_all();
	$plugin_options = array();
	foreach ($plugin_info_all as $plugin_key => $plugin_info) {
		$plugin_options[$plugin_key] = $plugin_info['name'];
	}

	$form['#tree'] = TRUE;

	$form['benefits'][$benefit_node->nid]['eligibility']['plugins']['enabled'] = array(
		'#title' => 'Enabled Eligibility Plugins',
		'#type' => 'checkboxes',
		'#options' => $plugin_options,
		'#default_value' => sirius_form_array_default_value($policy_json['benefits'][$benefit_node->nid]['eligibility']['plugins']['enabled']),
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_trust_node_tab_policy_benefit_eligibility_form_submit($form, &$form_state) {
	$policy_node = node_load($form_state['policy_nid']);
	$benefit_node = node_load($form_state['benefit_nid']);
	$policy_json = sirius_json_get($policy_node);
	$policy_json['benefits'][$benefit_node->nid]['eligibility']['plugins']['enabled'] = $form_state['values']['benefits'][$benefit_node->nid]['eligibility']['plugins']['enabled'];
	sirius_json_set($policy_node, $policy_json);
	node_save($policy_node);
	drupal_set_message('Settings saved.');
}