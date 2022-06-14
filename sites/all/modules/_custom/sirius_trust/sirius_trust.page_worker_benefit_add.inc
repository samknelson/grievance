<?php

function sirius_trust_node_tab_worker_benefit_add($worker_node) {
	return drupal_get_form('sirius_trust_node_tab_worker_benefit_add_form', $worker_node);
}

function sirius_trust_node_tab_worker_benefit_add_form($form, &$form_state, $worker_node) {
	$form_state['worker_nid'] = $worker_node->nid;

	$relationship_options = sirius_trust_relationship_options($worker_node, array('active' => 'Yes'));

	if ($relationship_options) {
		$relationship_options = array(NULL => t('[self]')) + $relationship_options;

		$form['relationship_nid'] = array(
			'#type' => 'select',
			'#title' => t('Beneficiary'),
			'#options' => $relationship_options,
		);
	}

	$employer_options = sirius_employer_list();

	$form['employer_nid'] = array(
		'#type' => 'select',
		'#title' => t('Employer'),
		'#options' => array(NULL => t('-- Select --')) + $employer_options,
		'#required' => TRUE, 
	);

	$policy_options = sirius_ledger_policy_options();

	$form['policy_nid'] = array(
		'#type' => 'select',
		'#title' => t('Policy'),
		'#options' => array(NULL => t('-- Select --')) + $policy_options,
		'#required' => TRUE, 
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_trust_node_tab_worker_benefit_add_form_validate($form, &$form_state) {
	$worker_node = node_load($form_state['worker_nid']);
}

function sirius_trust_node_tab_worker_benefit_add_form_submit($form, &$form_state) {
	$worker_node = node_load($form_state['worker_nid']);
	$relationship_node = node_load($form_state['values']['relationship_nid']);

	$path = 'node/add/sirius-trust-worker-benefit';
	$query = array();
	$query['destination'] = 'node/' . $worker_node->nid . '/sirius_trust_worker_benefits';
	$query['grievance_shop'] = $form_state['values']['employer_nid'];
	$query['sirius_trust_policy'] = $form_state['values']['policy_nid'];

	if ($relationship_node) {
		$beneficiary_contact_nid = $relationship_node->field_sirius_contact_alt['und'][0]['target_id'];
		$beneficiary_worker_node = sirius_worker_get_by_contact_nid($beneficiary_contact_nid);
		$query['sirius_worker'] = $beneficiary_worker_node->nid;
		$query['sirius_contact_relation'] = $relationship_node->nid;
	} else {
		$query['sirius_worker'] = $worker_node->nid;
	}

	drupal_goto($path, array('query' => $query));
}

