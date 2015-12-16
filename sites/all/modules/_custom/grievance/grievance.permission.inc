<?php

/***
 * Implements hook_permission
 */

function grievance_permission() {
  return array(
    'grievance user' => array(
      'title' => t('Grievance User'), 
      'description' => t('Base access permission for using the grievance system.'),
    ),
    'grievance admin' => array(
      'title' => t('Grievance Admin'), 
      'description' => t('Grievance administrator, with permission to modify system settings.'),
    ),
 );
}

function grievance_node_access($node, $op, $account) {
	if ($node->type == 'grievance') { return grievance_node_access_grievance($node, $op, $account); }
	return NODE_ACCESS_IGNORE;
}

function  grievance_node_access_grievance($node, $op, $account) {
	// Admins can do anything
	if (user_access('grievance admin', $account->uid)) { return NODE_ACCESS_IGNORE; }

	// Must be on one of the user rep fields for the grievance
	$fields = array(
		'field_grievance_rep_filed',
		'field_grievance_rep_organizer',
		'field_grievance_rep_lead',
		'field_grievance_rep_manager',
		'field_grievance_rep_watching'
	);
	$found = FALSE;
	foreach ($fields as $field) {
		if (isset($node->{$field}['und'])) {
			foreach ($node->{$field}['und'] as $tmp) {
				if ($tmp['target_id'] == $account->uid) { $found = TRUE; }
			}
		}
	}
	if (!$found) { return NODE_ACCESS_DENY; }

	// The user must have a role that's granted permission on this status
	// (i.e. in general you can only edit "step 1" grievances.)
	$found = FALSE;
	$status_tid = $node->field_grievance_status['und'][0]['tid'];
	$status_term = taxonomy_term_load($status_tid);
	if (isset($status_term->field_grievance_roles['und'])) {
		foreach ($status_term->field_grievance_roles['und'] as $tmp) {
			if (user_has_role($tmp['value'], $account)) {
				$found = TRUE;
			}
		}
	}
	if (!$found) { return NODE_ACCESS_DENY; }

	// They passed all the tests
	return NODE_ACCESS_IGNORE;
}