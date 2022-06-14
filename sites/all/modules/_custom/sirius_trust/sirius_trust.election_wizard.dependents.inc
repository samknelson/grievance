<?php

function sirius_trust_election_wizard_dependents_form($form, &$form_state, $info, $data, $wizard_node, $current_step) {
	$result = sirius_trust_election_wizard_get_policy($data);
	if (!$result['success']) { return sirius_form_error($result['msg']); }
	$policy_rnode = $result['data'];
	$policy_json = sirius_json_get($policy_rnode);

	$result = sirius_proxy_call('sirius_trust_policy_benefits_offered', $policy_rnode);
	if (!$result['success']) { return sirius_form_error($result['msg']); }
	$benefits = $result['data'];

	$result = sirius_trust_election_wizard_get_worker($wizard_node);
	if (!$result['success']) { return sirius_form_error($result['msg']); }
	$worker_rnode = $result['data'];
	$worker_contact_rnid = $worker_rnode->field_sirius_contact['und'][0]['target_id'];
	if (!$worker_contact_rnid) { return sirius_form_error("No contact record found for this worker."); }

	$result = sirius_proxy_call('sirius_taxonomy_load', 'sirius_contact_relationship_types', 'tid', 'name');
	if (!$result['success']) { return sirius_form_error($result['msg']); }
	$relationship_type_lookup = $result['data'];

  $params = array();
  $params['active'] = 'Yes';
	$params['contact_nid'] = $worker_contact_rnid;
	$result = sirius_proxy_call('sirius_contact_relationships', $params);
	if (!$result['success']) { return sirius_form_error($result['msg']); }
	$relationship_rnodes = $result['data'];
	$relationship_options = array();
	foreach ($relationship_rnodes as $relationship_rnid => $relationship_rnode) {
		$dependent_contact_rnid = $relationship_rnode->field_sirius_contact_alt['und'][0]['target_id'];
		$result = sirius_proxy_call('sirius_proxy_node_load', $dependent_contact_rnid);
		if (!$result['success']) { return sirius_form_error($result['msg']); }
		$dependent_contact_rnode = $result['data'];

		$relationship_options[$relationship_rnid] = '(' . 
			$relationship_type_lookup[$relationship_rnode->field_sirius_contact_reltype['und'][0]['tid']] . 
			' to) ' . 
			$dependent_contact_rnode->title;
	}

	$form['prefix'] = array('#markup' => $policy_json['elections']['dependents']['existing']['prefix']);

	if ($relationship_options) {
		$form['dependents']['general']['intro1'] = array('#markup' => '<p>' . t('Select any existing dependents which you would like to include.') . '</p>');

		$form['dependents']['general']['current'] = array(
			'#title' => t('Current Dependents'),
			'#type' => 'checkboxes',
			'#default_value' => sirius_form_array_default_value($data['dependents']['general']['current']),
			'#options' => $relationship_options,
		);
	}

	$form['dependents']['general']['intro2'] = array('#markup' => '<p>' . t('Enter the number of additional dependents that you would like to add.') . '</p>');

	$form['dependents']['general']['count_new'] = array(
		'#title' => t('Additional Dependents'),
		'#type' => 'select',
		'#default_value' => $data['dependents']['general']['count_new'],
		'#required' => TRUE,
		'#options' => array(
			0 => 0,
			1 => 1,
			2 => 2,
			3 => 3,
			4 => 4,
			5 => 5,
			6 => 6,
		),
	);

	$form['suffix'] = array('#markup' => $policy_json['elections']['dependents']['existing']['suffix']);

	return sirius_trust_election_wizard_form_prep($form, $form_state, $info, $data, $wizard_node, $current_step);
}

function sirius_trust_election_wizard_dependents_form_submit($form, &$form_state) {
	$info = $form_state['info'];
	$data = $form_state['data'];
	$current_step = $form_state['current_step'];
	$wizard_node = $form_state['wizard_node'];
	$data['dependents']['general'] = $form_state['values']['dependents']['general'];
	sirius_trust_election_set_title($wizard_node);
	sirius_wizard_submit($info, $data, $wizard_node, $current_step, "Response saved.", array('nonsequential' => TRUE));
}
