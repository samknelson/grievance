<?php

function sirius_pcct_node_tab_service_public_map_access($service_node) {
	$result = sirius_pcct_node_access_util($service_node->nid, 'sirius_trust_service');
	if (!$result['success']) { return FALSE; }
	return TRUE;
}


function sirius_pcct_node_tab_service_public_map($service_node) {
	$service_json = sirius_json_get($service_node);

	$service_nid = $service_node->nid;

	// An ugly and cumbersome way to figure out if providers are "providers" or "facilities"
	$service_type_tid = $service_node->field_sirius_trust_service_type['und'][0]['tid'];
	$result = sirius_trust_service_type_hardcode_lookup();
	if (!$result['success']) { return sirius_page_error($result['msg']); }
	$service_type_record = array();
	foreach ($result['data'] as $key => $record) {
		if ($record['tid'] == $service_type_tid) {
			$service_type_record = $record;
			break;
		}
	}

	// Render the service description
	$description = $service_node->body['und'][0]['value'];
	if ($description) {
		$html .= '<div class="sirius_pcct_service_description_wrap">';
		$html .= $description;
		$html .= '</div>';
	}

	$pin = sirius_map_pin_default();

	$map_features = array();
	$params = array();
	$providers = sirius_trust_service_provider_search($service_node, $params);
	if (!$providers) { return '<p><em>' . t('No matching records found.') . '</em></p>'; }

	foreach ($providers as $provider_nid => $provider_record) {
		if (!$provider_record['lat'] && !$provider_record['lon']) { continue; }

		$provider_node = node_load($provider_nid);

		$popup_htmls = array();
		$popup_htmls[] = '<strong>' . $provider_node->title . '</strong>';
		if ($provider_node->field_sirius_phone['und'][0]['value']) { 
			$popup_htmls[] = $provider_node->field_sirius_phone['und'][0]['value'];
		}
		$popup_htmls[] = sirius_addr_format_html($provider_node->field_sirius_address['und'][0]);
		$popup_htmls[] = l('View', "node/$provider_nid/sirius_pcct_provider_public/$service_nid");
		$popup_html = join('<br />', $popup_htmls);

    $map_feature = array(
      'type' => 'point',
      'lat' => $provider_record['lat'],
      'lon' => $provider_record['lon'],
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
  }

	drupal_add_css(drupal_get_path('module', 'sirius_event') . '/css/survey.css', 'file');
	drupal_add_css(drupal_get_path('module', 'sirius') . '/css/map.css', 'file');
	drupal_add_js(drupal_get_path('module', 'sirius') . '/js/map.js', array('type' => 'file', 'weight' => -5));

	$js_settings = array();
	$js_settings['pins'] = sirius_map_pins();
	drupal_add_js(array('sirius_map' => $js_settings), 'setting');

	$map = sirius_map_leaflet_create();
	$map['sirius']['center']['lat'] = sirius_domain_variable_get('sirius_pcct_map_default_center_lat');
	$map['sirius']['center']['lon'] = sirius_domain_variable_get('sirius_pcct_map_default_center_lon');
	$map['sirius']['center']['zoom'] = sirius_domain_variable_get('sirius_pcct_map_default_zoom', 13);

  $map_build = leaflet_build_map($map, $map_features, sirius_domain_variable_get('sirius_map_height', '600px'));
	$html .= render($map_build);
	//$html .= l('Legend', 'sirius/modal/event/map-legend/' . $event_node->nid, array('attributes' => array('class' => array('tool-button'))));

	return $html;
}

