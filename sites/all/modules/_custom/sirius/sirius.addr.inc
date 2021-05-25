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
	if (!$addr['thoroughfare'] || !$addr['locality'] || !$addr['administrative_area'] || !$addr['postal_code']) {
		return array('success' => FALSE, 'msg' => "Please enter an address, city, state, and zip.");
	}

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
	if (($domain_nid < 1) || sirius_domain_component_enabled('mapping')) {
		$geocoder_settings = variable_get('geocoder_settings', array());
		if ($geocoder_settings['geocoder_apikey_google']) {
			$txt = sirius_addr_format_oneline($addr);
			$point = geocoder('google', $txt);
			if ($point) {
				if (!($point->data['geocoder_formatted_address']) || ($point->data['geocoder_accuracy'] == 'APPROXIMATE')) {
					return array('success' => FALSE, 'msg' => 'The mapping service was unable to find a location for that address.');
				}
			}
		}
	}

	return array('success' => TRUE);
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

