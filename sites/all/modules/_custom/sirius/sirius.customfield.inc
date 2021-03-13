<?php

function sirius_customfield_set_list_all() {
	$avail = views_get_view_result('sirius_json_definitions', 'entityreference_1', sirius_domain_nid(), 'customfield');
	$options = array();
	if ($avail) {
		foreach ($avail as $hr) {
			$options[$hr->nid] = $hr->node_title;
		}
	}
	return $options;
}

function sirius_customfield_set_list($json) {
	$lookup = sirius_customfield_set_list_all();

	if (!$json || !$json['customfield']['sets']) { return array(); }
	$nids = array_filter(array_values($json['customfield']['sets']));
	$options = array();
	foreach ($nids as $nid) {
		$options[$nid] = $lookup[$nid];
	}
	return $options;
}

function sirius_customfield_value($node, $set_machine_name, $path) {
	$set_nid = sirius_customfield_set_lookup_by_machine_name($set_machine_name);
	if (!$set_nid) { return '[custom field set not found]'; }
  $which = 'customfield_' . $set_nid;
	$entity_json = sirius_jsonfield_util_node_json_get($node);
  $entity_results = sirius_jsonfield_util_results_get($entity_json, $which);
	return sirius_jsonfield_util_get_from_path($entity_results, $path);
}

function sirius_customfield_set_lookup_by_machine_name($machine_name) {
	// @todo: This is some really cumbersome code.
	$domain_nid = sirius_domain_nid();
  $query = new EntityFieldQuery;
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'sirius_json_definition');
  $query->fieldCondition('field_sirius_type', 'value', 'customfield');
  if ($domain_nid) { $query->fieldCondition('field_sirius_domain', 'target_id', $domain_nid); }
  $stmt = $query->execute();
  $nids = array();
  if (!$stmt['node']) { return NULL; }
	$nids = array_keys($stmt['node']);
  $nodes = node_load_multiple($nids);
  foreach ($nodes as $nid => $node) {
  	$json = sirius_jsonfield_util_node_json_get($node);
  	if ($json['machine_name'] == $machine_name) { return $nid; }
  }
 return NULL;
}