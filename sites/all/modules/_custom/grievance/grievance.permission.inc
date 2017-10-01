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
    'grievance view' => array(
    	'title' => t('Grievance View'),
    	'description' => t('Permission to view any grievance on the system.  Does not grant any editing permissions.')
    ),
    'grievance edit' => array(
        'title' => t('Grievance Edit'),
        'description' => t('Permission to edit any grievane on the system.')
    ),
    'grievance attachment' => array(
    	'title' => t('Grievance Attachments'),
    	'description' => t('Permission to upload attachments to grievances.')
    ),
    'grievance forward' => array(
    	'title' => t('Grievance Forward'),
    	'description' => t('Permission use the "forward a grievance" form.')
    ),
    'grievance statuslog' => array(
    	'title' => t('Grievance Status Log'),
    	'description' => t('Permission to view or edit the status log for a grievance.')
    ),
    'grievance arbitrationform' => array(
    	'title' => t('Grievance Arbitration Form'),
    	'description' => t('Permission to view or edit the arbitration form for a grievance.')
    ),
    'grievance notification' => array(
    	'title' => t('Grievance Notification'),
    	'description' => t('DEPRECATED - Permission to use the Notification tab.')
    ),
    'grievance form' => array(
        'title' => t('Grievance Form'),
        'description' => t('DEPRECATED - Permission to use the Forms tab.')
    ),
    'grievance settlement' => array(
    	'title' => t('Grievance Settlements'),
    	'description' => t('Permission to view and edit settlements for a grievance.')
    ),
    'grievance contact' => array(
    	'title' => t('Grievance Contacts'),
    	'description' => t('Permission to edit contacts for a shop of company.')
    ),
    'grievance report' => array(
    	'title' => t('Grievance Report'),
    	'description' => t('Permission to view most grievance reports.')
    ),
    'grievance report bytype' => array(
        'title' => t('Grievance Report Bytype'),
        'description' => t('Permission to view the "by type and status" report.')
    ),
    'grievance bundle' => array(
        'title' => t('Grievance Bundle'),
        'description' => t('Permission to generate and download grievance bundles.')
    ),
    'grievance quicksearch' => array(
    	'title' => t('Grievance Quicksearch'),
    	'description' => t('Permission to use the "quicksearch" grievance search.')
    ),
    'grievance contract' => array(
    	'title' => t('Grievance Contract'),
    	'description' => t('Permission to edit contracts and contract templates.')
    ),
    'grievance form contract template' => array(
    	'title' => t('Grievance Form - Change Contract Template'),
    ),
    'grievance form status date' => array(
    	'title' => t('Grievance Form - Change Status Date'),
    ),
    'grievance form alert tid' => array(
    	'title' => t('Grievance Form - Change Alert (aka Next Step)'),
    ),
    'grievance form alert date' => array(
    	'title' => t('Grievance Form - Change Next Deadline Date'),
    ),
    'grievance form grievance type' => array(
    	'title' => t('Grievance Form - Change Grievance Type'),
    ),
    'grievance form grievance type other' => array(
    	'title' => t('Grievance Form - Change Grievance Type (Other)'),
    ),
    'grievance form grievance remedy' => array(
    	'title' => t('Grievance Form - Change Remedy'),
    ),
    'grievance form grievance remedy other' => array(
    	'title' => t('Grievance Form - Change Remedy (Other)'),
    ),
    'grievance form supervisor' => array(
        'title' => t('Grievance Form - Change Supervisor'),
    ),
    'grievance form rep filed' => array(
    	'title' => t('Grievance Form - Change Filed By'),
    ),
    'grievance form rep organizer' => array(
    	'title' => t('Grievance Form - Change Organizer'),
    ),
    'grievance form rep lead' => array(
    	'title' => t('Grievance Form - Change Lead'),
    ),
    'grievance form rep manager' => array(
    	'title' => t('Grievance Form - Change Grievance Admin'),
    ),
    'grievance form rep watching' => array(
    	'title' => t('Grievance Form - Change Watching'),
    ),
    'grievance form rep assignee' => array(
        'title' => t('Grievance Form - Change Assignee'),
    ),   
    'grievance form company contacts' => array(
        'title' => t('Grievance Form - Change Company Represenative'),
    ),
    'grievance form st contacts' => array(
        'title' => t('Grievance Form - Change Shop Steward'),
    ),
    'grievance form id' => array(
        'title' => t('Grievance Form - Change Grievance ID'),
    ),
    'grievance letter variable delivery' => array(
        'title' => t('Grievance Letter - Change the Delivery drop-in variable'),
    ),
    'grievance letter variable letterhead' => array(
        'title' => t('Grievance Letter - User letterhead'),
    ),
    'grievance mine no first call' => array(
        'title' => t('My Grievances - No First Call'),
    ),
    'grievance mine expanded' => array(
        'title' => t('My Grievances - Tickler'),
    ),
 );
}

