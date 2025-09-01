<?php

function sirius_trust_election_wizard_type_form($form, &$form_state, $info, $data, $wizard_node, $current_step) {
	$policy_node = sirius_trust_election_wizard_get_policy($data);
	$policy_json = sirius_json_get($policy_node);
	$form['#tree'] = TRUE;

	$type_tids = array();
	if ($policy_json['elections']['types']['electable']) {
		$type_tids = array_filter(array_keys($policy_json['elections']['types']['electable']));
	}
	if (!$type_tids) { return sirius_form_error("No election types have been configured for this policy."); }

	$type_options = array();
	foreach ($type_tids as $type_tid) { $type_options[$type_tid] = sirius_term_title($type_tid); }
	$form['election_type']['tid'] = array(
		'#type' => 'select',
		'#title' => 'Election Type',
		'#options' => array(NULL => t('-- Select --')) + $type_options,
		'#required' => TRUE,
		'#default_value' => $data['election_type']['tid'],
	);

	return sirius_trust_election_wizard_form_prep($form, $form_state, $info, $data, $wizard_node, $current_step);
}

function sirius_trust_election_wizard_type_form_submit($form, &$form_state) {
	$info = $form_state['info'];
	$data = $form_state['data'];
	$current_step = $form_state['current_step'];
	$wizard_node = $form_state['wizard_node'];
	$data['election_type'] = $form_state['values']['election_type'];
	sirius_trust_election_set_title($wizard_node);
	sirius_wizard_submit($info, $data, $wizard_node, $current_step, "Response saved.", array('nonsequential' => FALSE));
}

function sirius_trust_election_wizard_type_render($info, &$data, $wizard_node, $current_step) {
	$html = sirius_fakerender_field('Election Type:', sirius_term_title($data['election_type']['tid']));

	return $html;
}
