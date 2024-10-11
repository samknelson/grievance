<?php

function sirius_trust_election_wizard_benefits_form($form, &$form_state, $info, $data, $wizard_node, $current_step) {
	$policy_node = sirius_trust_election_wizard_get_policy($data);
	$policy_json = sirius_json_get($policy_node);
	$worker_node = sirius_trust_election_wizard_get_worker($wizard_node);

	$benefits = sirius_trust_policy_benefits_offered($policy_node, array('electable' => TRUE, 'worker_node' => $worker_node));

	if (!$benefits) { return sirius_form_error('No benefits are offered under the policy ' . $policy_rnode->title); }

	$form['prefix'] = array('#markup' => $policy_json['elections']['benefits']['prefix']);

	$benefit_options_by_type = array();
	foreach ($benefits as $benefit_nid => $benefit_node) {
		$benefit_type_tid = $benefit_node->field_sirius_trust_benefit_type['und'][0]['tid'];
		$benefit_options_by_type[$benefit_type_tid][$benefit_nid] = $benefit_node->title;
	}

	foreach ($benefit_options_by_type as $benefit_type_tid => $benefit_options) {
		$form['benefits']['by_type'][$benefit_type_tid] = array(
			'#type' => 'select',
			'#title' => sirius_term_title($benefit_type_tid),
			'#options' => array(NULL => t('I waive this benefit')) + $benefit_options,
			'#default_value' => $data['benefits']['by_type'][$benefit_type_tid],
		);
	}

	$form['suffix'] = array('#markup' => $policy_json['elections']['benefits']['suffix']);

	return sirius_trust_election_wizard_form_prep($form, $form_state, $info, $data, $wizard_node, $current_step);
}

function sirius_trust_election_wizard_benefits_form_submit($form, &$form_state) {
	$info = $form_state['info'];
	$data = $form_state['data'];
	$current_step = $form_state['current_step'];
	$wizard_node = $form_state['wizard_node'];
	$data['benefits'] = $form_state['values']['benefits'];
	sirius_trust_election_set_title($wizard_node);
	sirius_wizard_submit($info, $data, $wizard_node, $current_step, "Response saved.", array('nonsequential' => TRUE));
}

function sirius_trust_election_wizard_benefits_render($info, $data, $wizard_node, $current_step) {
	$policy_node = sirius_trust_election_wizard_get_policy($data);
	$policy_json = sirius_json_get($policy_node);

	$html .= '<div class="sirius_wizard_render_section sirius_callout_basic">';
	$html .= '<h2>Elected Benefits</h2>';

	$form['prefix'] = array('#markup' => $policy_json['elections']['benefits']['prefix']);

	foreach ($data['benefits']['by_type'] as $type_tid => $benefit_nid) {
		$type_name = sirius_term_title($type_tid);
		if ($benefit_nid) {
			$benefit_name = sirius_node_title($benefit_nid);
		} else {
			$benefit_name = 'I waive this benefit';
		}

		$html .= sirius_fakerender_field($type_name . ' Benefit:', $benefit_name);
	}
	$html .= $policy_json['elections']['benefits']['suffix'];
	$html .= '</div>';

	return $html;
}