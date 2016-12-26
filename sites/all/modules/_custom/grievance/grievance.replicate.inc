<?php
function grievance_contract_template_node_tab_replicate($node) {
  drupal_set_title($node->title . ': ' . t('Duplicate'));

	return drupal_get_form('grievance_replicate_form', $node);
}

function grievance_replicate_form($form, &$form_state, $node) {
	$form_state['node'] = $node;

	/*
  $form['holidays'] = array(
 		'#type' => 'checkboxes',
  	'#options' => $options,
  	'#title' => t('Select the holidays that are observed under this conrtract.'),
  	'#default_value' => $default_value,
  );
  */

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Duplicate Now',
  );
	
	return $form;
}

function grievance_replicate_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;

	$node = $form_state['node'];

	drupal_set_message(t('Duplicating: @title', array('@title' => $node->title)));

	$query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node');
  $query->fieldCondition('field_grievance_contract_tplt', 'target_id', $node->nid);
  $query->entityCondition('bundle', array('grievance_contract_section', 'grievance_contract_clause', 'grievance_contract_rule'), 'IN');
  $stmt = $query->execute();
  $child_nids = array();
  if ($stmt['node']) {
    $child_nids = array_keys($stmt['node']);
  }

	// Init
	$operations = array();
	$operations[] = array(
		'grievance_replicate_batch_init', 
		array($node),
	);

	// Add paths
	foreach ($child_nids as $child_nid) {
		$operations[] = array('grievance_replicate_batch', array($child_nid));
	}
	
	// Launch the batch processor.
  $batch = array(
    'operations' => $operations,
		'finished' => 'grievance_replicate_batch_finished'
  );
  batch_set($batch);

	return;
}

function grievance_replicate_batch_init($node, &$context) {
	$context['message'] = t('Duplicating node: @title', array('@title' => $node->title));

  $replica_nid = replicate_entity('node', $node);

  $context['results']['from_nid'] = $node->nid;
  $context['results']['to_nid'] = $replica_nid;
}

function grievance_replicate_batch($child_nid, &$context) {
	$child_node = node_load($child_nid);
  $replica_nid = replicate_entity('node', $child_node);
  $replica_node = node_load($replica_nid);
  $replica_node->field_grievance_contract_tplt['und'][0]['target_id'] = $context['results']['to_nid'];
  node_save($replica_node);

  $context['message'] = t('Replicated: @title', array('@title' => $child_node->title));
}

function grievance_replicate_batch_finished($success, $results, $operations) {
	drupal_set_message(t('Duplication complete.  The new entity can be viewed ' . l(t('here'), 'node/' . $results['to_nid'])));
}







/**
 * Implements hook_replicate_entity_ENITYT_TYPE
 */
function grievance_replicate_entity_node(&$replica) {
	if ($replica->type == 'grievance_contract_template') {
		$replica->title = 'Copy of ' . $replica->title;
		drupal_set_message("Replicating grievance contract template.");
	}
}