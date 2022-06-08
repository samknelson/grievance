<?php

function sirius_addr_states() {
	$states = array(
	    'AL'=>'Alabama',
	    'AK'=>'Alaska',
	    'AZ'=>'Arizona',
	    'AR'=>'Arkansas',
	    'CA'=>'California',
	    'CO'=>'Colorado',
	    'CT'=>'Connecticut',
	    'DE'=>'Delaware',
	    'DC'=>'District of Columbia',
	    'FL'=>'Florida',
	    'GA'=>'Georgia',
	    'HI'=>'Hawaii',
	    'ID'=>'Idaho',
	    'IL'=>'Illinois',
	    'IN'=>'Indiana',
	    'IA'=>'Iowa',
	    'KS'=>'Kansas',
	    'KY'=>'Kentucky',
	    'LA'=>'Louisiana',
	    'ME'=>'Maine',
	    'MD'=>'Maryland',
	    'MA'=>'Massachusetts',
	    'MI'=>'Michigan',
	    'MN'=>'Minnesota',
	    'MS'=>'Mississippi',
	    'MO'=>'Missouri',
	    'MT'=>'Montana',
	    'NE'=>'Nebraska',
	    'NV'=>'Nevada',
	    'NH'=>'New Hampshire',
	    'NJ'=>'New Jersey',
	    'NM'=>'New Mexico',
	    'NY'=>'New York',
	    'NC'=>'North Carolina',
	    'ND'=>'North Dakota',
	    'OH'=>'Ohio',
	    'OK'=>'Oklahoma',
	    'OR'=>'Oregon',
	    'PA'=>'Pennsylvania',
	    'RI'=>'Rhode Island',
	    'SC'=>'South Carolina',
	    'SD'=>'South Dakota',
	    'TN'=>'Tennessee',
	    'TX'=>'Texas',
	    'UT'=>'Utah',
	    'VT'=>'Vermont',
	    'VA'=>'Virginia',
	    'WA'=>'Washington',
	    'WV'=>'West Virginia',
	    'WI'=>'Wisconsin',
	    'WY'=>'Wyoming',
	);

	return $states;
}

function sirius_addr_validate_full($addr) {
	$result = sirius_addr_geocode_enabled();
	if (!$result['success']) { 
		return array('success' => TRUE, 'msg' => 'Skipping validation because geocoding is not enabled: ' . $result['msg']);
	}

	if (is_array($addr)) {
		if (!$addr['thoroughfare'] || !$addr['locality'] || !$addr['administrative_area'] || !$addr['postal_code']) {
			return array('success' => FALSE, 'msg' => "Please enter an address, city, state, and zip.");
		}
		$oneline = sirius_addr_format_oneline($addr);
	} else {
		$oneline = $addr;
	}

	return sirius_addr_geocode_impl($oneline);
}

function sirius_addr_geocode_enabled() {
	// 
	// @todo: We gocode if:
	// - Either
	// - - We have a domain and the mapping component is enabled, OR
	// - - We are in the root domain
	// - AND there is a Google Maps API key.
	// Really this is fairly ugly. We should never be in the root domain, and we should make the geocode service a variable.
	//    (see $handlers = ctools_get_plugins('geocoder', 'geocoder_handler'); )
	// 

	$domain_nid = sirius_domain_nid();
	if ($domain_nid > 1 && !sirius_domain_component_enabled('mapping')) {
		return array('success' => FALSE, 'msg' => 'Mapping is not enabled for this domain.');
	}
	$geocoder_settings = variable_get('geocoder_settings', array());
	if (!$geocoder_settings['geocoder_apikey_google']) {
		return array('success' => FALSE, 'msg' => 'No Google Maps API Key has been set.');
	}
	return array('success' => TRUE);
}

function sirius_addr_geocode_impl($addr_oneline) {
	$result = sirius_addr_geocode_enabled();
	if (!$result['success']) { return $result; }

	$point = geocoder('google', $addr_oneline);
	if (!$point) {
		return array('success' => FALSE, 'msg' => 'The mapping service was unable to find a location for that address.');
	}
	if (!($point->data['geocoder_formatted_address']) || ($point->data['geocoder_accuracy'] == 'APPROXIMATE')) {
		return array('success' => FALSE, 'msg' => 'The mapping service was unable to find an exact location for that address.', 'point' => $point);
	}

	return array('success' => TRUE, 'point' => $point);
}

function sirius_addr_validate($addr) {
	$result = sirius_addr_validate_full($addr);
	return $result['success'];
}

function sirius_addr_format_oneline($addr) {
	// 123 Fake St., Apt 11, Beacon, NY 12508	
	if (!$addr) { return ''; }
	$txt = $addr['thoroughfare'] . ', ';
	if ($addr['premise']) { $txt .= $addr['premise'] . ', '; }
	$txt .= $addr['locality'] . ', ';
	$txt .= $addr['administrative_area'] . ' ' . $addr['postal_code'];
	return $txt;
}


function sirius_addr_format_html($addr) {
	// 123 Fake St.<br />Apt 11<br />Beacon, NY 12508	
	if (!$addr) { return ''; }
	$txt = '<div class="sirius_addr">';
	if ($addr['thoroughfare']) 				{ $txt = '<span class="sirius_addr_thoroughfare">' . $addr['thoroughfare'] . '</span><br />'; }
	if ($addr['premise']) 						{ $txt .= '<span class="sirius_addr_premise">' . $addr['premise'] . '</span>' . '<br />'; }
	if ($addr['locality']) 						{ $txt .= '<span class="sirius_addr_locality">' . $addr['locality'] . '</span>, '; }
	if ($addr['administrative_area']) { $txt .= '<span class="sirius_addr_administrative_area">' . $addr['administrative_area'] . '</span> '; }
	if ($addr['postal_code'])					{ $txt .= '<span class="sirius_addr_postal_code">' . $addr['postal_code'] . '</span>'; }
	return $txt;
}

