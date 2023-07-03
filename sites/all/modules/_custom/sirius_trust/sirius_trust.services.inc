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

function sirius_trust_service_options($params = array()) {
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
		'outpatient' => array(
			'title' => 'Outpatient',
			'landing_prompt' => 'Find an Outpatient Care Service',
			'is_facility' => FALSE,
		),
		'inpatient' => array(
			'title' => 'Inpatient',
			'landing_prompt' => 'Find an Inpatient Service',
			'is_facility' => TRUE,
		),
		'alternative' => array(
			'title' => 'Alternative',
			'landing_prompt' => 'Find Alternative Care Service',
			'is_facility' => FALSE,
		),
	);

	foreach ($tids as $id => $record) {
		$tid = sirius_find_tid_by_id($id, 'sirius_trust_service_type');
		// if (!$tid) { return array('success' => FALSE, 'msg' => "Configuration error. Make sure that there is a service type with the ID $id."); }
		if ($tid) {
			$tids[$id]['tid'] = $tid;
		} else {
			unset($tids[$id]); 
		}
	}

	if (!$tids) { return array('success' => FALSE, 'msg' => 'Configuration error. Make sure that there is at least one service type with one of the ids: inpatient, outpaitent, alternative.'); }

	return array('success' => TRUE, 'data' => $tids);
}


/**
 * This function returns the list of [service / provider] records, with rates.
 * 
 * At the moment this is done via 
 */

function sirius_trust_service_provider_search($service_node, $params = array()) {
	$service_json = sirius_json_get($service_node);
	$providers = array();

	// Find providers who have one of the levels.
	if ($service_json['rates']['by_level']) {
		$level_tids = array_keys(array_filter($service_json['rates']['by_level']));
	}
	if ($level_tids) {
		$sql_args = array();
		$sql = "select nid, ";
		$sql .= "title, ";
		$sql .= "field_sirius_trust_prov_levels_tid as level_tid, ";
		$sql .= "field_sirius_address_geo_lat as lat, ";		
		$sql .= "field_sirius_address_geo_lon as lon ";
		$sql .= "from node ";
		$sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
		$sql .= "left join field_data_field_sirius_trust_prov_levels on field_data_field_sirius_trust_prov_levels.entity_type = 'node' and field_data_field_sirius_trust_prov_levels.entity_id = node.nid ";
		$sql .= "left join field_data_field_sirius_address on field_data_field_sirius_address.entity_type = 'node' and field_data_field_sirius_address.entity_id = node.nid ";
		$sql .= "left join field_data_field_sirius_address_geo on field_data_field_sirius_address_geo.entity_type = 'node' and field_data_field_sirius_address_geo.entity_id = node.nid ";
		$sql .= "where node.type = 'sirius_trust_provider' ";
		$sql .= "and field_sirius_domain_target_id = :domain_nid ";
		$sql .= "and field_sirius_trust_prov_levels_tid in (:level_tids) ";
		if ($params['title_match']) {
			$sql .= "and title like :title_match ";
			$sql_args[':title_match'] = $params['title_match'];
		}
		$sql .= "group by nid order by title ";

		$sql_args[':domain_nid'] = sirius_domain_nid();
		$sql_args[':level_tids'] = $level_tids;

		$stmt = sirius_sql_query($sql, $sql_args);
		while ($hr = $stmt->fetchAssoc()) {
			$hr['rate'] = $service_json['rates']['by_level'][$hr['level_tid']];
			$providers[$hr['nid']] = $hr;
		}
	}

	// Find providers who have a negotiated rate
	$service_nid = $service_node->nid;
	$sql_args = array();
	$sql = "select nid, ";
	$sql .= "title, ";
	$sql .= "cast(json_unquote(json_extract(field_sirius_json_value, '\$.rates.by_service.$service_nid')) as signed) as rate, ";
	$sql .= "field_sirius_address_geo_lat as lat, ";		
	$sql .= "field_sirius_address_geo_lon as lon ";
	$sql .= "from node ";
	$sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_json on field_data_field_sirius_json.entity_type = 'node' and field_data_field_sirius_json.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_address on field_data_field_sirius_address.entity_type = 'node' and field_data_field_sirius_address.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_address_geo on field_data_field_sirius_address_geo.entity_type = 'node' and field_data_field_sirius_address_geo.entity_id = node.nid ";
	$sql .= "where node.type = 'sirius_trust_provider' ";
	$sql .= "and field_sirius_domain_target_id = :domain_nid ";
	$sql .= "and cast(json_unquote(json_extract(field_sirius_json_value, '\$.rates.by_service.$service_nid')) as signed) > 0 ";
		if ($params['title_match']) {
			$sql .= "and title like :title_match ";
			$sql_args[':title_match'] = $params['title_match'];
		}
	$sql .= "group by nid order by title ";

	$sql_args[':domain_nid'] = sirius_domain_nid();
	$stmt = sirius_sql_query($sql, $sql_args);
	while ($hr = $stmt->fetchAssoc()) {
		$providers[$hr['nid']] = $hr;
	}

	// Filter by distance
	if ($params['distance']) {
		$search_lat = $params['lat'];
		$search_lon = $params['lon'];
		$search_distance = $params['distance'];
		foreach ($providers as $provider_nid => $provider_record) {
			// Clear providers with no lat / lon
			if (!$provider_record['lat'] && !$provider_record['lon']) { 
				unset($providers[$provider_nid]);
				continue; 
			}

			// Clear providers who are too far away
			$distance = sirius_addr_latlon_distance($search_lat, $search_lon, $provider_record['lat'], $provider_record['lon']);
			if ($distance > $search_distance) { unset($providers[$provider_nid]); }
		}
	}

	return $providers;
}

function sirius_trust_provider_service_search($provider_node, $params = array()) {
	$service_json = sirius_json_get($service_node);
	$level_tids = sirius_fieldvals($provider_node, 'field_sirius_trust_prov_levels', 'tid');
	if (!$level_tids) { $level_tids = array(); }
	$services = array();

	// Find services that have one of those levels.
	foreach ($level_tids as $level_tid) {
		$sql_args = array();
		$sql_args = array();
		$sql = "select nid, ";
		$sql .= "title, ";
		$sql .= "cast(json_unquote(json_extract(field_sirius_json_value, '\$.rates.by_level.$level_tid')) as signed) as rate ";
		$sql .= "from node ";
		$sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
		$sql .= "left join field_data_field_sirius_json on field_data_field_sirius_json.entity_type = 'node' and field_data_field_sirius_json.entity_id = node.nid ";
		$sql .= "where node.type = 'sirius_trust_service' ";
		$sql .= "and field_sirius_domain_target_id = :domain_nid ";
		$sql .= "and cast(json_unquote(json_extract(field_sirius_json_value, '\$.rates.by_level.$level_tid')) as signed) > 0 ";
		$sql .= "group by nid order by title ";

		$sql_args[':domain_nid'] = sirius_domain_nid();

		$stmt = sirius_sql_query($sql, $sql_args);
		while ($hr = $stmt->fetchAssoc()) {
			$services[$hr['nid']] = $hr;
		}
	}

	// Find services with a negotiated rate
	$provider_json = sirius_json_get($provider_node);
	$rates_by_service = $provider_json['rates']['by_service'];
	if (!$rates_by_service) { $rates_by_service = array(); }
	foreach ($rates_by_service as $service_nid => $rate) {
		if (!$rate > 0) { continue; }

		$services[$service_nid] = array(
			'title' => sirius_node_title($service_nid),
			'rate' => $rate,
		);
	}

	return $services;
}
