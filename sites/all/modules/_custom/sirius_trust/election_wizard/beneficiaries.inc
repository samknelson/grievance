<?php

function sirius_trust_election_wizard_beneficiaries_form($form, &$form_state, $info, $data, $wizard_node, $current_step) {
	$worker_node = sirius_trust_election_wizard_get_worker($wizard_node);
	$policy_node = sirius_trust_election_wizard_get_policy($data);
	$policy_json = sirius_json_get($policy_node);

	$beneficiary = Sirius_Trust_Beneficiary::getInstance();

	$form['prefix'] = array('#markup' => $policy_json['elections']['beneficiaries']['prefix']);
	$form += $beneficiary->form($form, $form_state, $worker_node);
	$form['suffix'] = array('#markup' => $policy_json['elections']['beneficiaries']['suffix']);

	if ($data['beneficiaries']) {
		sirius_jsonfield_util_set_defaults($form['beneficiaries'], $data['beneficiaries']);
	}

	return sirius_trust_election_wizard_form_prep($form, $form_state, $info, $data, $wizard_node, $current_step);
}

function sirius_trust_election_wizard_beneficiaries_form_validate($form, &$form_state) {
	$beneficiary = Sirius_Trust_Beneficiary::getInstance();
	$beneficiary->form_validate($form, $form_state);
}

function sirius_trust_election_wizard_beneficiaries_form_submit($form, &$form_state) {
	$info = $form_state['info'];
	$data = $form_state['data'];
	$wizard_node = $form_state['wizard_node'];
	$current_step = $form_state['current_step'];

	// Don't submit yet
	// $beneficiary->form_submit($form, &$form_state);
	$data['beneficiaries'] = $form_state['values']['beneficiaries'];

	return sirius_wizard_submit($info, $data, $wizard_node, $current_step, "Response saved.", array('nonsequential' => FALSE));
}

function sirius_trust_election_wizard_beneficiaries_post($info, &$data, $wizard_node, $current_step) {
	$worker_node = sirius_trust_election_wizard_get_worker($wizard_node);
	$worker_json = sirius_json_get($worker_node);
	$worker_json['beneficiaries'] = $data['beneficiaries'];
	sirius_json_set($worker_node, $worker_json);
	node_save($worker_node);
	return array('success' => TRUE);
}


function sirius_trust_election_wizard_beneficiaries_render($info, &$data, $wizard_node, $current_step) {
	$beneficiary = Sirius_Trust_Beneficiary::getInstance();
	$json = $data['beneficiaries'];
	return $beneficiary->render($json);
}

