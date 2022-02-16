<?php

function unite_airports_node_presave($property_node) {
	foreach (array('field_airport' => 'field_calc_airport_dist', 'field_airport_alt' => 'field_calc_airport_dist_alt') as $source_field => $target_field) {
		$airport_node = node_load($property_node->{$source_field}['und'][0]['target_id']);
		$lat1 = $property_node->field_address_geofield['und'][0]['lat'];
		$lon1 = $property_node->field_address_geofield['und'][0]['lon'];

		$lat2 = $airport_node->field_address_geofield['und'][0]['lat'];
		$lon2 = $airport_node->field_address_geofield['und'][0]['lon'];

		if (isset($lat1) && isset($lat2)) {
			$property_node->{$target_field}['und'][0]['value'] = sirius_addr_latlon_distance($lat1, $lon1, $lat2, $lon2);
		} else {
			unset($property_node->{$target_field}['und']);
		}
	}
}