function sirius_geofield_enabled($entity_type, $entity, $field = NULL, $instance = NULL) {
	if ($entity_type != 'node') { return TRUE; }
	$domain_nid = $entity->field_sirius_domain['und'][0]['target_id'];
	if (!$domain_nid) { return TRUE; }
	return sirius_domain_component_enabled('mapping', $domain_nid);
}

function sirius_addr_node_presave($node) {
	$field_name = 'field_address';
	if (isset($node->{$field_name})) {
		$entity_type = 'node';
		$entity = $node;

		$field_instance = field_info_instance('node', 'field_address_geofield', $node->type);
    $handler = geocoder_get_handler($field_instance['widget']['settings']['geocoder_handler']);
    $target_info = field_info_field($field_instance['field_name']);
    $field_info = geocoder_widget_get_field_info($entity_type, $field_instance, $entity);
    $items = geocoder_widget_get_entity_field_value($entity_type, $field_instance, $entity);
    $item = $items[0];

    // Get the handler-specific-settings.
    if (isset($field_instance['widget']['settings']['handler_settings'][$handler['name']])) {
      $handler_settings = $field_instance['widget']['settings']['handler_settings'][$handler['name']];
    }
    else {
      $handler_settings = array();
    }
    $geometry = geocoder_cache_get($handler['name'], $item, $handler_settings);

    if ($geometry && ($geometry->data['geocoder_accuracy'] == 'ROOFTOP') && $geometry->data['geocoder_formatted_address']) {
    	$node->field_address_canon['und'][0]['value'] = $geometry->data['geocoder_formatted_address'];
    } else {
	    $node->field_address_canon['und'][0]['value'] = sirius_addr_format_oneline($item);
    }

    if ($geometry) {
	    $json = sirius_json_get($node);
	    $json['addr'][$field_name]['geometry'] = $geometry;
	    sirius_json_set($node, $json);
	    // dpm($json);
	  }
	}
}

function sirius_addr_form_alter_node(&$form, &$form_state, $form_id) {
	// Hide canonical addresses on the node form. They are always set automatically.
	if (isset($form['field_address_canon'])) { $form['field_address_canon']['#access'] = FALSE; }
}

function sirius_addr_get_compponent($point, $type) {
	if (!$point) { return NULL; }
	if (!$point->data) { return NULL; }
	if (!$point->data['geocoder_address_components']) { return NULL; }
	foreach ($point->data['geocoder_address_components'] as $component) {
		if (!in_array($type, $component->types)) { continue; }
		return $component->long_name;
	}
}

function sirius_addr_latlon_distance($lat1, $lon1, $lat2, $lon2, $unit = 'M') {
  if (($lat1 == $lat2) && ($lon1 == $lon2)) {
    return 0;
  }

  $theta = $lon1 - $lon2;
  $dist = sin(deg2rad($lat1)) * sin(deg2rad($lat2)) +  cos(deg2rad($lat1)) * cos(deg2rad($lat2)) * cos(deg2rad($theta));
  $dist = acos($dist);
  $dist = rad2deg($dist);
  $miles = $dist * 60 * 1.1515;
  $unit = strtoupper($unit);

  if ($unit == "K") {
    return ($miles * 1.609344);
  } else if ($unit == "N") {
    return ($miles * 0.8684);
  } else {
    return $miles;
  }
}

function sirius_addr_driving_distance($addr1, $addr2, $options = array()) {
  // Get a Google API key
  $geocoder_settings = variable_get('geocoder_settings', array());
  $google_api_key = $geocoder_settings['geocoder_apikey_google'];
  if (!$google_api_key) {
    return array('success' => FALSE, 'msg' => "Please make sure that the geocoder module is configured with a Google API key.");
  }

  $units = $options['units'];
  if (!$units) { $units = 'imperial'; }

  $total_distance = 0;

  // Get directions from google directions api
  $url = 'https://maps.googleapis.com/maps/api/distancematrix/json';
  $options = array(
    'query' => array(
      'origins' => sirius_addr_format_oneline($addr1),
      'destinations' => sirius_addr_format_oneline($addr2),
      // 'waypoints' => implode('|', $waypoints),
      'units' => $units,
      'key' => $google_api_key
    )
  );
  $full_url = url($url, $options);
  $response = drupal_http_request($full_url);
  if (!$response->data) {
    return array('success' => FALSE, 'msg' => "Unable to connect to $url: " . $response->error);
  }

  $result = json_decode($response->data);
  if ($result->error_message) {
    return array('success' => FALSE, 'msg' => $result->error_message);
  }
  if ($result->status != 'OK') {
    return array('success' => FALSE, 'msg' => "Google API returned status " . $result->status); 
  }

  if (!$result->rows[0]) {
    return array('success' => FALSE, 'msg' => "Unable to parse the Google API response."); 
  }
  $meters = $result->rows[0]->elements[0]->distance->value;
  $miles = $meters / 1609.34;
  return array('success' => TRUE, 'distance' => $miles);
}
