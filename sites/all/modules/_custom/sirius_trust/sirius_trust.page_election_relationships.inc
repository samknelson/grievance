<?php

function sirius_trust_node_tab_election_relationships($election_node) {
	return drupal_get_form('sirius_trust_node_tab_election_relationships_form', $election_node);
}

function sirius_trust_node_tab_election_relationships_form($form, &$form_state, $election_node) {
	$form_state['election_nid'] = $election_node->nid;
	$form['#tree'] = TRUE;

	$worker_nid = $election_node->field_sirius_worker['und'][0]['target_id'];
	if (!$worker_nid) { return sirius_form_error('There is no worker associated with this election.'); }
	$worker_node = node_load($worker_nid);
	if (!$worker_node) { return sirius_form_error("The worker, NID $worker_nid, was not found."); }
	$election_type_term = taxonomy_term_load($election_node->field_sirius_trust_election_type['und'][0]['tid']);
	if (!$election_type_term) { return sirius_form_error('There is no election type associated with this election.'); }
	$election_type_json = sirius_json_get($election_type_term);
	$relationship_criteria = sirius_trust_relationship_criteria_get($election_type_json);

	$form['relationship_criteria'] = array(
		'#markup' => sirius_trust_relationship_criteria_render(
			$relationship_criteria, 
			array('title' => 'Please Select')
		)
	);

	$relationship_options = sirius_trust_relationship_options($worker_node);
	$form['relationships'] = array(
		'#type' => 'checkboxes',
		'#title' => 'Relationships',
		'#options' => $relationship_options,
		'#default_value' => sirius_form_array_default_value(
			sirius_fieldvals($election_node, 'field_sirius_contact_relations', 'target_id')
		),
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_trust_node_tab_election_relationships_form_validate($form, &$form_state) {
	$election_node = node_load($form_state['election_nid']);
	$election_type_term = taxonomy_term_load($election_node->field_sirius_trust_election_type['und'][0]['tid']);
	$election_type_json = sirius_json_get($election_type_term);
	$relationship_criteria = sirius_trust_relationship_criteria_get($election_type_json);

	$relationship_nids = array_filter(array_values($form_state['values']['relationships']));
	$result = sirius_trust_relationship_criteria_check($relationship_criteria, $relationship_nids);
	if (!$result['success']) { 
		form_set_error('relationships', $result['msg']);
	}
}

function sirius_trust_node_tab_election_relationships_form_submit($form, &$form_state) {
	$election_node = node_load($form_state['election_nid']);

	$nids = array_filter(array_values($form_state['values']['relationships']));
	$result = array();
	foreach ($nids as $nid) {
		$result[] = array('target_id' => $nid);
	}

	$election_node->field_sirius_contact_relations['und'] = $result;
	node_save($election_node);
	drupal_set_message("Election relationships saved.");
}