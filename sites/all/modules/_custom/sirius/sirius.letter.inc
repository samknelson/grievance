<?php

function sirius_letter_access($action, $entity, $letter_node = NULL) {
	global $user; 

	if ($entity->tid) { $node = sirius_taxonomy_proxy($entity->tid); } else { $node = $entity; } 

	// Verify that this letter belongs to this node or entity
	if ($letter_node) { 
		if ($letter_node->field_sirius_log_handler['und'][0]['target_id'] == $node->nid) { $good = TRUE; }
		if (!$good) {
			$real_parent = node_load($letter_node->field_sirius_log_handler['und'][0]['target_id']);
			if ($real_parent->nid != $node->nid) { 
				return sirius_letter_access($action, $real_parent, $letter_node); 
			}
		}
		if (!$good) { return FALSE; }
	}

	// Verify that the user has access to this letter type
	// @todo

	// Admins can do anything
	if (user_access('sirius admin')) { return TRUE; }

	// //////////////////////////////////////////////////////
	// Type-specific logic
	// @todo: This should be in callbacks
	// //////////////////////////////////////////////////////

	if ($node->type == 'sirius_worker') {
		if (!node_access('view', $node)) { return FALSE; }

		// Admins can do anything
		if (user_access('sirius admin')) { return TRUE; }
		if (user_access('sirius domain admin')) { return TRUE; }
		if (user_access('sirius worker admin')) { return TRUE; }

		return FALSE;
	}

	if ($node->type == 'grievance') {
		if (!node_access('view', $node)) { return FALSE; }

		// Admins can do anything
		if (user_access('sirius admin')) { return TRUE; }
		if (user_access('sirius domain admin')) { return TRUE; }
		if (user_access('grievance admin')) { return TRUE; }
		if (user_access('grievance user')) { return TRUE; }

		return FALSE;
	}

	return FALSE;
}

function sirius_letter_list($entity) {
	if ($entity->tid) { $node = sirius_taxonomy_proxy($entity->tid); } else { $node = $entity; } 
	$query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'sirius_log');
  $query->fieldCondition('field_sirius_category', 'value', 'letter');
	$query->fieldCondition('field_sirius_log_handler', 'target_id', $node->nid);
  $query->propertyOrderBy('title');
  $stmt = $query->execute();
  if (!$stmt['node']) { return array(); }

  $all_nodes = node_load_multiple(array_keys($stmt['node']));
  $good_nodes = array();
  foreach ($all_nodes as $nid => $node) {
  	if (!sirius_letter_access('view', $entity, $node)) { continue; }
  	$good_nodes[$nid] = $node;
  }
  return $good_nodes;
}

function sirius_letter_admin() {
  $form = array();

  $name = sirius_domain_variable_name('sirius_letter_content_types');
  $form[$name] = array(
    '#type' => 'checkboxes',
    '#title' => t('Enable letters for these content types'),
    '#description' => t('The delivery token can be used to insert e.g. "Sent by Certified Mail" into certain letter templates.'),
    '#options' => array(
    	'sirius_worker' => 'Worker',
    	'grievance' => 'Grievance',
    ),
  );
  if (variable_get($name)) { $form[$name]['#default_value'] = variable_get($name); }

  $form['markup'] = array('#markup' => '<em>There are currently no letter configuration options.</em>');

  return system_settings_form($form);
}


