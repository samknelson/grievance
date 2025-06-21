<?php

function unite_export_union_hotel_guide_access() {
	// return sirius_ip_check(sirius_loopback_remote_ip(), variable_get('unite_export_ip_allowlist'));
	return TRUE;
}

function unite_export_union_hotel_guide($secs = 0) {
	if (!$secs) { $secs = $_REQUEST["secs"]; }

	$lookup_airports = unite_airports_list();
	$lookup_convention = unite_conventions_list();
	$lookup_brand = sirius_taxonomy_load('brand', 'tid', 'name');
	$lookup_niche = sirius_taxonomy_load('hdb_niche', 'tid', 'name');

	$lookup_brand_full = sirius_taxonomy_load('brand', 'tid', 'full');

	$sql_args = array();

	$fields = array(
		'nid' => 'nid',
		'title' => 'title',
		'field_address_administrative_area' => 'address - state',
		'field_address_country' => 'address - country',
		'field_address_locality' => 'address - city',
		'field_address_postal_code' => 'address - zip',
		'field_address_premise' => 'address - premise',
		'field_address_thoroughfare' => 'address - street',
		'field_development_status_value' => 'development_status',
		'field_phone_value' => 'phone',
		'field_fax_value' => 'fax',
		'field_rooms_value' => 'rooms',
		'field_union_status_value' => 'union_status',
		'field_url_url' => 'url',
		'field_address_geofield_lat' => 'latitude',
		'field_address_geofield_lon' => 'longitude',
		'field_labor_activity_index_value' => 'labor_activity_index',
		'field_boycott_url_url' => 'boycott_url',
		'field_fh_notes_value' => 'fh_notes',
		'field_fh_nonunion_value' => 'fh_nonunion',
		'field_airport_target_id' => 'airport',
		'field_calc_airport_dist_value' => 'airport_dist',
		'field_airport_alt_target_id' => 'airport_2',
		'field_calc_airport_dist_alt_value' => 'airport_2_dist',
		'field_convention_center_target_id' => 'convention_center',
		'field_calc_convention_dist_value' => 'convention_center_dist',
		'field_convention_center_attached_value' => 'convention_center_attached',
		'field_fh_meeting_room_count_value' => 'meeting_room_count',
		'field_fh_meeting_room_capacity_value' => 'meeting_room_capacity',
		'field_fh_meeting_room_largest_value' => 'meeting_room_largest',
		'field_fh_event_space_value' => 'event_space',
		'field_fh_ballroom_count_value' => 'ballroom_count',
		'field_fh_ballroom_capacity_value' => 'ballroom_capacity',
		'field_fh_outdoor_value' => 'outdoor_space_available',
		'field_fh_outdoor_area_value' => 'outdoor_space',
		'field_fh_parking_value' => 'parking_available',
		'field_fh_parking_capacity_value' => 'parking_capacity',
		'field_brand_tid' => 'brand',
		'field_niche_tid' => 'niche',
	);

	$sql = "select ";
	$sql .= join(', ', array_keys($fields));
	$sql .= " from node ";

	$tables = array(
		'field_labor_activity_index',
		'field_address',
		'field_development_status',
		'field_fax',
		'field_phone',
		'field_rooms',
		'field_union_status',
		'field_url',
		'field_address_geofield',
		'field_boycott_url',
		'field_fh_notes',
		'field_fh_nonunion',
		'field_airport',
		'field_calc_airport_dist',
		'field_airport_alt',
		'field_calc_airport_dist_alt',
		'field_convention_center',
		'field_calc_convention_dist',
		'field_convention_center_attached',
		'field_fh_meeting_room_count',
		'field_fh_meeting_room_capacity',
		'field_fh_meeting_room_largest',
		'field_fh_event_space',
		'field_fh_ballroom_count',
		'field_fh_ballroom_capacity',
		'field_fh_outdoor',
		'field_fh_outdoor_area',
		'field_fh_parking',
		'field_fh_parking_capacity',
		'field_brand',
		'field_niche',
	);
	foreach ($tables as $table) {
		$sql .= "left join field_data_$table on field_data_$table.entity_type = 'node' and field_data_$table.entity_id = node.nid ";
	}
	$sql .= "where node.type = 'property' ";
	$sql .= "and field_labor_activity_index_value is not null ";
	$sql .= "and field_labor_activity_index_value > 0 ";
	if ($secs) {
		$sql .= "and changed >= :ts ";
		$sql_args[':ts'] = time() - $secs;
	}
	// $sql .= "and nid = 37266 ";
	$sql .= "order by nid ";
	$stmt = db_query($sql, $sql_args);

	if ($_REQUEST['onscreen']) {
		print "<pre>";
	} else {
		drupal_add_http_header('Content-Type', 'text/csv');
	}
	$fp = fopen('php://output', 'w');

	$header = array_values($fields);
	$header[] = 'brand_parent';
	fputcsv($fp, $header);

	$count = 0;
	while ($hr = $stmt->fetchAssoc()) {
		$row = array();
		$delta = 0;

		$hr['field_airport_target_id'] = $lookup_airports[$hr['field_airport_target_id']];
		$hr['field_airport_alt_target_id'] = $lookup_airports[$hr['field_airport_alt_target_id']];
		$hr['field_convention_center_target_id'] = $lookup_convention[$hr['field_convention_center_target_id']];
		$brand_term = $lookup_brand_full[$hr['field_brand_tid']];
		$hr['field_brand_tid'] = $lookup_brand[$hr['field_brand_tid']];
		$hr['field_niche_tid'] = $lookup_niche[$hr['field_niche_tid']];

		$brand_parent_name = '';
		if ($brand_term) {
			$brand_parent_tid = $brand_term->parents[0];
			if ($brand_parent_tid) {
				$brand_parent_name = $lookup_brand[$brand_parent_tid];
			}
		}
		$hr['brand_parent'] = $brand_parent_name;

		// print_r($row);
		fputcsv($fp, array_values($hr));
		++$count;
	}

	fclose($fp);
	// print "Found: $count\n";
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
		print "Here with $url\n";
		watchdog('unite_export_feed_cache', "Generation starting: $name :: $url :: $path/$name");
		$data = file_get_contents($url, 0, $ctx);
		file_put_contents($path . '/' . $name, $data);
		watchdog('unite_export_feed_cache', "Generation complete: $name :: $url :: $path/$name");
	}
}
