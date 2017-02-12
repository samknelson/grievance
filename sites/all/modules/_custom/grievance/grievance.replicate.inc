<?php

function grievance_contract_template_node_tab_replicate($node) {
  drupal_goto('node/' . $node->nid . '/replicate/all');
}

function grievance_contract_template_node_tab_replicate_all($node) {
  drupal_set_title($node->title . ': ' . t('Duplicate Entire Contract'));

	return drupal_get_form('grievance_contract_template_replicate_form_all', $node);
}

function grievance_contract_template_node_tab_replicate_rules($node) {
  drupal_set_title($node->title . ': ' . t('Duplicate Rules Only'));

  return drupal_get_form('grievance_contract_template_replicate_form_rules', $node);
}

function grievance_contract_template_replicate_form_all($form, &$form_state, $node) {
	$form_state['node'] = $node;

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Duplicate Now',
  );
	
	return $form;
}

function grievance_contract_template_replicate_form_all_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;

	$node = $form_state['node'];

	drupal_set_message(t('Duplicating: @title', array('@title' => $node->title)));

  // Iterate over the bundles, because we want to do all the sections first (so the clauses will know which section they're in)
  $child_nids = array();
  foreach (array('grievance_contract_section', 'grievance_contract_clause', 'grievance_contract_rule') as $bundle) {
  	$query = new EntityFieldQuery();
    $query->entityCondition('entity_type', 'node');
    $query->fieldCondition('field_grievance_contract_tplt', 'target_id', $node->nid);
    $query->entityCondition('bundle', $bundle);
    $stmt = $query->execute();
    if ($stmt['node']) {
      foreach (array_keys($stmt['node']) as $child_nid) {
        $child_nids[] = $child_nid;
      }
    }
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
  if ($replica_node->type == 'grievance_contract_clause') {
    $target = $context['results']['section_map'][$replica_node->field_grievance_contract_section['und'][0]['target_id']];
    if ($target) {
      $replica_node->field_grievance_contract_section['und'][0]['target_id'] = $target;
    }
  }
  node_save($replica_node);

  if ($replica_node->type == 'grievance_contract_section') {
    if (!$context['results']['section_map']) {
      $context['results']['section_map'] = array();
    }
    $context['results']['section_map'][$child_node->nid] = $replica_node->nid;
  }

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





function grievance_contract_template_replicate_form_rules($form, &$form_state, $node) {
  $form_state['node'] = $node;

  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'grievance_contract_template');
  $query->propertyCondition('nid', $node->nid, '!=');
  $query->propertyOrderBy('title');
  $stmt = $query->execute();

  $options = array(NULL => t('-- Target Contract --'));
  if ($stmt['node']) {
    $target_contract_nodes = node_load_multiple(array_keys($stmt['node']));
    foreach ($target_contract_nodes as $target_contract_nid => $target_contract_node) {
      $options[$target_contract_nid] = $target_contract_node->title;
    }
  }

  $form['target'] = array(
    '#type' => 'select',
    '#required' => TRUE,
    '#options' => $options,
    '#title' => t('Target Contract'),
    '#description' => t('The rules from this contract template will REPLACE the rules in the target contract.')
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Copy Now',
  );
  
  return $form;
}

function grievance_contract_template_replicate_form_rules_submit($form, &$form_state) {
  $form_state['rebuild'] = TRUE;

  $source_node = $form_state['node'];
  $source_nid = $source_node->nid;
  $target_nid = $form_state['values']['target'];
  $target_node = node_load($target_nid);
  if (!$target_node) {
    drupal_set_message(t('Target could not be loaded'), 'error');
    return;
  }

  // First clear the rules from the target node
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node');
  $query->fieldCondition('field_grievance_contract_tplt', 'target_id', $target_nid);
  $query->entityCondition('bundle', 'grievance_contract_rule');
  $stmt = $query->execute();
  $delete_count = 0;
  if ($stmt['node']) {
    foreach (array_keys($stmt['node']) as $child_nid) {
      ++$delete_count;
      node_delete($child_nid);
    }
  }

  // Now copy all the rules from the source node
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node');
  $query->fieldCondition('field_grievance_contract_tplt', 'target_id', $source_nid);
  $query->entityCondition('bundle', 'grievance_contract_rule');
  $stmt = $query->execute();
  $insert_count = 0;
  if ($stmt['node']) {
    foreach (array_keys($stmt['node']) as $child_nid) {
      ++$insert_count;      $child_node = node_load($child_nid);

      $replica_nid = replicate_entity('node', $child_node);
      $replica_node = node_load($replica_nid);
      $replica_node->field_grievance_contract_tplt['und'][0]['target_id'] = $target_nid;
      node_save($replica_node);
    }
  }

  $msg = t('Success! Copied <strong>@insert_count</strong> new rules from [<strong>@source_title</strong>] to [<strong>@target_title</strong>].', 
    array(
      '@insert_count' => $insert_count, 
      '@source_title' => $source_node->title,
      '@target_title' => $target_node->title
    )
  );
  
  if ($delete_count) {
    $msg .= t(' (<strong>@delete_count</strong> old rules were deleted from <strong>@target_title</strong>.)', 
      array(
        '@delete_count' => $delete_count, 
        '@target_title' => $target_node->title
      )
    );
  }

  drupal_set_message($msg);
}
