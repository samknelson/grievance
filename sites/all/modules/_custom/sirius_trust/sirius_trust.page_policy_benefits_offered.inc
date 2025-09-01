<?php

function sirius_trust_node_tab_policy_benefits_offered_form($form, &$form_state, $policy_node) {
	$benefit_options = sirius_trust_benefit_options(FALSE);
	if (!$benefit_options) {
		return array('error' => array('#markup' => "No benefits have been defined. Please define at least one benefit on the " .
			l('benefit administration page.', 'sirius/trust/benefits')));
	}

	$policy_json = sirius_json_get($policy_node);
	$form_state['policy_nid'] = $policy_node->nid;

	$form['#tree'] = TRUE;

	$form['benefits']['enabled'] = array(
		'#title' => 'Benefits available under this policy',
		'#type' => 'checkboxes',
		'#checkall' => TRUE,
		'#options' => $benefit_options,
	);
	if ($policy_json['benefits']['enabled']) { $form['benefits']['enabled']['#default_value'] = $policy_json['benefits']['enabled']; }

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_trust_node_tab_policy_benefits_offered_form_submit($form, &$form_state) {
	$policy_node = node_load($form_state['policy_nid']);
	$policy_json = sirius_json_get($policy_node);
	$policy_json['benefits']['enabled'] = $form_state['values']['benefits']['enabled'];
	sirius_json_set($policy_node, $policy_json);
	node_save($policy_node);
	drupal_set_message("Benefits saved.");
}
