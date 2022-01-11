<?php

function unite_local_acquire() {
	return drupal_get_form('unite_local_acquire_form', $node);
}

function unite_local_acquire_form($form, $form_state) {

	$form['acquire_source'] = array(
		'#title' => t("Acquiring Local"),
		'#description' => t('This is the local who will RECEIVE the relationships.'),
		'#required' => 1,
		'#type' => 'entityreference',
		'#era_entity_type' => 'node',  // Mandatory.
		'#era_bundles' => array('local'), // Optional (Any bundle by default).
		'#era_query_settings' => array(),
		'#era_cardinality' => 3,
	);

	$form['acquire_target'] = array(
		'#title' => t("Acquired Local"),
    '#autocomplete_path' => 'unite/ajax/quicksearch',
		'#description' => t('This is the local who will LOSE the relationships.'),
		'#required' => 1,
		'#type' => 'entityreference',
		'#era_entity_type' => 'node',  // Mandatory.
		'#era_bundles' => array('local'), // Optional (Any bundle by default).
		'#era_query_settings' => array(),
		'#era_cardinality' => 3,
	);
	
	$form['confirm'] = array(
		'#type' => 'checkbox', 
		'#title' => t("Yes, I'm really sure!"),
		'#required' => 1,
	);

	$form['submit'] = array(
		'#type' => 'submit', 
		'#value' => t('Acquire Now')
	);
	
	return $form;
}

function unite_local_acquire_form_get_locals($form, &$form_state) {
	$tmp = $form_state['values']['acquire_source'];
	if (!$tmp) { 
		form_set_error('acquire_source', t('Select an ACQUIRING local.'));
		return NULL;
	}
	$acquire_source_nid = array_keys($tmp)[0];

	$tmp = $form_state['values']['acquire_target'];
	if (!$tmp) { 
		form_set_error('acquire_target', t('Select an ACQUIRED local.'));
		return NULL;
	}
	$acquire_target_nid = array_keys($tmp)[0];
	
	$matches = array();
	if (preg_match('/\((\d+)\)/', $acquire_source, $matches) && $matches[1]) { 
		$acquire_source_nid = $matches[1];
	}
	if (!$acquire_source_nid) {
		form_set_error('acquire_source', t('Select an ACQUIRING local.'));
		return NULL;
	}

	$matches = array();
	if (preg_match('/\((\d+)\)/', $acquire_target, $matches) && $matches[1]) { 
		$acquire_target_nid = $matches[1];
	}
	if (!$acquire_target_nid) {
		form_set_error('acquire_target', t('Select an ACQUIRED local.'));
		return NULL;
	}
	
	if ($acquire_source_nid == $acquire_target_nid) {
		form_set_error('acquire_target', t('The two locals cannot be the same!'));
		return NULL;
	}

	$acquire_source_node = node_load($acquire_source_nid);
	if (!$acquire_source_node || $acquire_source_node->type != 'local') {
		form_set_error('acquire_source', t('ACQUIRING local must be an local, not a property.'));
		return NULL;
	}

	$acquire_target_node = node_load($acquire_target_nid);
	if (!$acquire_target_node || $acquire_target_node->type != 'local') {
		form_set_error('acquire_target', t('ACQUIRED local must be an local, not a property.'));
		return NULL;
	}
	
	return array($acquire_source_node, $acquire_target_node);
}

function unite_local_acquire_form_validate($form, &$form_state) {
	unite_local_acquire_form_get_locals($form, $form_state);
}

function unite_local_acquire_form_submit($form, &$form_state) {
	// All done.
	$form_state['rebuild'] = 1;

	list($acquire_source_node, $acquire_target_node) = unite_local_acquire_form_get_locals($form, $form_state);

	$stmt = db_update('field_data_field_local');
	$stmt->condition('entity_type', 'node');
	$stmt->condition('bundle', 'property');
	$stmt->condition('field_local_target_id', $acquire_target_node->nid);
	$stmt->fields(array('field_local_target_id' => $acquire_source_node->nid));
	$count = $stmt->execute();

	$stmt = db_update('field_revision_field_local');
	$stmt->condition('entity_type', 'node');
	$stmt->condition('bundle', 'property');
	$stmt->condition('field_local_target_id', $acquire_target_node->nid);
	$stmt->fields(array('field_local_target_id' => $acquire_source_node->nid));
	$other_count = $stmt->execute();

	field_cache_clear();

	drupal_set_message(
		$acquire_source_node->title . 
		t(' has acquired ') . 
		$count . 
		t(' properties from ') . 
		$acquire_target_node->title);
}
