<?php

function grievance_node_tab_prior($grievance_node) {
  drupal_set_title($grievance_node->title . ': ' . t('Prior grievances'));
  $entity_nid = $grievance_node->nid;
  $grievance_min = $grievance_node->field_grievance_min['und'][0]['value'];
  if (!$grievance_min) { $grievance_min = '[empty]'; }

  $result = views_embed_view('grievance_prior_grievances', 'block', $grievance_min, $grievance_node->nid);
  return $result;
}

/** 
 * Get a count of prior grievances by a given member ID.  We could
 * just run the view, but this is faster and since we want to call 
 * it on every page, it's time to write some raw SQL.
 */

function grievance_prior_count($grievance_nid, $grievance_min) {
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'grievance');
  $query->fieldCondition('field_grievance_min', 'value', $grievance_min);
  $query->propertyCondition('nid', $grievance_nid, '!=');
  $stmt = $query->execute();
  if (!$stmt['node']) {
  	return 0;
  }
  return count($stmt['node']);
}