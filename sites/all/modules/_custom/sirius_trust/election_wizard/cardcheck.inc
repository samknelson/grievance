<?php

function sirius_trust_election_wizard_cardcheck_form($form, &$form_state, $info, $data, $wizard_node, $current_step) {
	$policy_node = sirius_trust_election_wizard_get_policy($data);
	$policy_json = sirius_json_get($policy_node);

	$definition_nid = preg_replace('/cardcheck_/', '', $current_step);
	$definition_node = node_load($definition_nid);
	if (!$definition_node) { return sirius_form_error("No cardcheck definition node found."); }

	$worker_node = sirius_trust_election_wizard_get_worker($wizard_node);

	// Find any existing signed cardcheck
	$cardcheck = Sirius_Cardcheck::getInstance();
	$search_params = array();
	$search_params['definition_nid'] = $definition_nid;
	$search_params['worker_nid'] = $worker_node->nid;
	$search_params['statuses'] = array('signed', 'unsigned');
	$cardcheck_nodes = $cardcheck->search($search_params);
	$cardcheck_node = reset($cardcheck_nodes);

	// Didn't find one. So we create a new (unsigned) entry.
	if (!$cardcheck_node) {
		$cardcheck_node = $cardcheck->create($definition_node, array('entity_node' => $worker_node));
	}

	// Render the form
	$cardcheck = Sirius_Cardcheck::getInstance();
	$form = $cardcheck->cardcheck_form($form, $form_state, $cardcheck_node);
	$form = sirius_trust_election_wizard_form_prep($form, $form_state, $info, $data, $wizard_node, $current_step);

	if ($cardcheck->status($cardcheck_node) == 'signed') {
		unset($form['revoke']);
		unset($form['confirm']);
	} else {
		unset($form['submit']);
		$form['signable'] = array('#type' => 'value', '#value' => TRUE);
	}


	return $form;
}

function sirius_trust_election_wizard_cardcheck_form_submit($form, &$form_state) {
	$info = $form_state['info'];
	$data = $form_state['data'];
	$wizard_node = $form_state['wizard_node'];
	$current_step = $form_state['current_step'];

	if ($form_state['values']['signable']) {
		$cardcheck = Sirius_Cardcheck::getInstance();
		$cardcheck->cardcheck_form_submit($form, $form_state);
	}

	sirius_trust_election_set_title($wizard_node);
	sirius_wizard_submit($info, $data, $wizard_node, $current_step, "Response saved.", array('nonsequential' => FALSE));
}