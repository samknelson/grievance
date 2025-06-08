<?php

function sirius_trust_election_wizard_disclaimers_form($form, &$form_state, $info, $data, $wizard_node, $current_step) {
	$policy_node = sirius_trust_election_wizard_get_policy($data);
	$policy_json = sirius_json_get($policy_node);

  $disclaimer_nids = array();
  if ($policy_json['elections']['disclaimer']['nids']) {
  	$disclaimer_nids = array_filter(array_values($policy_json['elections']['disclaimer']['nids']));
  }
  $disclaimer_nodes = node_load_multiple($disclaimer_nids);
	if (!$disclaimer_nodes) { return sirius_form_error('No disclaimers have been configured for this election.'); }

	$wizard_json = sirius_json_get($wizard_node);

	$accepted_all = TRUE;
	foreach ($disclaimer_nodes as $disclaimer_nid => $disclaimer_node) {
		$disclaimer_json = sirius_json_get($disclaimer_node)['disclaimer'];
		$acceptance_json = $wizard_json['disclaimer'][$disclaimer_nid]['acceptances'][0];
		if (!$acceptance_json) { $accepted_all = FALSE; }
		$form['disclaimer'][$disclaimer_nid] = sirius_disclaimer_render_form($disclaimer_json, $acceptance_json);			
	}

	return sirius_trust_election_wizard_form_prep($form, $form_state, $info, $data, $wizard_node, $current_step);
}

function sirius_trust_election_wizard_disclaimers_form_submit($form, &$form_state) {
	$info = $form_state['info'];
	$data = $form_state['data'];
	$wizard_node = $form_state['wizard_node'];
	$current_step = $form_state['current_step'];

	$disclaimer_nodes = node_load_multiple(array_keys($form_state['values']['disclaimer']));
	sirius_disclaimer_accept($wizard_node, $disclaimer_nodes, array());
	
	sirius_trust_election_set_title($wizard_node);
	sirius_wizard_submit($info, $data, $wizard_node, $current_step, "Response saved.", array('nonsequential' => FALSE));
}