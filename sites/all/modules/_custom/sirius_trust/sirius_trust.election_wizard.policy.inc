<?php

/************************************************************************************************
 * POLICY (admin only)
 ************************************************************************************************/

function sirius_trust_election_wizard_policy_form($form, &$form_state, $info, $data, $wizard_node, $current_step) {
	// Get this list of employers
	$worker_node = sirius_trust_election_wizard_get_worker($wizard_node);
	if (!$worker_node) { return sirius_form_error("Worker not found."); }
	$search_params['worker_nid'] = $worker_node->nid;
	$search_params['statuses'] = array('accepted', 'accepted_secondary');
	$dispatches = sirius_dispatch_search($search_params);
	$employers = array();
	$employer_options = array();
	foreach ($dispatches as $dispatch_nid => $dispatch_node) {
		$job_node = node_load($dispatch_node->field_sirius_dispatch_job['und'][0]['target_id']);
		$employer_node = node_load($job_node->field_grievance_shop['und'][0]['target_id']);
		$employer_options[$employer_node->nid] = $employer_node->title;
	}

	$result = Sirius_Trust_Cobra::getInstance()->worker_can_elect($worker_node);
	$cobra_eligible = $result['success'];

	if (!$employer_options && !$cobra_eligible) {
		return sirius_form_error("You have no active employment record, and are not eligible for COBRA. You cannot make an election at this time.");
	}

	if (count($employer_options) < 1) {
		// Do nothing
	} else if (count($employer_options) == 1) {
		$form['employer']['employer_nid'] = array(
			'#type' => 'value',
			'#value' => reset(array_keys($employer_options)),
		);
		$form['employer']['info'] = array(
			'#markup' => sirius_fakerender_field(t('Home Employer:'), reset($employer_options)),
		);
	} else {
		$form['employer']['employer_nid'] = array(
			'#type' => 'select',
			'#title' => t('Select Your Home Employer'),
			'#options' => array(NULL => t('-- Select --')) + $employer_options,
			'#default_value' => $data['policy']['employer_nid'],
			'#required' => TRUE,
		);
	}

	if ($cobra_eligible) {
		$form['employer']['is_cobra'] = array(
			'#title' => 'COBRA Election?',
			'#type' => 'radios',
			'#options' => array(
				'No' => t('No, This is NOT a COBRA election'),
				'Yes' => t('Yes, This is a COBRA election because I have lost benefits at my home employer.'),
			),
			'#required' => TRUE,
		);
	}

	return sirius_trust_election_wizard_form_prep($form, $form_state, $info, $data, $wizard_node, $current_step);
}

function sirius_trust_election_wizard_policy_form_submit($form, &$form_state) {
	$info = $form_state['info'];
	$data = $form_state['data'];
	$current_step = $form_state['current_step'];
	$wizard_node = $form_state['wizard_node'];
	$worker_node = sirius_trust_election_wizard_get_worker($wizard_node);


	// Set the policy
	if ($form_state['values']['employer']['is_cobra'] == 'Yes') {
		// @todo
		$cobra = Sirius_Trust_Cobra::getInstance();
		$employer_node = $cobra->cobra_employer();
		if (!$employer_node) {
			drupal_set_message("There is no COBRA employer assigned. Please check the system configuration.", 'error');
			return;
		}
	} else {
		if ($form_state['values']['employer']['employer_nid']) {
			$employer_node = node_load($form_state['values']['employer']['employer_nid']);
		}
	}
	if (!$employer_node) {
		drupal_set_message("No employer found.", 'error');
		return;
	}

	$policy_node = sirius_ledger_policy_lookup($worker_node, $employer_node);
	if (!$policy_node) {
		drupal_set_message("No policy found for " . $employer_node->title, 'error');
		return;
	}

	$data['employer']['employer_nid'] = $employer_node->nid;
	$data['policy']['policy_nid'] = $policy_node->nid;

	sirius_trust_election_set_title($wizard_node);
	sirius_wizard_submit($info, $data, $wizard_node, $current_step, "Response saved.", array('nonsequential' => FALSE));

}
