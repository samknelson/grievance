<?php

function sirius_trust_denormalize_queue($node) {
  if ($node->type == 'sirius_trust_worker_benefit') {
    sirius_denorm_queue_add($node->nid, $node->field_sirius_worker['und'][0]['target_id'], 'sirius_trust_worker_benefits');
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
  if ($item['denorm_trigger'] == 'sirius_trust_worker_benefits') {
    sirius_trust_denorm_worker_benefits($item['denorm_target_nid']);
  }

  /*
  if ($item['denorm_trigger'] == 'sirius_trust_relationships') {
    sirius_trust_denorm_relationships($item['denorm_source_nid']);
  }
  */
}

/**
 * Make sure that there is a simple field in the Worker record that shows all active
 * worker benefits
 */

function sirius_trust_denorm_worker_benefits($worker_nid) {
  $worker_node = node_load($worker_nid);

  $benefits = sirius_trust_worker_benefits_search(array('worker_nid' => $worker_nid, 'active' => 'Yes'));

  $new_benefit_nids = array();
  foreach ($benefits as $worker_benefit) {
    $new_benefit_nids[] = $worker_benefit->field_sirius_trust_benefit['und'][0]['target_id'];
  }
  $new_benefit_nids = array_unique($new_benefit_nids);
  $old_benefit_nids = sirius_fieldvals($worker_node, 'field_sirius_denorm_benefits', 'target_id');

  if (array_diff($new_benefit_nids, $old_benefit_nids) || array_diff($old_benefit_nids, $new_benefit_nids)) {
    $worker_node->field_sirius_denorm_benefits = array();
    foreach ($new_benefit_nids as $new_benefit_nid) {
      $worker_node->field_sirius_denorm_benefits['und'][] = array('target_id' => $new_benefit_nid);
    }
    node_save($worker_node);
  }
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