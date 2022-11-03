<?php

/************************************************************************************************
 * POLICY (admin only)
 ************************************************************************************************/

function sirius_trust_election_wizard_policy_form($form, &$form_state, $info, $data, $wizard_node, $current_step) {
	$result = sirius_proxy_call('sirius_ledger_policy_list');
	if (!$result['success']) { return sirius_form_error($result['msg']); }
	$policies = $result['data'];
	if ($policies) {
		$policy_options = array();
		foreach ($policies as $policy_rnid => $policy_rnode) {
			$policy_options[$policy_rnid] = $policy_rnode->title;
		}
		$form['policy']['policy_rnid'] = array(
			'#type' => 'select',
			'#title' => t('Policy'),
			'#options' => array(NULL => t('-- Select --')) + $policy_options,
			'#default_value' => $data['policy']['policy_rnid'],
			'#required' => TRUE,
		);
	}

	$result = sirius_proxy_call('sirius_employer_list', array());
	if (!$result['success']) { return sirius_form_error($result['msg']); }
	$employer_options = $result['data'];
	$form['policy']['employer_rnid'] = array(
		'#type' => 'select',
		'#title' => t('Employer'),
		'#options' => array(NULL => t('-- Select --')) + $employer_options,
		'#default_value' => $data['policy']['employer_rnid'],
	);

	return sirius_trust_election_wizard_form_prep($form, $form_state, $info, $data, $wizard_node, $current_step);
}

function sirius_trust_election_wizard_policy_form_submit($form, &$form_state) {
	$info = $form_state['info'];
	$data = $form_state['data'];
	$current_step = $form_state['current_step'];
	$wizard_node = $form_state['wizard_node'];
	$data['policy'] = $form_state['values']['policy'];
	sirius_trust_election_set_title($wizard_node);
	sirius_wizard_submit($info, $data, $wizard_node, $current_step, "Response saved.", array('nonsequential' => TRUE));

}
