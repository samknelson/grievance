<?php

function sirius_trust_providers($active = TRUE) {
	$query = new EntityFieldQuery;
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'sirius_trust_provider');
  $query->fieldCondition('field_sirius_domain', 'target_id', sirius_domain_nid());
  if ($active) {
	  $query->fieldCondition('field_sirius_active', 'value', 'Yes');
  }
  $query->propertyOrderBy('title');
  $stmt = $query->execute();
  if (!$stmt['node']) { return array(); }
  return node_load_multiple(array_keys($stmt['node']));
}

function sirius_trust_provider_options($active = TRUE) {
	$nodes = sirius_trust_providers($active);
	$options = array();
	foreach ($nodes as $nid => $node) { $options[$nid] = $node->title; }
	return $options;
}
