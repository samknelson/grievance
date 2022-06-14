<?php

function sirius_trust_node_tab_worker_election_add($worker_node) {
	return drupal_get_form('sirius_trust_node_tab_worker_election_add_form', $worker_node);
}

function sirius_trust_node_tab_worker_election_add_form($form, &$form_state, $worker_node) {
	$form_state['worker_nid'] = $worker_node->nid;

	$employer_options = sirius_employer_list();

	$form['employer_nid'] = array(
		'#type' => 'select',
		'#title' => t('Employer'),
		'#options' => array(NULL => t('-- Select --')) + $employer_options,
		'#required' => TRUE, 
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_trust_node_tab_worker_election_add_form_validate($form, &$form_state) {
	$worker_node = node_load($form_state['worker_nid']);
}

function sirius_trust_node_tab_worker_election_add_form_submit($form, &$form_state) {
	$worker_node = node_load($form_state['worker_nid']);
	$relationship_node = node_load($form_state['values']['relationship_nid']);

	$path = 'node/add/sirius-trust-worker-election';
	$query = array();
	$query['destination'] = 'node/' . $worker_node->nid . '/sirius_trust_worker_elections';
	$query['grievance_shop'] = $form_state['values']['employer_nid'];
	$query['sirius_worker'] = $worker_node->nid;

	drupal_goto($path, array('query' => $query));
}

