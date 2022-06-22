<?php

function sirius_trust_election_wizard_benefits_form($form, &$form_state, $info, $data, $wizard_node, $current_step) {
	$result = sirius_trust_election_wizard_get_policy($data);
	if (!$result['success']) { return sirius_form_error($result['msg']); }
	$policy_rnode = $result['data'];
	$policy_json = sirius_json_get($policy_rnode);

	$result = sirius_trust_election_wizard_get_worker($wizard_node);
	if (!$result['success']) { return; }
	$worker_rnode = $result['data'];

	$result = sirius_proxy_call(
		'sirius_trust_policy_benefits_offered', 
		$policy_rnode, 
		array(
			'electable' => TRUE, 
			'worker_node' => $worker_rnode,
		)
	);
	if (!$result['success']) { return sirius_form_error($result['msg']); }
	$benefits = $result['data'];
	if (!$benefits) { return sirius_form_error('No benefits are offered under the policy ' . $policy_rnode->title); }

	$form['prefix'] = array('#markup' => $policy_json['elections']['benefits']['prefix']);

	foreach ($benefits as $benefit_rnid => $benefit_rnode) {
		if (strlen($benefit_rnode->body['und'][0]['value']) > 10) {
			$form['benefits'][$benefit_rnid]['intro'] = array(
				'#markup' => $benefit_rnode->body['und'][0]['value'],
			);
		}

		$form['benefits'][$benefit_rnid]['election'] = array(
			'#type' => 'select',
			'#title' => $benefit_rnode->title,
			'#options' => array(
				NULL => t('-- Select --'),
				'No' => t('No'),
				'Yes' => t('Yes'),
			),
			'#default_value' => $data['benefits'][$benefit_rnid]['election'],
			'#required' => TRUE,
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
