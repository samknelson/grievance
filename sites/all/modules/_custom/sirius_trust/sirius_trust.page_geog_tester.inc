<?php

function sirius_trust_page_geog_tester() {
	return drupal_get_form('sirius_trust_page_geog_tester_form');
}

function sirius_trust_page_geog_tester_form($form, &$form_state) {
	// Find all the possible plugins. This is some ugly code
	$options = array();
	$all_settings = array();

	$policies = sirius_ledger_policy_list();
	foreach ($policies as $policy_nid => $policy_node) {
		$benefits = sirius_trust_policy_benefits_offered($policy_node);
		foreach ($benefits as $benefit_nid => $benefit_node) {
			$enabled_plugins = sirius_trust_eligibility_plugins_enabled($policy_node, $benefit_node);
			foreach ($enabled_plugins as $plugin_key) {
				$plugin_info = sirius_trust_eligibility_plugin_info($plugin_key);
				if (!is_subclass_of($plugin_info['instance'], 'Sirius_Trust_Elgibility_Plugin_Geog')) {
					continue;
				}

				$policy_json = sirius_json_get($policy_node);
				$settings = $policy_json['benefits'][$benefit_nid]['eligibility']['plugin'][$plugin_key]['settings'];

				$key = "$policy_nid|$benefit_nid";
				$all_options[$key] = $policy_node->title . ': ' . $benefit_node->title;
				$all_settings[$key] = $settings;
			}
		}
	}

	if (!$all_options) { return sirius_form_error("There are no geography plugins enabled."); }

	$form['which'] = array(
		'#type' => 'select',
		'#title' => 'Which Benefit?',
		'#options' => array(NULL => t('-- Select --')) + $all_options,
		'#required' => TRUE,
	);

	$form['addr'] = array(
		'#type' => 'textfield',
		'#title' => t('Address to Test'),
		'#required' => TRUE,
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	if ($form_state['submitted']) {
		$which = $form_state['values']['which'];
		$settings = $all_settings[$which];

		$addr = $form_state['values']['addr'];
		$result = sirius_addr_geocode_impl($addr);
		if (!$result['success']) {
			drupal_set_message("Unable to geocode: " . $result['msg'], 'warning');
			return $form;
		}

		$worker_lat = $result['point']->coords[1];
		$worker_lon = $result['point']->coords[0];

		$pin_lookup = sirius_map_pins();

		$msg .= '<h2>Input</h2>';
		$msg .= sirius_fakerender_field("Input Address:", $result['point']->data['geocoder_formatted_address']);
		$msg .= sirius_fakerender_field("Input Accuracy:", $result['point']->data['geocoder_accuracy']);

		$current_id = 100;
		$map_features = array();

		$pin_key = 'bulb-blue';
		$pin = $pin_lookup[$pin_key];
		$marker_id = $current_id++;
    $map_feature = array(
      'type' => 'point',
      'lat' => $worker_lat,
      'lon' => $worker_lon,
      'popup' => 'Worker Location: ' . $result['point']->data['geocoder_formatted_address'],
      'marker_id'  => $marker_id,
      'leaflet_id' => 'marker_1',
      'sirius' => array(
        'marker' => array('id' => $marker_id),
        'icon_url' => $pin['path'],
      ),
      'icon' => sirius_map_pin_render_leaflet($pin),
      'pin_key' => 'bulb-blue',
    );
    $map_features[] = $map_feature;

		foreach ($settings['addresses'] as $delta => $record) {
			$probe_addr = $record['address'];

			if (!$record['lat'] && !$record['lon']) { continue; }
			$distance = round(sirius_addr_latlon_distance($worker_lat, $worker_lon, $record['lat'], $record['lon']), 4);
			$radius = $record['radius'];

			$msg .= '<h2>Address #' . ($delta+1) . '</h2>';
			$msg .= sirius_fakerender_field("Service Address:", $probe_addr);
			$msg .= sirius_fakerender_field("Service Radius:", $record['radius'] . " miles");
			$msg .= sirius_fakerender_field("Computed Distance:", number_format($distance, 2) . " miles");

			$pin_key = 'bulb-green';
			$pin = $pin_lookup[$pin_key];
			$marker_id = $current_id++;
	    $map_feature = array(
	      'type' => 'point',
	      'lat' => $record['lat'],
	      'lon' => $record['lon'],
	      'popup' => 'Service Location: ' . $record['address'],
	      'marker_id'  => $marker_id,
	      'leaflet_id' => 'marker_' . $marker_id,
	      'sirius' => array(
	        'marker' => array('id' => $marker_id),
	        'icon_url' => $pin['path'],
	        'type' => 'contact',
	      ),
	      'icon' => sirius_map_pin_render_leaflet($pin),
	      'pin_key' => 'bulb-green',
	    );
	    $map_features[] = $map_feature;


	    $map_feature = array(
	      'type' => 'circle',
	      'lat' => $record['lat'],
	      'lon' => $record['lon'],
	      'radius' => '5000',
	      'marker_id'  => $marker_id+1000,
	      'leaflet_id' => 'marker_' . ($marker_id+1000),
	      'icon' => sirius_map_pin_render_leaflet($pin),
	      'pin_key' => 'bulb-red',
	    );
	    $map_features[] = $map_feature;

		}

		// Get map implementation provided by http://drupal.org/project/leaflet_googlemaps.
		drupal_add_css(drupal_get_path('module', 'sirius') . '/css/map.css', 'file');
		drupal_add_js(drupal_get_path('module', 'sirius') . '/js/map.js', array('type' => 'file', 'weight' => -5));
		$js_settings = array();
		$js_settings['pins'] = $pin_lookup;
		drupal_add_js(array('sirius_map' => $js_settings), 'setting');
		$map = sirius_map_leaflet_create();
	  $map_build = leaflet_build_map($map, $map_features, sirius_domain_variable_get('sirius_map_height', '600px'));
		$msg .= render($map_build);
		$msg .= l('Legend', 'sirius/modal/event/map-legend/' . $event_node->nid, array('attributes' => array('class' => array('tool-button'))));
		$msg .= '<a href="/sirius/map" id="sirius_map_poll" class="tool-button">Refresh Now</a>';
		$msg .= '<div id="sirius_map_poll_message"></div>';

		$form['results'] = array('#markup' => '<hr>' . $msg);
	}

  return $form;
}


function sirius_trust_page_geog_tester_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;
}
