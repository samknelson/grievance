<?php

function sirius_dispatch_facility_dropdown($options = array()) {
	$map = array();

	// Include null? Really should default to FALSE, but I'm lazily supporting some bad legacy code (that I wrote)
	$include_null = $options['include_null'];
	if (!isset($include_null)) { $include_null = TRUE; }
	if ($include_null) { $map[NULL] = t('-- Select --'); }

	// Run as raw $sql for efficiecy
	$domain_nid = sirius_domain_nid();

	$sql_args = array();
	$sql = "select nid, title from node ";
	if ($domain_nid) {
		$sql .= "join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid and field_sirius_domain_target_id = :domain_nid ";
		$sql_args[':domain_nid'] = $domain_nid;
	}
	$sql .= "where type = 'sirius_dispatch_facility' ";


	if ($options['employer_node']) {
		$employer_json = sirius_json_get($options['employer_node']);
		if ($employer_json['dispatch']['facilities']['restrict'] == 'Yes') {
			if (!$employer_json['dispatch']['facilities']['list']) { return $map; }
			$list = array_filter(array_values($employer_json['dispatch']['facilities']['list']));
			if (!$list) { return $map; }
			$sql .= "and nid in (:list) ";
			$sql_args[':list'] = $list;
		}
	}
	$sql .= "order by title ";



	$stmt = db_query($sql, $sql_args);
	while ($hr = $stmt->fetchAssoc()) {
		$map[$hr['nid']] = $hr['title'];
	}

	return $map;
}

function sirius_dispatch_facility_hall_dropdown($facility_nid) {
	// Run as raw $sql for efficiecy
	$sql = "select nid, title from node, field_data_field_sirius_dispatch_facility ";
	$sql .= "where type = 'sirius_dispatch_facility_hall' ";
	$sql .= "and field_data_field_sirius_dispatch_facility.entity_type = 'node' ";
	$sql .= "and field_data_field_sirius_dispatch_facility.entity_id = node.nid "; 
	$sql .= "and field_sirius_dispatch_facility_target_id = :facility_nid ";
	$slq .= "order by title ";
	$stmt = db_query($sql, array(':facility_nid' => $facility_nid));
	$map = array();
	$map[NULL] = t('-- Select --');
	while ($hr = $stmt->fetchAssoc()) {
		$map[$hr['nid']] = $hr['title'];
	}
	return $map;
}