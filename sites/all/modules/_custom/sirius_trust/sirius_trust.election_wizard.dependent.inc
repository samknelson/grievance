<?php

function sirius_trust_election_wizard_dependent_new_form($form, &$form_state, $info, $data, $wizard_node, $current_step) {
	$result = sirius_trust_election_wizard_get_policy($data);
	if (!$result['success']) { return sirius_form_error($result['msg']); }
	$policy_rnode = $result['data'];
	$policy_json = sirius_json_get($policy_rnode);

	$which = preg_replace('/dependent_/', '', $current_step);
	if (!($which > 0)) { return sirius_form_error("Don't know which dependent."); }

	$result = sirius_proxy_call('sirius_taxonomy_load', 'sirius_contact_relationship_types', 'tid', 'name');
	if (!$result['success']) { return sirius_form_error($result['msg']); }
	$relationship_type_lookup = $result['data'];

	$form['prefix'] = array('#markup' => $policy_json['elections']['dependents']['new']['prefix']);

	$form['dependents']['new'][$which]['reltype'] = array(
		'#title' => t('Your Relationship to the Dependent'),
		'#type' => 'select',
		'#options' => array(NULL => t('-- Select --')) + $relationship_type_lookup,
		'#default_value' => $data['dependents']['new'][$which]['reltype'],
		'#required' => TRUE,
	);

	$form['dependents']['new'][$which]['name_first'] = array(
		'#title' => t('First Name'),
		'#type' => 'textfield',
		'#default_value' => $data['dependents']['new'][$which]['name_first'],
		'#required' => TRUE,
	);

	$form['dependents']['new'][$which]['name_last'] = array(
		'#title' => t('Last Name'),
		'#type' => 'textfield',
		'#default_value' => $data['dependents']['new'][$which]['name_last'],
		'#required' => TRUE,
	);

	$form['dependents']['new'][$which]['ssn'] = array(
		'#title' => t('SSN'),
		'#type' => 'textfield',
		'#default_value' => $data['dependents']['new'][$which]['ssn'],
		'#required' => TRUE,
		'#size' => 11,
	);

	$form['dependents']['new'][$which]['phone'] = array(
		'#title' => t('Phone'),
		'#type' => 'textfield',
		'#default_value' => $data['dependents']['new'][$which]['phone'],
		'#size' => 11,
	);

	$form['suffix'] = array('#markup' => $policy_json['elections']['dependents']['new']['suffix']);

	return sirius_trust_election_wizard_form_prep($form, $form_state, $info, $data, $wizard_node, $current_step);
}

function sirius_trust_election_wizard_dependent_new_form_validate($form, &$form_state) {
	$info = $form_state['info'];
	$data = $form_state['data'];
	$current_step = $form_state['current_step'];
	$wizard_node = $form_state['wizard_node'];
	$which = preg_replace('/dependent_/', '', $current_step);

	if (!sirius_ssn_validate($form_state['values']['dependents']['new'][$which]['ssn'])) {
		form_set_error("dependents][new][$which][ssn", 'Please enter a valid SSN.');
	}

	if ($form_state['values']['dependents']['new'][$which]['phone'] && !sirius_phone_validate($form_state['values']['dependents']['new'][$which]['phone'])) {
		form_set_error("dependents][new][$which][phone", 'Please enter a valid phone number.');
	}
}


function sirius_trust_election_wizard_dependent_new_form_submit($form, &$form_state) {
	$info = $form_state['info'];
	$data = $form_state['data'];
	$current_step = $form_state['current_step'];
	$wizard_node = $form_state['wizard_node'];
	$which = preg_replace('/dependent_/', '', $current_step);

	$data['dependents']['new'][$which] = $form_state['values']['dependents']['new'][$which];

	sirius_trust_election_set_title($wizard_node);
	sirius_wizard_submit($info, $data, $wizard_node, $current_step, "Response saved.", array('nonsequential' => TRUE));
}
