<?php

function sirius_trust_node_tab_policy_benefit_eligibility_plugin_access($policy_node, $benefit_node, $plugin_key) {
  $plugin_info = sirius_trust_eligibility_plugin_info($plugin_key);
	if (!$plugin_info) { return FALSE; }

  $enabled_plugins = sirius_trust_eligibility_plugins_enabled($policy_node, $benefit_node);
  if (!in_array($plugin_key, $enabled_plugins)) { return FALSE; }

	return TRUE;
}

function sirius_trust_node_tab_policy_benefit_eligibility_plugin_form($form, &$form_state, $policy_node, $benefit_node, $plugin_key) {
	$form_state['policy_nid'] = $policy_node->nid;
	$form_state['benefit_nid'] = $benefit_node->nid;
	$form_state['plugin_key'] = $plugin_key;

	$policy_json = sirius_json_get($policy_node);

  $plugin_info = sirius_trust_eligibility_plugin_info($plugin_key);
	sirius_trust_eligibility_plugin_load($plugin_key);

	$form = array();
	$form['#tree'] = TRUE;

	if ($plugin_info['description']) {
	  $form['intro'] = array(
	    '#markup' => '<div class="sirius_trust_eligibility_plugin_description">' . $plugin_info['description'] . '</div>',
	  );
	}

	$subform = $plugin_info['instance']->form($policy_json['benefits'][$benefit_node->nid]['eligibility']['plugin'][$plugin_key]['settings']);

	$subform['scan_types'] = array(
		'#type' => 'checkboxes',
		'#title' => t('Scan Types'),
		'#options' => array(
			'benefit_start' => t('Require for benefit to start'),
			'benefit_continue' => t('Require for benefit to continue'),
		),
		'#description' => 'Select when this eligibility plugin must be met<ul><li>"Benefit Start" is for requirements that must be met in order for the benefit to be elected, such as a geographical or a relationship requirement.</li><li>"Benefit Continue" is checked each month and a benefit is discontinued if the requirement is not met; it is appropriate for requirements such as an hours threshold or an election requirement.</ul>',
		'#default_value' => sirius_form_array_default_value($policy_json['benefits'][$benefit_node->nid]['eligibility']['plugin'][$plugin_key]['settings']['scan_types']),
	);

	if (!$subform) {
		$form['nosettings'] = array('#markup' => '<p><em>' . t('There are no settings for this plugin') . '</em></p>');
		return $form;
	}

  $form['benefits'][$benefit_node->nid]['eligibility']['plugin'][$plugin_key]['settings'] = $subform;

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_trust_node_tab_policy_benefit_eligibility_plugin_form_submit($form, &$form_state) {
	$policy_node = node_load($form_state['policy_nid']);
	$benefit_node = node_load($form_state['benefit_nid']);
	$plugin_key = $form_state['plugin_key'];

  $plugin_info = sirius_trust_eligibility_plugin_info($plugin_key);
	sirius_trust_eligibility_plugin_load($plugin_key);
	$plugin_info['instance']->form_submit($form_state['values']['benefits'][$benefit_node->nid]['eligibility']['plugin'][$plugin_key]['settings']);

	$policy_json = sirius_json_get($policy_node);
	$policy_json['benefits'][$benefit_node->nid]['eligibility']['plugin'][$plugin_key]['settings'] = 
		$form_state['values']['benefits'][$benefit_node->nid]['eligibility']['plugin'][$plugin_key]['settings'];

	sirius_json_set($policy_node, $policy_json);
	node_save($policy_node);
	drupal_set_message('Settings saved.');
}