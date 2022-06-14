<?php

function sirius_trust_election_wizard_disclaimers_form($form, &$form_state, $info, $data, $wizard_node, $current_step) {
	return array();
	$result = sirius_trust_election_wizard_get_policy($data);
	if (!$result['success']) { return sirius_form_error($result['msg']); }
	$policy_rnode = $result['data'];
	$policy_json = sirius_json_get($policy_rnode);

  $disclaimer_rnids = array();
  if ($policy_json['elections']['disclaimer']['nids']) {
  	$disclaimer_rnids = array_filter(array_values($policy_json['elections']['disclaimer']['nids']));
  }

  $disclaimer_rnodes = array();
  foreach ($disclaimer_rnids as $disclaimer_rnid) {
		$result = sirius_proxy_call('sirius_proxy_node_load', $disclaimer_rnid);
		if (!$result['success']) { return sirius_form_error($result['msg']); }
		$disclaimer_rnodes[$disclaimer_rnid] = $result['data'];
  }

	$wizard_json = sirius_json_get($wizard_node);

	if (!$disclaimer_rnodes) { return sirius_form_error('No disclaimers have been configured for this election.'); }

	$accepted_all = TRUE;
	foreach ($disclaimer_rnodes as $disclaimer_rnid => $disclaimer_rnode) {
		$disclaimer_json = sirius_json_get($disclaimer_rnode)['disclaimer'];
		$acceptance_json = $wizard_json['disclaimer'][$disclaimer_rnid]['acceptances'][0];
		if (!$acceptance_json) { $accepted_all = FALSE; }
		$form['disclaimer'][$disclaimer_rnid] = sirius_disclaimer_render_form($disclaimer_json, $acceptance_json);			
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
	sirius_wizard_submit($info, $data, $wizard_node, $current_step, "Response saved.", array('nonsequential' => TRUE));
}