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
	// If the node is a grievance, check if we have access to it.
	if ($node->type == 'grievance') { return grievance_node_access_grievance($node, $op, $account); }

	// If the node references a grievance, check that we have access to that grievance.
	if (isset($node->field_grievance) && isset($node->field_grievance['und'][0]['target_id'])) {
		$grievance_node = node_load($node->field_grievance['und'][0]['target_id']);
		return grievance_node_access_grievance($grievance_node, $op, $account);
	}
	
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

	if (!$found) {
		// Can also see grievances at the same shop
		$user_full = user_load($account->uid);
		if ($user_full->field_grievance_shops['und'][0]['target_id'] == $node->field_grievance_shop['und'][0]['target_id']) {
			$found = TRUE;
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

function grievance_views_query_alter(&$view, &$query) {
	if ($view->name != 'grievances') { return; }

	global $user;
	$user_full = user_load($user->uid);

	foreach ($query->where as &$where) {
		$count = 0;
		foreach ($where['conditions'] as &$condition) {
			if (is_string($condition['field'])) {
				if ($condition['field'] == 'field_data_field_grievance_shop.field_grievance_shop_target_id') {
					if ($user_full->field_grievance_shops['und'][0]['target_id']) {
						$condition['value'] = $user_full->field_grievance_shops['und'][0]['target_id'];
					} else {
						unset($where['conditions'][$count]);
					}
				}
			}
			++$count;
		}
	}
}