function grievance_node_access($node, $op, $account) {
	// If the node is a grievance, check if we have access to it.
	if ($node->type == 'grievance') { return grievance_node_access_grievance($node, $op, $account); }

    // If the node references a grievance, check that we have access to that grievance.
    if (isset($node->field_grievance) && isset($node->field_grievance['und'][0]['target_id'])) {
        $grievance_node = node_load($node->field_grievance['und'][0]['target_id']);
        $result = grievance_node_access_grievance($grievance_node, $op, $account);
        if ($result != NODE_ACCESS_IGNORE) { return $result; }
    }

    // Some additional restrictions based on log type.
    if ($node->type == 'grievance_log') { return grievance_node_access_grievance_log($node, $op, $account); }
	
	return NODE_ACCESS_IGNORE;
}

function grievance_node_access_grievance($node, $op, $account) {
 	// Admins can do anything
	if (user_access('grievance admin', $account)) { return NODE_ACCESS_IGNORE; }

    // Editors can do anything.  @todo probably should treate delete specially.
    if (user_access('grievance edit')) { return NODE_ACCESS_IGNORE; }

	// Viewers can view
	if ($op == 'view' && user_access('grievance view', $account)) { return NODE_ACCESS_IGNORE; }

	// Must be on one of the user rep fields for the grievance
	$fields = array(
		'field_grievance_rep_filed',
		'field_grievance_rep_organizer',
		'field_grievance_rep_lead',
		'field_grievance_rep_manager',
		'field_grievance_rep_watching',
        'field_grievance_rep_assignee',
	);
	$found = FALSE;
	foreach ($fields as $field) {
		if (isset($node->{$field}['und'])) {
			foreach ($node->{$field}['und'] as $tmp) {
				if ($tmp['target_id'] == $account->uid) { $found = TRUE; }
			}
		}
	}

	/*
	if (!$found) {
		// Can also see grievances at the same shop
		$user_full = user_load($account->uid);
		if ($user_full->field_grievance_shops['und'][0]['target_id'] == $node->field_grievance_shop['und'][0]['target_id']) {
			$found = TRUE;
		}
	}
	*/
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

function grievance_views_query_alter_permission(&$view, &$query) {
	if ($view->name != 'grievances') { return; }

	global $user;
	$user_full = user_load($user->uid);

	foreach ($query->where as &$where) {
		$count = 0;
		foreach ($where['conditions'] as &$condition) {
			if (is_string($condition['field'])) {
				if ($condition['field'] == 'field_data_field_grievance_shop.field_grievance_shop_target_id') {
					/*
					if ($user_full->field_grievance_shops['und'][0]['target_id']) {
						$condition['value'] = $user_full->field_grievance_shops['und'][0]['target_id'];
					} else {
						unset($where['conditions'][$count]);
					}
					*/
					unset($where['conditions'][$count]);
				}
			}
			++$count;
		}
	}
}
