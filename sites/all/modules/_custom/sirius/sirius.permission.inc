<?php

/***
 * Implements hook_permission
 */

function sirius_permission() {
  return array(
    'sirius' => array(
      'title' => t('Sirius'), 
      'description' => t('Permission to use any aspect of the Sirius system.  ALL users must have this permission.'),
    ),
    'sirius ssn' => array(
      'title' => t('Sirius SSN'), 
      'description' => t('Permission to view full SSNs'),
    ),
    'sirius news admin' => array(
      'title' => t('Sirius News Admin'), 
      'description' => t('Permission to add, edit, or delete news items'),
    ),
    'sirius log view' => array(
      'title' => t('Sirius Log View'), 
      'description' => t('View any log entry'),
    ),
    'sirius log edit' => array(
      'title' => t('Sirius Log Edit'), 
      'description' => t('Create or edit any log entry'),
    ),
    'sirius admin' => array(
      'title' => t('Sirius Admin'), 
      'description' => t('Administrative permission for the Sirius system'),
    ),
    'sirius edit own user' => array(
      'title' => t('Sirius Edit Own User'), 
      'description' => t('Permission to edit your own user account (i.e. set your password)'),
    ),
    'sirius representative' => array(
      'title' => t('Sirius Representative'), 
      'description' => t('Permission to act as representative of the union, such as a shop steward or business agent'),
    ),
    'sirius letterhead use' => array(
      'title' => t('Sirius Letterhead Use'), 
      'description' => t('Permission to print onto letterhead'),
    ),
    'sirius letterhead edit' => array(
      'title' => t('Sirius Letterhead Edit'), 
      'description' => t('Permission to create or edit letterhead'),
    ),
    'sirius signature edit' => array(
      'title' => t('Sirius Signature Edit'),
      'description' => t('Permission to edit an uploaded signature on a user record that you have access to. Most users will have access only to their own signature, but admin users will have access to everybody\'s.')
    ),
    'sirius contact admin' => array(
      'title' => t('Sirius Contact Admin'),
      'description' => t('Permission to add, edit, or delete any contact or any contact relationship.')
    ),
    'sirius domain switch' => array(
      'title' => t('Sirius Domain Switch'),
      'description' => t('Permission to change to a different domain'),
    ),
    'sirius domain admin' => array(
      'title' => t('Sirius Domain Admin'),
      'description' => t('Permission to perform any action within user\'s domain, including adding, modifying, or switching to subdomains'),
    ),
    'sirius domain delete' => array(
      'title' => t('Sirius Domain Delete'),
      'description' => t('Permission to delete a domain and all content associated with it. Requires permission on the domain (i.e. a domain admin with this permission can delete their own subdomains, but not other user\'s subdomains.)'),
    ),
    'sirius provider' => array(
      'title' => t('Sirius Provider'),
      'description' => t('Base level permission for all service providers, such as fund administrators or clinic operators.'),
    ),
    'sirius bookmark' => array(
      'title' => t('Sirius Bookmark'),
      'description' => t('Permission to bookmark nodes using the "Flag" module.'),
    ),
    'sirius public admin' => array(
      'title' => t('Sirius Public Admin'),
      'description' => t('Permission to manage public pages for a domain.'),
    ),
  );
}

/**
 * Implements hook_node_access
 */

function sirius_node_access($node, $op, $account) {
  // Get the node type
  if (is_object($node)) { $type = $node->type; } else { $type = $node; }

  // Check domain. 
  $domain_nid = $node->field_sirius_domain['und'][0]['target_id'];
  if ($domain_nid && !user_access('sirius admin', $account)) {
    if ($type == 'sirius_domain') {
      // For domain-type nodes, need access to both the domain and its parent
      if (!sirius_domain_access($domain_nid)) { return NODE_ACCESS_DENY; }
      if ($node->nid && !sirius_domain_access($node->nid)) { return NODE_ACCESS_DENY; }
    } else {
      // For all other nodes, it must be our current domain.
      static $my_domain_nid;
      if (!$my_domain_nid) { $my_domain_nid = sirius_domain_nid(); }
      if ($my_domain_nid != $domain_nid) {
        return NODE_ACCESS_DENY;
      }
    }
  }

  // Check access restrictions
  $result = sirius_node_access_restrict($node, $op, $account);
  if ($result != NODE_ACCESS_IGNORE) { return $result; }

  // Specific checks by node type
	if ($type == 'sirius_log') { return sirius_node_access_log($node, $op, $account); }
  if ($type == 'sirius_news') { return sirius_node_access_news($node, $op, $account); }
  if ($type == 'sirius_feed') { return sirius_node_access_feed($node, $op, $account); }
  if ($type == 'sirius_letterhead') { return sirius_node_access_letterhead($node, $op, $account); }
  if ($type == 'sirius_contact') { return sirius_node_access_contact($node, $op, $account); }
  if ($type == 'sirius_domain') { return sirius_node_access_domain($node, $op, $account); }
  if ($type == 'sirius_term_proxy') { return sirius_node_access_term_proxy($node, $op, $account); }

  // We know nothing.
  return NODE_ACCESS_IGNORE;
}


function sirius_node_access_restrict($node, $op, $account) {
  $type = is_string($node) ? $node : $node->type;
  if ($op != 'view') { return NODE_ACCESS_IGNORE; }

  if (user_access('sirius admin') || user_access('sirius domain admin')) { return NODE_ACCESS_IGNORE; }

  $json = sirius_json_get($node);
  if (!$json || !$json['restrict']) { return NODE_ACCESS_IGNORE; }

  global $user;

  // All users: restrict by role
  if ($json['restrict']['role']) {
    $ok_rids = array_filter(array_values($json['restrict']['role']));
    if ($ok_rids) {
      if (!$user->roles) { return NODE_ACCESS_DENY; }
      $my_rids = array_filter(array_keys($user->roles));
      if (!$my_rids) { return NODE_ACCESS_DENY; }
      if (!array_intersect($my_rids, $ok_rids)) { return NODE_ACCESS_DENY; }
    }
  }

  // The following restictions are for workers only. 
  if (user_access('sirius dispatch staff')) { return NODE_ACCESS_IGNORE; }

  $worker_node = sirius_worker_get();
  if (!$worker_node) { return NODE_ACCESS_IGNORE; }

  if ($json['restrict']['ms']) { 
    $ok_ms_tids = array_filter(array_values($json['restrict']['ms']));
    if ($ok_ms_tids) {
      $my_ms_tids = sirius_fieldvals($worker_node, 'field_sirius_member_status', 'tid');
      if (!$my_ms_tids) { return NODE_ACCESS_DENY; }
      if (!array_intersect($my_ms_tids, $ok_ms_tids)) { return NODE_ACCESS_DENY; }
    }
  }

  if ($json['restrict']['ws']) { 
    $ok_ws_tids = array_filter(array_values($json['restrict']['ws']));
    if ($ok_ws_tids) {
      $my_ws_tids = sirius_fieldvals($worker_node, 'field_sirius_work_status', 'tid');
      if (!$my_ws_tids) { return NODE_ACCESS_DENY; }
      if (!array_intersect($my_ws_tids, $ok_ws_tids)) { return NODE_ACCESS_DENY; }
    }
  }

  return NODE_ACCESS_IGNORE;
}

