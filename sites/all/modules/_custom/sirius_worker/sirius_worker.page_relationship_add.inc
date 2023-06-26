<?php

function sirius_node_tab_worker_relationship_add($worker_node) {
	return drupal_get_form('sirius_node_tab_worker_relationship_add_form', $worker_node);
}


function sirius_node_tab_worker_relationship_add_form($form, &$form_state, $worker_node) {
	$contact_node = node_load($worker_node->field_sirius_contact['und'][0]['target_id']);
	if (!$contact_node) {
		drupal_set_message("Contact not found for this worker.", 'error');
		return array();
	}

	$form_state['contact_nid'] = $contact_node->nid;
	$form_state['worker_nid'] = $worker_node->nid;

	$form['direction'] = array(
		'#type' => 'select',
		'#title' => 'Direction',
		'#options' => array(
			NULL => t('-- Select --'),
			'from' => 'FROM ' . $contact_node->title,
			'to' => 'TO ' . $contact_node->title,
		),
		'#required' => TRUE,
	);

	$form['ssn'] = array(
		'#type' => 'textfield',
		'#title' => 'SSN',
		'#size' => 10,
		'#required' => TRUE,
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_node_tab_worker_relationship_add_form_validate($form, &$form_state) {
	$contact_node = node_load($form_state['contact_nid']);
	$worker_node = node_load($form_state['worker_nid']);

	$ssn = trim($form_state['values']['ssn']);
	if (!sirius_ssn_validate($ssn)) {
		form_set_error('ssn', 'Please enter a valid SSN.');
		return;
	}

	$other_worker_node = sirius_worker_get_by_ssn($ssn);
	if (!$other_worker_node) {
		form_set_error('ssn', 'No worker was found with that SSN.');
		return;
	}

	$other_contact_node = node_load($other_worker_node->field_sirius_contact['und'][0]['target_id']);
	if (!$other_contact_node) {
		form_set_error('ssn', 'The worker with that SSN does not have a contact.');
		return;
	}

	if ($other_contact_node->nid == $contact_node->nid) {
		form_set_error('ssn', 'Relationships must be between two different workers.');
		return;
	}
}

function sirius_node_tab_worker_relationship_add_form_submit($form, &$form_state) {
	$contact_node = node_load($form_state['contact_nid']);
	$worker_node = node_load($form_state['worker_nid']);

	$ssn = trim($form_state['values']['ssn']);
	$other_worker_node = sirius_worker_get_by_ssn($ssn);
	$other_contact_node = node_load($other_worker_node->field_sirius_contact['und'][0]['target_id']);
	$path = 'node/add/sirius-contact-relationship';
	$query = array();
	$query['destination'] = 'node/' . $worker_node->nid . '/sirius_worker_relationships';
	if ($form_state['values']['direction'] == 'from') {
		$query['sirius_contact'] = $contact_node->nid;
		$query['sirius_contact_alt'] = $other_contact_node->nid;
	} else {
		$query['sirius_contact'] = $other_contact_node->nid;
		$query['sirius_contact_alt'] = $contact_node->nid;
	}

	drupal_goto($path, array('query' => $query));
}

