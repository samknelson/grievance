<?php

function unite_export_union_hotel_guide_access() {
	return sirius_ip_check(sirius_loopback_remote_ip(), variable_get('unite_export_ip_allowlist'));
}

function unite_export_union_hotel_guide_old() {
	$query = new EntityFieldQuery();
	$query->entityCondition('entity_type', 'node');
	$query->entityCondition('bundle', 'property');
	$query->propertyOrderBy('nid');
	$result = $query->execute();

	db_set_active('sps');
	$sql = "drop table if exists h_Properties_Incoming";
	print "$sql\n";
	$stmt = db_query($sql);
	$sql = "create table h_Properties_Incoming like h_Properties";
	print "$sql\n";
	$stmt = db_query($sql);
	db_set_active('default');


	foreach ($result['node'] as $nid => $ignore) {
		print "Inserting: $nid\n";
		$node = node_load($nid);
		print "... loaded ...\n";
		$legacy_id = $node->field_legacy_id['und'][0]['value'];
		$legacy_id = preg_replace('/property_/', '', $legacy_id);
		if (!$legacy_id) { $legacy_id = $nid + 10000000; }
		
		$msa_tid = $node->field_msa['und'][0]['tid'];
		$$msa_legacy_id = null;
		if ($msa_tid) {
			$msa_term = taxonomy_term_load($msa_tid);
			$msa_legacy_id = $msa_term->field_legacy_id['und'][0]['value'];
		}
		
		$development_status = $node->field_development_status['und'][0]['value'];
		if (!$development_status) { $development_status = ''; }
		db_set_active('sps');
		$sql = "insert into h_Properties_Incoming (";
		$sql .= "property_id, ";
		$sql .= "property_name, ";
		$sql .= "loc_addr1, ";
		$sql .= "loc_addr2, ";
		$sql .= "loc_city, ";
		$sql .= "loc_state, ";
		$sql .= "loc_zip, ";
		$sql .= "loc_country, ";
		$sql .= "loc_phone, ";
		$sql .= "loc_fax, ";
		$sql .= "loc_website, ";
		$sql .= "hla_index_new, ";
		$sql .= "development_status, ";
		$sql .= "geo_latitude, ";
		$sql .= "geo_longitude, ";
		$sql .= "msa_id, ";
		$sql .= "mod_date_prop ";
		$sql .= ") values (";
		$sql .= ":property_id, ";
		$sql .= ":property_name, ";
		$sql .= ":loc_addr1, ";
		$sql .= ":loc_addr2, ";
		$sql .= ":loc_city, ";
		$sql .= ":loc_state, ";
		$sql .= ":loc_zip, ";
		$sql .= ":loc_country, ";
		$sql .= ":loc_phone, ";
		$sql .= ":loc_fax, ";
		$sql .= ":loc_website, ";
		$sql .= ":hla_index_new, ";
		$sql .= ":development_status,";
		$sql .= ":geo_latitude, ";
		$sql .= ":geo_longitude, ";
		$sql .= ":msa_id, ";
		$sql .= ":mod_date_prop ";
		$sql .= ") ";
		print "... about to save ...\n";
		$stmt = db_query($sql, array(
			':property_id' => $legacy_id,
			':property_name' => $node->title,
			':loc_addr1' => $node->field_address['und'][0]['thoroughfare'],
			':loc_addr2' =>  $node->field_address['und'][0]['premise'],
			':loc_city' =>  $node->field_address['und'][0]['locality'],
			':loc_state' =>  $node->field_address['und'][0]['administrative_area'],
			':loc_zip' =>  $node->field_address['und'][0]['postal_code'],
			':loc_country' =>  $node->field_address['und'][0]['country'],
			':loc_phone' =>  $node->field_phone['und'][0]['value'],
			':loc_fax' => $node->field_fax['und'][0]['value'],
			':loc_website' => substr($node->field_url['und'][0]['url'], 0, 128),
			':hla_index_new' => $node->field_labor_activity_index['und'][0]['value'],
			':development_status' => $development_status,
			':geo_latitude' => $node->field_address_geofield['und'][0]['lat'] * 1000000,
			':geo_longitude' => $node->field_address_geofield['und'][0]['lon'] * 1000000,
			':msa_id' => $msa_legacy_id,
			':mod_date_prop' => date('Y-m-d H:i:s', $node->changed),
		));
		print "... saved.\n";
		
		db_set_active('default');
	}
	
	db_set_active('sps');
	$sql = "drop table if exists h_Properties_Outgoing";
	print "$sql\n";
	$stmt = db_query($sql);
	$sql = "alter table h_Properties rename to h_Properties_Outgoing";
	print "$sql\n";
	$stmt = db_query($sql);
	$sql = "alter table h_Properties_Incoming rename to h_Properties";
	print "$sql\n";
	$stmt = db_query($sql);
	db_set_active('default');

	
}


