<?php

function sirius_trust_denormalize_queue($node) {
  if ($node->type == 'sirius_trust_worker_benefit') {
    sirius_denorm_queue_add($node->nid, $node->field_sirius_worker['und'][0]['target_id'], 'sirius_trust_worker_benefits');
    sirius_denorm_queue_add($node->nid, $node->field_sirius_worker['und'][0]['target_id'], 'sirius_trust_worker_memdep_tag');
  }

  if ($node->type == 'sirius_trust_worker_election') {
    sirius_denorm_queue_add($node->nid, $node->field_sirius_worker['und'][0]['target_id'], 'sirius_trust_worker_election');
  }

  /*
  if ($node->type == 'sirius_contact_relationship') {
    sirius_denorm_queue_add($node->nid, $node->nid, 'sirius_trust_relationships');
  }
  */
}

/**
 * Implements hook_sirius_denorm
 */

function sirius_trust_sirius_denorm($item) {
  if ($item['denorm_trigger'] == 'sirius_trust_worker_memdep_tag') {
    sirius_trust_denorm_worker_memdep_tag($item['denorm_source_nid'], $item['denorm_target_nid']);
  }

  if ($item['denorm_trigger'] == 'sirius_trust_worker_election') {
    sirius_trust_denorm_worker_election($item['denorm_source_nid'], $item['denorm_target_nid']);
  }

  /*
  if ($item['denorm_trigger'] == 'sirius_trust_relationships') {
    sirius_trust_denorm_relationships($item['denorm_source_nid']);
  }
  */
}

function sirius_trust_denorm_worker_election($election_nid, $subscriber_nid) {
  // Check all the subscriber's benefits
  $params = array();
  $wb_nodes = sirius_trust_worker_benefits_search(array('subscriber_nid' => $subscriber_nid));
  foreach ($wb_nodes as $wb_nid => $wb_node) {
    $election_nid = sirius_trust_wb_find_election_nid($wb_node);
    if ($election_nid != $wb_node->field_sirius_trust_election['und'][0]['target_id']) {
      node_save($wb_node);
    }
  }
}

function sirius_trust_denorm_worker_memdep_tag($wb_nid, $worker_nid) {
  // Load the worker
  $worker_node = node_load($worker_nid);

  // Change into the correct domain. This should be done by the denorm function, but we can't count on it.
  $old_domain_nid = sirius_domain_nid();
  $new_domain_nid = $worker_node->field_sirius_domain['und'][0]['target_id'];
  if ($old_domain_nid != $new_domain_nid) {
    sirius_domain($new_domain_nid);
  }

  // Load the contact
  $contact_node = sirius_worker_contact_get($worker_node);
  if (!$contact_node) { 
    sirius_domain($old_domain_nid);
    return array('success' => FALSE, 'msg' => 'No contact for this worker.');
  }
  $contact_node->no_recurse = TRUE;

  // Load the member and dependent TID configuration variables
  $member_tid = sirius_domain_variable_get('sirius_trust_member_tid');
  if (!$member_tid) { 
    sirius_domain($old_domain_nid);
    return array('success' => FALSE, 'msg' => 'Member TID is not defined for this domain.');
  }

  $dependent_tid = sirius_domain_variable_get('sirius_trust_dependent_tid');
  if (!$dependent_tid) {
    sirius_domain($old_domain_nid);
    return array('success' => FALSE, 'msg' => 'Dependent TID is not defined for this domain.');
  }

  // Are we tagging as a member or a dependent?
  $wb_node = node_load($wb_nid);
  $relationship_nid = $wb_node->field_sirius_contact_relation['und'][0]['target_id'];
  if ($relationship_nid) {
    $tag_tid = $dependent_tid;
  } else {
    $tag_tid = $member_tid;
  }

  // Add the tag
  $result = sirius_contact_tag_add($contact_node, $tag_tid);
  // watchdog('whatever', "Adding $tag_id "

  // All done.
  sirius_domain($old_domain_nid);
  return $result;
}

/**
 * NOT USED
 * 
 * Each relationship needs a sequence number.
 * 
 * Sequence numbers are defined by the target of the relationship. For example, if contact A has multiple relationships 
 * with contact B, then all of those relationships should have the same sequence number.
 * 
 * Sequence numbers start with "01" and increment by 1. 
 * 
 * Sequence numbers may be imported, so the denorm should just fill in new numbers, not update existing ones.
 */

/*
function sirius_trust_denorm_relationships($relationship_nid) {
	$relationship_node = node_load($relationship_nid);
	$contact_nid = $relationship_node->field_sirius_contact['und'][0]['target_id'];
	watchdog('trust', "Denormalize relationships from $contact_nid");
}
*/