<?php

function sirius_trust_services($params = array()) {
	$query = new EntityFieldQuery;
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'sirius_trust_service');
  $query->fieldCondition('field_sirius_domain', 'target_id', sirius_domain_nid());
  if ($params['service_type_tid']) {
	  $query->fieldCondition('field_sirius_trust_service_type', 'tid', $params['service_type_tid']);
  }
  if ($params['active']) {
	  $query->fieldCondition('field_sirius_active', 'value', $params['active']);
  }
  $query->propertyOrderBy('title');
  $stmt = $query->execute();
  if (!$stmt['node']) { return array(); }
  return node_load_multiple(array_keys($stmt['node']));
}

function sirius_trust_service_options($params) {
	$nodes = sirius_trust_services($params);
	$options = array();
	foreach ($nodes as $nid => $node) { $options[$nid] = $node->title; }
	return $options;
}

/**
 * At the moment, there are three "special" service types: "inpatient", "outpatient", and "alternative".
 * Long term we probably don't want to hardcode these types. But the application is fluid and it's premature to 
 * build lots of structure. So we hardcode for now.
 */

function sirius_trust_service_type_hardcode_lookup() {
	$tids = array(
		'inpatient' => array(
			'title' => 'Inpatient',
			'landing_prompt' => 'Find an Outpatient Service',
		),
		'outpatient' => array(
			'title' => 'Outpatient',
			'landing_prompt' => 'Find an Inpatient Care Service',
		),
		'alternative' => array(
			'title' => 'Alternative',
			'landing_prompt' => 'Find Alternative Care Service',
		),
	);

	foreach ($tids as $id => $record) {
		$tid = sirius_find_tid_by_id($id, 'sirius_trust_service_type');
		if (!$tid) { return array('success' => FALSE, 'msg' => "Configuration error. Make sure that there is a service type with the ID $id."); }
		$tids[$id]['tid'] = $tid;
	}

	return array('success' => TRUE, 'data' => $tids);
}

