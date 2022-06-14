<?php

function sirius_trust_beneficiary_form($form, &$form_state, $worker_node, $benefit_node) {
	$benefit_json = sirius_json_get($benefit_node);
	$beneficiary_settings_json = $benefit_json['beneficiary_settings'];
	if ($beneficiary_settings_json['active'] != 'Yes') { return array(); }

	$reltype_tids = array_filter(array_values($beneficiary_settings_json['relationship_types']));
	if (!$reltype_tids) { 
		return sirius_form_error('There is a configuration error. This benefit is configured to allow beneficiaries to be designated, but no relationship types are supported.');
	}

	$relationship_options = sirius_trust_relationship_options($worker_node, array('reltype_tids' => $reltype_tids, 'active' => 'Yes'));
	if (!$relationship_options) {
		return sirius_form_error('No matching relationships available.');
	}

	$beneficiary_json = sirius_trust_beneficiary_get($worker_node, $benefit_node);

	$form['primary'] = array(
		'#type' => 'checkboxes',
		'#title' => 'Primary Beneficiary',
		'#options' => $relationship_options,
		'#default_value' => sirius_form_array_default_value($beneficiary_json['primary']),
	);

	$form['whatever'] = array(
		'#type' => 'textfield',
		'#title' => 'Whatever',
		'#default_value' => $beneficiary_json['whatever'],
	);

	return $form;
}

function sirius_trust_beneficiary_get($worker_node, $benefit_node) {
	$worker_json = sirius_json_get($worker_node);
	return $worker_json['beneficiaries'][$benefit_node->nid]['beneficiary'];
}

function sirius_trust_beneficiary_set($worker_node, $benefit_node, $new_beneficiary_json) {
	$old_beneficiary_json = sirius_trust_beneficiary_get($worker_node, $benefit_node);
	$changed = sirius_revision_changed_impl($old_beneficiary_json, $new_beneficiary_json, 10);

	if (!$changed) {
		return array('success' => TRUE, 'msg' => $benefit_node->title . ': Beneficiary is unchanged.');
	}

	$worker_json = sirius_json_get($worker_node);
	$worker_json['beneficiaries'][$benefit_node->nid]['beneficiary'] = $new_beneficiary_json;
	sirius_json_set($worker_node, $worker_json);
	node_save($worker_node);

	return array(
		'success' => TRUE, 
		'msg' => $benefit_node->title . ': Beneficiary updated.',
		'old_beneficiary' => $old_beneficiary_json,
		'new_beneficiary' => $new_beneficiary_json
	);
}
