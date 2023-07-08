<?php

function sirius_pcct_node_tab_provider_public_access($provider_node) {
	$result = sirius_pcct_node_access_util($provider_node->nid, 'sirius_trust_provider');
	if (!$result['success']) { return FALSE; }
	return TRUE;
}


function sirius_pcct_node_tab_provider_public($provider_node) {
	drupal_add_css(drupal_get_path('module', 'sirius_pcct') . '/css/pcct.css', 'file');	
	drupal_add_css(drupal_get_path('module', 'sirius') . '/css/table.css', 'file');	

	$services = sirius_trust_provider_service_search($provider_node);

	$service_nid = sirius_node_tab_arg(0);
	if ($service_nid) {
		$result = sirius_pcct_node_access_util($service_nid, 'sirius_trust_service');
		if (!$result['success']) { return sirius_page_error("Access denied: " . $result['msg']); }
		if (!$services[$service_nid]) { return sirius_page_error("Access denied: This provider does not offer this service."); }
		$service_node = node_load($service_nid);
	}

	$html = '';

	$lat = $provider_node->field_sirius_address_geo['und'][0]['lat'];
	$lon = $provider_node->field_sirius_address_geo['und'][0]['lon'];

	if ($lat || $lon) {
		$pin = sirius_map_pin_default();
		$map_features = array();

    $map_feature = array(
      'type' => 'point',
      'lat' => $lat,
      'lon' => $lon,
      'popup' => $popup_html,
      'marker_id'  => $provider_nid,
      'leaflet_id' => 'marker_' . $provider_nid,
      'sirius' => array(
        'marker' => array('id' => $provider_nid),
        'icon_url' => $pin['path'],
        'type' => 'provider',
      ),
      'icon' => sirius_map_pin_render_leaflet($pin),
      'pin_key' => $pin['key'],
    );

    $map_features[] = $map_feature;

		drupal_add_css(drupal_get_path('module', 'sirius_event') . '/css/survey.css', 'file');
		drupal_add_css(drupal_get_path('module', 'sirius') . '/css/map.css', 'file');
		drupal_add_js(drupal_get_path('module', 'sirius') . '/js/map.js', array('type' => 'file', 'weight' => -5));

		$js_settings = array();
		$js_settings['pins'] = sirius_map_pins();
		drupal_add_js(array('sirius_map' => $js_settings), 'setting');

		$map = sirius_map_leaflet_create();
		$map['sirius']['center']['lat'] = $lat;
		$map['sirius']['center']['lon'] = $lon;
		$map['sirius']['center']['zoom'] = 13;

	  $map_build = leaflet_build_map($map, $map_features, sirius_domain_variable_get('sirius_map_height', '400px'));
  }

  /*
	$html .= '<div class="sirius_pcct_policy_wrap">';
	$html .= sirius_pcct_policy_render();
	$html .= '</div>';
	*/

  if ($service_node) {
		$policy_node = sirius_pcct_policy();
		$policy_settings = sirius_pcct_policy_settings($policy_node);

		$rate = $services[$service_nid]['rate'];

		global $user;
		if ($user && $user->uid) {
			$result = sirius_pcct_member_lookup();
			if (!$result['success']) {
				drupal_set_message('Member lookup failed: ' . $result['msg'], 'warning');
			} else {
				$member_lookup = TRUE;
				$member_oop = $result['oop'];
				$member_deductible = $result['deductible'];
			}
		}

		// Inpatient or outpatient?
		// @todo: Abstract this into a function call
		$service_type_tid = $service_node->field_sirius_trust_service_type['und'][0]['tid'];
		$service_type_term = taxonomy_term_load($service_type_tid);
		if ($service_type_term->field_sirius_id['und'][0]['value'] == 'inpatient') {
			$is_inpatent = TRUE; 
		} else {
			$is_inpatent = FALSE;
		}

		if ($is_inpatent) {
			$copay = 0;

	  	$coinsurance_rate = $policy_settings['mbbh_inpatient_coinsurance'];
  		if (!$coinsurance_rate) { $coinsurance_rate = 0; }
  		$coinsurance_units = $policy_settings['mbbh_inpatient_coinsurance_units'];
  		if ($coinsurance_units == '%') { $coinsurance = $coinsurance_rate * $rate / 100; } else { $coinsurance = $coinsurance_rate; }
  	} else {
  		$coinsurance = 0;

	  	$copay_rate = $policy_settings['mbbh_outpatient_copay'];
  		if (!$copay_rate) { $copay_rate = 0; }
  		$copay_units = $policy_settings['mbbh_outpatient_copay_units'];
  		if ($copay_units == '%') { $copay = $copay_rate * $rate / 100; } else { $copay = $copay_rate; }
  	}

  	$you_pay = $copay + $coinsurance;
  	if ($you_pay < 0) { $you_pay = 0; }
  	if ($you_pay > $rate) { $you_pay = $rate; }

  	// If you've met the deductible, you don't pay
  	// @todo: Need to review these calculations with Joey.
		$deductible = $policy_settings['deductible'];
		if ($deductible) {
  		$deductible -= $member_deductible;
  		if ($deductible < 0) { $deductible = 0; }

  		if ($deductible < $you_pay) {
  			$they_pay += ($you_pay - $deductible);
  			$you_pay = $deductible;
  		}
  	}

  	$they_pay = $rate - $you_pay;

  	$html .= "<div class=\"sirius_pcct_cost_calculation_wrap\">";

  	$html .= '<h2>' . t('@service: Estimated Out-of-Pocket Cost', array('@service' => $service_node->title)) . '</h2>';
  	$html .= "<div class=\"sirius_pcct_cost_calculation_header\">";
  	$html .= sirius_domain_variable_get('sirius_pcct_cost_calculation_header');
  	$html .= '</div>';

  	// $copay = sirius_domain_variable_get(mbbh_outpatient_copay

  	$html .= sirius_fakerender_field('Plan Deductible:', $policy_settings['deductible_display']);
  	$html .= sirius_fakerender_field('Plan Out-of-Pocket Max:', $policy_settings['oop_display']);
  	$html .= sirius_fakerender_field('Plan Outpatient Copay:', $policy_settings['mbbh_outpatient_copay_display']);
  	$html .= sirius_fakerender_field('Plan Inpatient Coinsurance:', $policy_settings['mbbh_inpatient_coinsurance_display']);

  	if ($member_lookup) {
  		$html .= '<br />';
  		$html .= sirius_fakerender_field('Your out-of-pocket spend so far:', '<strong>' . '$' . number_format($member_oop, 2) . '</strong>');
  		$html .= sirius_fakerender_field('Your deductible spend so far:', '<strong>' . '$' . number_format($member_deductible, 2) . '</strong>');
  	}

  	$html .= '<br />';
  	$html .= sirius_fakerender_field('Estimated Total Cost:', '$' . number_format($rate, 2));
  	$html .= sirius_fakerender_field('Copay:', '$' . number_format($copay, 2));
  	$html .= sirius_fakerender_field('Co-Insurance:', '$' . number_format($coinsurance, 2));
  	$html .= sirius_fakerender_field('Your Health Plan Pays:', '$' . number_format($they_pay, 2));
  	$html .= sirius_fakerender_field('Estimated Out-of-Pocket Cost:', '<strong>' . '$' . number_format($you_pay, 2) . '</strong>');

  	$html .= "<div class=\"sirius_pcct_cost_calculation_footer\">";
  	$html .= sirius_domain_variable_get('sirius_pcct_cost_calculation_footer');
  	$html .= '</div>';

  	$html .= '</div>';
  }

	$html .= '<h2>' . $provider_node->title . '</h2>';
	if ($provider_node->field_sirius_address['und'][0]['thoroughfare']) {
  	$html .= sirius_fakerender_field('Location:', sirius_addr_format_html($provider_node->field_sirius_address['und'][0]));
  }
  if ($provider_node->field_sirius_phone['und'][0]) {
  	$html .= sirius_fakerender_field('Phone:', $provider_node->field_sirius_phone['und'][0]['value']);
  }
  if ($map_build) {
	  $html .= '<br />';
		$html .= render($map_build);
	}

	$services = sirius_trust_provider_service_search($provider_node);
	if ($services) {
		$html .= '<div class="sirius_pcct_provider_service_listwrap">';
		$html .= '<h2>' . t('Other services offered by @provider', array('@provider' => $provider_node->title)) . '</h2>';
		$rows = array();
		foreach ($services as $other_service_nid => $service_record) {

			$row = array();

			if ($other_service_nid == $service_nid) {
				$row['class'][] = 'green';
			}
			$row['data'][] = $service_record['title'];
			$row['data'][] = 
				l('View', "node/" . $provider_node->nid . "/sirius_pcct_provider_public/$other_service_nid", array('attributes' => array('class' => array('tool-button')))) . 
				l('Other Providers', "/node/$other_service_nid/sirius_pcct_service_public_list", array('attributes' => array('class' => array('tool-button'))));

			$rows[] = $row;
		}

		$header = array();
		$header[] = t('Name');
		// $header[] = t('Rate');
		$header[] = t('');

		$html .= sirius_table_render($header, $rows, array('class' => array('sirius_pcct_services')));
		$html .= '</div>';
	}

	return $html;
}

