<?php

function sirius_trust_election_wizard_signature_form($form, &$form_state, $info, $data, $wizard_node, $current_step) {
	$policy_node = sirius_trust_election_wizard_get_policy($data);
	$policy_json = sirius_json_get($policy_node);

	sirius_esignature_init();
	sirius_require('sirius', 'sirius.page_entity_signatures.inc');

	$result = sirius_proxy_call('sirius_esignature_defs');
	if (!$result['success']) { return sirius_form_error($result['msg']); }
	$all_defs = $result['data'];

	$wizard_json = sirius_json_get($wizard_node);

	$which = $policy_json['elections']['esignature']['def'];
	$statement = $policy_json['elections']['esignature']['statement'];
	if (!$statement) { $statement = $wizard_node->title; }

	$esignature = sirius_jsonfield_util_esignature_get($wizard_json, $which);
	if ($esignature) {
		$form['signature'] = array('#markup' => sirius_esignature_render($esignature, 'full', $wizard_node->nid, $which));
		$form['action'] = array(
			'#type' => 'value', 
			'#value' => 'revoke'
		);
	} else {
		sirius_esignature_form_prepare($form, $which, "", $statement, array('show_name' => FALSE, 'show_org' => FALSE));
	}

	sirius_trust_election_wizard_form_prep($form, $form_state, $info, $data, $wizard_node, $current_step);
	if ($esignature) {
		$form['submit']['#value'] = 'revoke';
	} else {
		unset($form['submit']);
	}
	return $form;
}

function sirius_trust_election_wizard_signature_form_submit($form, &$form_state) {
	$info = $form_state['info'];
	$data = $form_state['data'];
	$wizard_node = $form_state['wizard_node'];
	$wizard_json = sirius_json_get($wizard_node);
	$current_step = $form_state['current_step'];

	$policy_node = sirius_trust_election_wizard_get_policy($data);
	$policy_json = sirius_json_get($policy_node);

	$which = $policy_json['elections']['esignature']['def'];
	$action = $form_state['values']['action'];
	if ($action == 'revoke') {
		drupal_set_message("Revoking signature.");
		sirius_jsonfield_util_esignature_set($wizard_json, $which, NULL);
		sirius_json_set($wizard_node, $wizard_json);
		sirius_trust_election_set_title($wizard_node);
		sirius_wizard_submit($info, $data, $wizard_node, $current_step, "Signature revoked.", array('incomplete' => TRUE, 'nonsequential' => TRUE));
	} else {
		$signature = sirius_esignature_form_get_json($form, $form_state, $which);
		sirius_jsonfield_util_esignature_set($wizard_json, $which, $signature);
		sirius_json_set($wizard_node, $wizard_json);
		sirius_trust_election_set_title($wizard_node);
		$wizard_node->field_sirius_type['und'][0]['value'] = 'submitted';
		sirius_wizard_submit($info, $data, $wizard_node, $current_step, "Response saved.", array('nonsequential' => TRUE));
	}
}