function unite_export_union_hotel_guide_new($secs = 0) {
	if (!$secs) { $secs = $_REQUEST["secs"]; }

	$query = new EntityFieldQuery();
	$query->entityCondition('entity_type', 'node');
	$query->entityCondition('bundle', 'property');
	$query->propertyOrderBy('nid');
	if ($secs) {
		$ts = time() - $secs;
		$query->propertyCondition('changed', $ts, '>=');
	}
	$result = $query->execute();

	$nodes = $result['node'];
	if (!$nodes) { $nodes = array(); }

	if (!$_REQUEST['onscreen']) {
		drupal_add_http_header('Content-Type', 'text/csv');
	}
	$fp = fopen('php://output', 'w');
	
	$fields = array(
		'nid',
		'title',
		'address - state',
		'address - country',
		'address - city',
		'address - zip',
		'address - premise',
		'address - street',
		'development_status',
		'fax',
		'phone',
		'rooms',
		'union_status',
		'url',
		'latitude',
		'longitude',
		'labor_activity_index',
		'boycott_url'
	);
	
	fputcsv($fp, $fields);

	foreach ($nodes as $nid => $ignore) {
		$node = node_load($nid);
		$row = array();
		$row[] = $node->nid;
		$row[] = $node->title;
		$row[] = $node->field_address['und'][0]['administrative_area'];
		$row[] = $node->field_address['und'][0]['country'];
		$row[] = $node->field_address['und'][0]['locality'];
		$row[] = $node->field_address['und'][0]['postal_code'];
		$row[] = $node->field_address['und'][0]['premise'];
		$row[] = $node->field_address['und'][0]['thoroughfare'];
		$row[] = $node->field_development_status['und'][0]['value'];
		$row[] = $node->field_fax['und'][0]['value'];
		$row[] = $node->field_phone['und'][0]['value'];
		$row[] = $node->field_rooms['und'][0]['value'];
		$row[] = $node->field_union_status['und'][0]['value'];
		$row[] = $node->field_url['und'][0]['url'];
		$row[] = $node->field_address_geofield['und'][0]['lat'];
		$row[] = $node->field_address_geofield['und'][0]['lon'];
		$row[] = $node->field_labor_activity_index['und'][0]['value'];
		$row[] = $node->field_boycott_url['und'][0]['url'];
		
		fputcsv($fp, $row);
	}

	fclose($fp);
	exit();
}


function unite_export_feed_cache() {
	$path = DRUPAL_ROOT . '/sites/default/files/unite_feed_cache';
	$base_url = 'http://hotelsdb.unitehere.org/unite/export/app';
	$date = date('Ymd', strtotime('-1 day'));

	$urls = array(
		'full' => $base_url . '/' . '20010101',
		'today' => $base_url . '/' . $date,
	);


	$ctx = stream_context_create(array('http' => array('timeout' => 1000)));

	foreach ($urls as $name => $url) {
		watchdog('unite_export_feed_cache', "Generation starting: $name :: $url :: $path/$name");
		$data = file_get_contents($url, 0, $ctx);
		file_put_contents($path . '/' . $name, $data);
		watchdog('unite_export_feed_cache', "Generation complete: $name :: $url :: $path/$name");
	}
}
