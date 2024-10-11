<?php

function sirius_trust_election_wizard_intro_form($form, &$form_state, $info, $data, $wizard_node, $current_step) {
	$policy_node = sirius_trust_election_wizard_get_policy($data);
	$policy_json = sirius_json_get($policy_node);
	$intro = $policy_json['elections']['intro']['body'];

	$form['intro'] = array('#markup' => $intro);

	return sirius_trust_election_wizard_form_prep($form, $form_state, $info, $data, $wizard_node, $current_step);
}

function sirius_trust_election_wizard_intro_form_submit($form, &$form_state) {
	$info = $form_state['info'];
	$data = $form_state['data'];
	$wizard_node = $form_state['wizard_node'];
	$current_step = $form_state['current_step'];

	$data['identity'] = $form_state['values']['identity'];
	if ($event_json['intake']['anonymous']['require_ssn'] != 'No') {
		$data['identity']['ssn'] = sirius_ssn_format($data['identity']['ssn']);
	}

	sirius_trust_election_set_title($wizard_node);
	sirius_wizard_submit($info, $data, $wizard_node, $current_step, "Response saved.", array('nonsequential' => TRUE));
}
