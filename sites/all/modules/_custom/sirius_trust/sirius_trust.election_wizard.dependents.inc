<?php

function sirius_trust_election_wizard_dependents_form($form, &$form_state, $info, $data, $wizard_node, $current_step) {
	$policy_node = sirius_trust_election_wizard_get_policy($data);
	$policy_json = sirius_json_get($policy_node);

	$benefits = sirius_trust_policy_benefits_offered($policy_node);

	$worker_node = sirius_trust_election_wizard_get_worker($wizard_node);
	$worker_contact_nid = $worker_node->field_sirius_contact['und'][0]['target_id'];
	if (!$worker_contact_nid) { return sirius_form_error("No contact record found for this worker."); }

	$relationship_type_lookup = sirius_taxonomy_load('sirius_contact_relationship_types', 'tid', 'name');
	$election_type = sirius_trust_election_wizard_get_election_type($data);
	$election_type_json = sirius_json_get($election_type);
	$relationship_criteria = sirius_trust_relationship_criteria_get($election_type_json);
	$ok_relationship_types = array();
	foreach ($relationship_criteria as $uuid => $criterion) {
		$ok_relationship_types = array_merge($ok_relationship_types, array_filter(array_values($criterion['relationship_types'])));
	}
	$relationship_type_options = array();
	foreach ($ok_relationship_types as $relationship_type_tid) {
		$relationship_type_options[$relationship_type_tid] = sirius_term_title($relationship_type_tid);
	}

	$form['intro'] = array('#markup' => sirius_trust_relationship_criteria_render($relationship_criteria, array('title' => 'Please select the following dependents')));

  $params = array();
  $params['active'] = 'Yes';
	$params['contact_nid'] = $worker_contact_nid;
	$relationship_nodes = sirius_contact_relationships($params);
	$relationship_options = array();
	foreach ($relationship_nodes as $relationship_nid => $relationship_node) {
		if (!in_array($relationship_node->field_sirius_contact_reltype['und'][0]['tid'], $ok_relationship_types)) { continue; }

		$dependent_contact_nid = $relationship_node->field_sirius_contact_alt['und'][0]['target_id'];
		$dependent_contact_node = node_load($dependent_contact_nid);

		$relationship_options[$relationship_nid] = '(' . 
			$relationship_type_lookup[$relationship_node->field_sirius_contact_reltype['und'][0]['tid']] . 
			' to) ' . 
			$dependent_contact_node->title;
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
