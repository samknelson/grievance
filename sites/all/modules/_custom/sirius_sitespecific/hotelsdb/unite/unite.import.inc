<?php

/**
 * One-off functions. I believe that this entire file is unreferenced and should be removed.
 */

function unite_import_development_status() {
	$stmt = db_query(
		'SELECT nid FROM {node} WHERE type = :type',
		array(':type' => 'property')	
	);
	
	$nids = array();
	while ($hr = $stmt->fetchAssoc()) {
		$nids[] = $hr['nid'];
	}
	
	foreach ($nids as $nid) {
		print "Checking: $nid ...\n";
		$node = node_load($nid);
		
		if ($node->field_development_status['und'][0]) {
			print "... DS exists.  Skipping.\n";
			continue;
		}
		
		if (!$node->field_legacy_id['und'][0]['value']) {
			print "... No legacy id.  Skipping.\n";
			continue;
		}
		$legacy_id = $node->field_legacy_id['und'][0]['value'];
		$legacy_id = preg_replace('/property_/', '', $legacy_id);
		
		print "... Found legacy id $legacy_id ...\n";

		db_set_active('sps');
		$sql = "select * from h_Properties where property_id = :legacy_id ";
		$stmt = db_query($sql, array('legacy_id' => $legacy_id));
		$hr = $stmt->fetchAssoc();
		db_set_active('default');

		if (!$hr['development_status']) { 
			print "... no DS.  Skipping.\n";
			continue;
		}
		
		$node->field_development_status['und'][0]['value'] = $hr['development_status'];
		
		node_save($node);
		
		print "... Done\n";
		// break;
	}
}



function unite_import_geo() {
	$stmt = db_query(
		'SELECT nid FROM {node} WHERE type = :type',
		array(':type' => 'property')
	);
	
	$nids = array();
	while ($hr = $stmt->fetchAssoc()) {
		$nids[] = $hr['nid'];
	}
	
	foreach ($nids as $nid) {
		print "Checking: $nid ...\n";
		$node = node_load($nid);
		
		if ($node->field_address_geofield['und'][0]) {
			print "... Geofield exists.  Skipping.\n";
			continue;
		}
		
		if (!$node->field_legacy_id['und'][0]['value']) {
			print "... No legacy id.  Skipping.\n";
			continue;
		}
		$legacy_id = $node->field_legacy_id['und'][0]['value'];
		$legacy_id = preg_replace('/property_/', '', $legacy_id);
		
		print "... Found legacy id $legacy_id ...\n";

		db_set_active('sps');
		$sql = "select * from h_Properties where property_id = :legacy_id ";
		$stmt = db_query($sql, array('legacy_id' => $legacy_id));
		$hr = $stmt->fetchAssoc();
		db_set_active('default');

		if (!$hr['geo_latitude'] || !$hr['geo_longitude']) { 
			print "... no latitude.  Skipping.\n";
			continue;
		}
		
		$latitude = $hr['geo_latitude'] / 1000000;
		$longitude = $hr['geo_longitude'] / 1000000;

		print "... Setting: $latitude, $longitude ...\n";

		$node->field_address_geofield['und'][0]['lat'] = $latitude;
		$node->field_address_geofield['und'][0]['lon'] = $longitude;
		
		node_save($node);
		
		print "... Done\n";
		// break;
	}
}

function unite_import_brand() {
	db_set_active('sps');
	$sql = "select property_id, brand_id from h_Properties where brand_id is not null and brand_id > 0 ";
	$stmt = db_query($sql, array());
	$records = array();
	while ($hr = $stmt->fetchAssoc()) {
		$records[] = $hr;
	}
	db_set_active('default');

	foreach ($records as $hr) {
		print_r($hr);
		$legacy_property_id = $hr['property_id'];
		$legacy_brand_id = $hr['brand_id'];
		
		$sql = "select entity_id from field_data_field_legacy_id ";
		$sql .= "where field_legacy_id_value = :legacy_id ";
		$sql .= "and entity_type = 'taxonomy_term' ";
		$sql .= "and bundle = 'brand' ";
		$stmt = db_query($sql, array('legacy_id' => $legacy_brand_id));
		$hr2 = $stmt->fetchAssoc();
		$brand_tid = $hr2['entity_id'];
		if (!$brand_tid) { continue; }
		
		print "Brand TID: $brand_tid\n";

		$sql = "select entity_id from field_data_field_legacy_id ";
		$sql .= "where field_legacy_id_value = :legacy_id ";
		$sql .= "and entity_type = 'node' ";
		$sql .= "and bundle = 'property' ";
		$stmt = db_query($sql, array('legacy_id' => "property_$legacy_property_id"));
		$hr2 = $stmt->fetchAssoc();
		$property_nid = $hr2['entity_id'];
		if (!$property_nid) { die("Boo for $legacy_property_id "); continue; }
		
		print "Property NID: $property_nid\n";
		
		$node = node_load($property_nid);
		$node->field_brand['und'][0]['tid'] = $brand_tid;
		node_save($node);
	}
}

function unite_import_zip_to_msa() {
	$nids = db_query(
		'SELECT nid FROM {node} WHERE type = :type',
		array(':type' => 'zip_to_msa')
	);
	foreach ($nids as $nid) {
		print "Deleting " . $nid->nid . "\n";
		node_delete($nid->nid);
	}
	
	// Fetch from the old
	db_set_active('sps');
	// $last_wsh = 0;
	$sql = "select * from MSAZipLookup order by zip ";
	$stmt = db_query($sql, array("last_wsh" => $last_wsh));
	$records = array();
	while ($hr = $stmt->fetchAssoc()) {
		$records[] = $hr;
	}
	db_set_active('default');

	foreach ($records as $hr) {
		print_r($hr);
		
		$sql = "select entity_id from field_data_field_legacy_id ";
		$sql .= "where field_legacy_id_value = :legacy_id ";
		$sql .= "and entity_type = 'taxonomy_term' ";
		$sql .= "and bundle = 'msa' ";
		$stmt = db_query($sql, array('legacy_id' => $hr['msa_id']));
		$hr2 = $stmt->fetchAssoc();
		$entity_id = $hr2['entity_id'];
		if (!$entity_id) { die("No entity ID."); }
		
		print "$entity_id\n";
		
		$node = new stdClass();
		$node->type = "zip_to_msa";
		$node->language = LANGUAGE_NONE;
		$node->uid = 1;
		node_object_prepare($node);
		$node->field_zip["und"][0]["value"] = $hr['zip'];
		$node->field_msa['und'][0]['tid'] = $entity_id;
		$node = node_submit($node);
		node_save($node);
	}
}

function unite_geocode_batch() {
	$last_nid = variable_get('unite_geocode_last_nid', 0);
	
	$query = new EntityFieldQuery();
	$query->entityCondition('entity_type', 'node');
	$query->entityCondition('bundle', 'property');
	$query->propertyCondition('nid', $last_nid, '>');
	$query->fieldCondition('field_address', 'postal_code', '%', 'like');
	$query->range(0, 100);
	$result = $query->execute();
	if (!$result['node']) { 
		print "No more nodes.";
		exit();
	}
	
	$nids = array_keys($result['node']);
	foreach ($nids as $nid) {
		$node = node_load($nid);
		variable_set('unite_geocode_last_nid', $nid);
		print "Examining $nid " . $node->title . " ... \n";
		
		if ($node->field_address_geofield['und'][0]['lat']) { 
			print "... Has a latitude.  Skipping.\n";
			continue;
		}
		
		node_save($node);
		print "... Saved.\n";
	}
}