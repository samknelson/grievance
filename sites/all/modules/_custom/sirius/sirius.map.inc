<?php

function sirius_map_leaflet_create($settings = array()) {
	if (!$settings) {
		$settings['zoom'] = 16;
		$settings['fullscreenControl'] = TRUE;
		$settings['tap'] = FALSE; // See https://github.com/Leaflet/Leaflet/issues/7255
		// settings['fullscreenControlOptions'] = array('positio' => 'topleft');
		// $settings['layerControl'] = TRUE;
	}

	drupal_add_css('sites/all/libraries/leaflet_fullscreen/dist/leaflet.fullscreen.css');
 	drupal_add_js('sites/all/libraries/leaflet_fullscreen/dist/Leaflet.fullscreen.min.js');
 	// drupal_add_js('sites/all/libraries/leaflet_print/dist/bundle.js');

 	// Fetch our map
	$map = leaflet_map_get_info(sirius_domain_variable_get('sirius_map_provider', 'OSM Mapnik'));

 	// Set "maxNativeZoom" and "maxZoom".
 	// Drupal makes this oddly weird. The variables are represented in different places in different map types
 	// - see leaflet_more_maps.js line 5. Weird.
 	// We also don't know which layer contains the tile, so we're guessing it's the one that contains a "urlTemplate".
 	// HACKY.

 	$max_native_zoom = sirius_domain_variable_get('sirius_map_max_native_zoom', 19);
 	$max_zoom = sirius_domain_variable_get('sirius_map_max_zoom', 22);
	foreach ($map['layers'] as $key => $value) {
		if (!$map['layers'][$key]['urlTemplate']) { continue; }

		if (!$map['layers'][$key]['options']['maxNativeZoom']) { $map['layers'][$key]['options']['maxNativeZoom'] = $max_native_zoom; }
		if (!$map['layers'][$key]['options']['maxZoom']) { $map['layers'][$key]['options']['maxZoom'] = $max_zoom; }

		// if (!$map['layers'][$key]['maxNativeZoom']) { $map['layers'][$key]['maxNativeZoom'] = $max_native_zoom; }
		// if (!$map['layers'][$key]['maxZoom']) { $map['layers'][$key]['maxZoom'] = $max_zoom; }

		/*
		if ($map['layers'][$key]['type'] == 'google' || true) {
			if (!$map['layers'][$key]['options']['maxNativeZoom']) { $map['layers'][$key]['options']['maxNativeZoom'] = $max_native_zoom; }
			if (!$map['layers'][$key]['options']['maxZoom']) { $map['layers'][$key]['options']['maxZoom'] = $max_zoom; }
		} else {
			if (!$map['layers'][$key]['maxNativeZoom']) { $map['layers'][$key]['maxNativeZoom'] = $max_native_zoom; }
			if (!$map['layers'][$key]['maxZoom']) { $map['layers'][$key]['maxZoom'] = $max_zoom; }
		}
		*/
	}
	if (!$settings['maxZoom']) { $settings['maxZoom'] = $max_zoom; }
	if (!$settings['maxNativeZoom']) { $settings['maxNativeZoom'] = $max_native_zoom; }

	$map['settings'] = $settings;
	return $map;
}

function sirius_map_admin($form, &$form_state) {
	$providers = leaflet_map_get_info();
	$provider_options = array();
	foreach ($providers as $key => $record) {
		$provider_options[$key] = $record['label'];
	}

  $name = sirius_domain_variable_name('sirius_map_provider');
  $form[$name] = array(
    '#type' => 'select',
    '#title' => t('Map Provider'),
    '#options' => $provider_options,
    '#default_value' => variable_get($name, 'OSM Mapnik'),
  );

  $name = sirius_domain_variable_name('sirius_map_height');
  $form[$name] = array(
    '#type' => 'textfield',
    '#title' => t('Map Height'),
    '#default_value' => variable_get($name, '600px'),
    '#description' => t('E.g. "600px".'),
  );

  $name = sirius_domain_variable_name('sirius_map_current_location_pin');
  $form[$name] = sirius_map_pin_picker(variable_get($name, 'dot-blue'));

  return system_settings_form($form);
}

function sirius_map_current_location_checkin() {
	global $user;
	if (!$user) {	drupal_json_output(array('success' => FALSE, 'msg' => 'No User')); exit(); }
	$user_full = user_load($user->uid);

	if (!$_REQUEST['location']['lat'] && !$_REQUEST['location']['lon']) { drupal_json_output(array('success' => FALSE, 'msg' => 'No lat / lon.')); exit(); }

	$json = sirius_jsonfield_util_user_json_get($user_full);
	$json['current_location'] = array(
		'checkin' => array(
			'ts' => time(),
			'lat' => $_REQUEST['location']['lat'],
			'lon' => $_REQUEST['location']['lon'],
		),
	);
	sirius_jsonfield_util_user_json_set($user_full, $json);
	user_save($user_full);

	drupal_json_output(array('success' => TRUE, 'msg' => 'Checkin complete.')); 
	exit();	
}

function sirius_user_tab_map_current_location_access($map_uid) {
	// Current location check must be enabled
	if (!sirius_domain_component_enabled('mapping:checkin')) { return FALSE; }

	// Admins can see anybody
	if (user_access('sirius admin')) { return TRUE; }

	// Must be domain admin
	if (!user_access('sirius domain admin')) { return FALSE; }

	// Must be in the current domain
	$map_user = user_load($map_uid);
	if ($map_user_full->field_sirius_domain['und'][0]['target_id'] != sirius_domain_nid()) { return FALSE; }

	// Ok then
	return TRUE;
}

function sirius_user_tab_map_current_location($map_uid) {
	drupal_add_css(drupal_get_path('module', 'sirius_event') . '/css/survey.css', 'file');
	drupal_add_js(drupal_get_path('module', 'sirius') . '/js/map.js', array('type' => 'file', 'weight' => -5));

	$map_user = user_load($map_uid);
	$json = sirius_jsonfield_util_user_json_get($map_user);
	$json_checkin = $json['current_location']['checkin'];
	if (!$json_checkin) {
		return t('This user has never checked in. Location data is not available.');
	}

	$js_settings = array();
	drupal_add_js(array('sirius_map' => $js_settings), 'setting');

	global $user;
	$event_nid = $event_node->nid;
	$event_json = sirius_jsonfield_util_node_json_get($event_node);
	$type_json = sirius_event_type_json($event_node);
	$survey_node = sirius_event_survey_get($event_node, 'map');
	$survey_json = sirius_jsonfield_util_node_json_get($survey_node);
	$survey_form = sirius_jsonfield_util_form_get($survey_json, 'default');

	$popup_html = $map_user->name . " last checked in at <strong>" . date('l, F j, Y g:i:s A', $json_checkin['ts']) . '</strong> (' . format_interval(time() - $json_checkin['ts']) . ' ago).';
	$pin = sirius_map_pin_default();
	$map_feature = array(
	  'type' => 'point',
	  'lat' => $json_checkin['lat'],
	  'lon' => $json_checkin['lon'],
	  'popup' => $popup_html,
    'icon' => sirius_map_pin_render_leaflet($pin),
		'icon_name' => 'Blue',
	);
	$map_features[] = $map_feature;

	// Get map implementation provided by http://drupal.org/project/leaflet_googlemaps.
	$html = $popup_html;
	$html .= " The last location is shown on the map below (latitude " . round($json_checkin['lat'], 10) . ", longitude " . round($json_checkin['lon'], 10) . ').<br /><br />';
	$map = sirius_map_leaflet_create();
  $map_build = leaflet_build_map($map, $map_features, sirius_domain_variable_get('sirius_map_height', '600px'));
	$html .= render($map_build);

	return $html;
}


function sirius_map_pins() {
	$pins = array();
	$pins['bulb-black'] = array('title' => 'bulb-black', 'file' => 'bulb-black.png', 'path' => '/sites/all/modules/_custom/sirius/images/map/pins/bulb-black.png');
	$pins['bulb-blue'] = array('title' => 'bulb-blue', 'file' => 'bulb-blue.png', 'path' => '/sites/all/modules/_custom/sirius/images/map/pins/bulb-blue.png');
	$pins['bulb-brown'] = array('title' => 'bulb-brown', 'file' => 'bulb-brown.png', 'path' => '/sites/all/modules/_custom/sirius/images/map/pins/bulb-brown.png');
	$pins['bulb-gray'] = array('title' => 'bulb-gray', 'file' => 'bulb-gray.png', 'path' => '/sites/all/modules/_custom/sirius/images/map/pins/bulb-gray.png');
	$pins['bulb-green'] = array('title' => 'bulb-green', 'file' => 'bulb-green.png', 'path' => '/sites/all/modules/_custom/sirius/images/map/pins/bulb-green.png');
	$pins['bulb-lime'] = array('title' => 'bulb-lime', 'file' => 'bulb-lime.png', 'path' => '/sites/all/modules/_custom/sirius/images/map/pins/bulb-lime.png');
	$pins['bulb-orange'] = array('title' => 'bulb-orange', 'file' => 'bulb-orange.png', 'path' => '/sites/all/modules/_custom/sirius/images/map/pins/bulb-orange.png');
	$pins['bulb-pink'] = array('title' => 'bulb-pink', 'file' => 'bulb-pink.png', 'path' => '/sites/all/modules/_custom/sirius/images/map/pins/bulb-pink.png');
	$pins['bulb-purple'] = array('title' => 'bulb-purple', 'file' => 'bulb-purple.png', 'path' => '/sites/all/modules/_custom/sirius/images/map/pins/bulb-purple.png');
	$pins['bulb-red'] = array('title' => 'bulb-red', 'file' => 'bulb-red.png', 'path' => '/sites/all/modules/_custom/sirius/images/map/pins/bulb-red.png');
	$pins['bulb-teal'] = array('title' => 'bulb-teal', 'file' => 'bulb-teal.png', 'path' => '/sites/all/modules/_custom/sirius/images/map/pins/bulb-teal.png');
	$pins['bulb-white'] = array('title' => 'bulb-white', 'file' => 'bulb-white.png', 'path' => '/sites/all/modules/_custom/sirius/images/map/pins/bulb-white.png');
	$pins['bulb-yellow'] = array('title' => 'bulb-yellow', 'file' => 'bulb-yellow.png', 'path' => '/sites/all/modules/_custom/sirius/images/map/pins/bulb-yellow.png');
	$pins['dot-black'] = array('title' => 'dot-black', 'file' => 'dot-black.png', 'path' => '/sites/all/modules/_custom/sirius/images/map/pins/dot-black.png');
	$pins['dot-blue'] = array('title' => 'dot-blue', 'file' => 'dot-blue.png', 'path' => '/sites/all/modules/_custom/sirius/images/map/pins/dot-blue.png');
	$pins['dot-brown'] = array('title' => 'dot-brown', 'file' => 'dot-brown.png', 'path' => '/sites/all/modules/_custom/sirius/images/map/pins/dot-brown.png');
	$pins['dot-gray'] = array('title' => 'dot-gray', 'file' => 'dot-gray.png', 'path' => '/sites/all/modules/_custom/sirius/images/map/pins/dot-gray.png');
	$pins['dot-green'] = array('title' => 'dot-green', 'file' => 'dot-green.png', 'path' => '/sites/all/modules/_custom/sirius/images/map/pins/dot-green.png');
	$pins['dot-lime'] = array('title' => 'dot-lime', 'file' => 'dot-lime.png', 'path' => '/sites/all/modules/_custom/sirius/images/map/pins/dot-lime.png');
	$pins['dot-orange'] = array('title' => 'dot-orange', 'file' => 'dot-orange.png', 'path' => '/sites/all/modules/_custom/sirius/images/map/pins/dot-orange.png');
	$pins['dot-pink'] = array('title' => 'dot-pink', 'file' => 'dot-pink.png', 'path' => '/sites/all/modules/_custom/sirius/images/map/pins/dot-pink.png');
	$pins['dot-purple'] = array('title' => 'dot-purple', 'file' => 'dot-purple.png', 'path' => '/sites/all/modules/_custom/sirius/images/map/pins/dot-purple.png');
	$pins['dot-red'] = array('title' => 'dot-red', 'file' => 'dot-red.png', 'path' => '/sites/all/modules/_custom/sirius/images/map/pins/dot-red.png');
	$pins['dot-teal'] = array('title' => 'dot-teal', 'file' => 'dot-teal.png', 'path' => '/sites/all/modules/_custom/sirius/images/map/pins/dot-teal.png');
	$pins['dot-white'] = array('title' => 'dot-white', 'file' => 'dot-white.png', 'path' => '/sites/all/modules/_custom/sirius/images/map/pins/dot-white.png');
	$pins['dot-yellow'] = array('title' => 'dot-yellow', 'file' => 'dot-yellow.png', 'path' => '/sites/all/modules/_custom/sirius/images/map/pins/dot-yellow.png');
	$pins['fuzzy-black'] = array('title' => 'fuzzy-black', 'file' => 'fuzzy-black.png', 'path' => '/sites/all/modules/_custom/sirius/images/map/pins/fuzzy-black.png');
	$pins['fuzzy-blue'] = array('title' => 'fuzzy-blue', 'file' => 'fuzzy-blue.png', 'path' => '/sites/all/modules/_custom/sirius/images/map/pins/fuzzy-blue.png');
	$pins['fuzzy-brown'] = array('title' => 'fuzzy-brown', 'file' => 'fuzzy-brown.png', 'path' => '/sites/all/modules/_custom/sirius/images/map/pins/fuzzy-brown.png');
	$pins['fuzzy-green'] = array('title' => 'fuzzy-green', 'file' => 'fuzzy-green.png', 'path' => '/sites/all/modules/_custom/sirius/images/map/pins/fuzzy-green.png');
	$pins['fuzzy-purple'] = array('title' => 'fuzzy-purple', 'file' => 'fuzzy-purple.png', 'path' => '/sites/all/modules/_custom/sirius/images/map/pins/fuzzy-purple.png');
	$pins['fuzzy-red'] = array('title' => 'fuzzy-red', 'file' => 'fuzzy-red.png', 'path' => '/sites/all/modules/_custom/sirius/images/map/pins/fuzzy-red.png');
	$pins['fuzzy-teal'] = array('title' => 'fuzzy-teal', 'file' => 'fuzzy-teal.png', 'path' => '/sites/all/modules/_custom/sirius/images/map/pins/fuzzy-teal.png');
	$pins['fuzzy-yellow'] = array('title' => 'fuzzy-yellow', 'file' => 'fuzzy-yellow.png', 'path' => '/sites/all/modules/_custom/sirius/images/map/pins/fuzzy-yellow.png');
	$pins['kernel-black'] = array('title' => 'kernel-black', 'file' => 'kernel-black.png', 'path' => '/sites/all/modules/_custom/sirius/images/map/pins/kernel-black.png');
	$pins['kernel-blue'] = array('title' => 'kernel-blue', 'file' => 'kernel-blue.png', 'path' => '/sites/all/modules/_custom/sirius/images/map/pins/kernel-blue.png');
	$pins['kernel-brown'] = array('title' => 'kernel-brown', 'file' => 'kernel-brown.png', 'path' => '/sites/all/modules/_custom/sirius/images/map/pins/kernel-brown.png');
	$pins['kernel-gray'] = array('title' => 'kernel-gray', 'file' => 'kernel-gray.png', 'path' => '/sites/all/modules/_custom/sirius/images/map/pins/kernel-gray.png');
	$pins['kernel-green'] = array('title' => 'kernel-green', 'file' => 'kernel-green.png', 'path' => '/sites/all/modules/_custom/sirius/images/map/pins/kernel-green.png');
	$pins['kernel-lime'] = array('title' => 'kernel-lime', 'file' => 'kernel-lime.png', 'path' => '/sites/all/modules/_custom/sirius/images/map/pins/kernel-lime.png');
	$pins['kernel-orange'] = array('title' => 'kernel-orange', 'file' => 'kernel-orange.png', 'path' => '/sites/all/modules/_custom/sirius/images/map/pins/kernel-orange.png');
	$pins['kernel-pink'] = array('title' => 'kernel-pink', 'file' => 'kernel-pink.png', 'path' => '/sites/all/modules/_custom/sirius/images/map/pins/kernel-pink.png');
	$pins['kernel-purple'] = array('title' => 'kernel-purple', 'file' => 'kernel-purple.png', 'path' => '/sites/all/modules/_custom/sirius/images/map/pins/kernel-purple.png');
	$pins['kernel-red'] = array('title' => 'kernel-red', 'file' => 'kernel-red.png', 'path' => '/sites/all/modules/_custom/sirius/images/map/pins/kernel-red.png');
	$pins['kernel-teal'] = array('title' => 'kernel-teal', 'file' => 'kernel-teal.png', 'path' => '/sites/all/modules/_custom/sirius/images/map/pins/kernel-teal.png');
	$pins['kernel-white'] = array('title' => 'kernel-white', 'file' => 'kernel-white.png', 'path' => '/sites/all/modules/_custom/sirius/images/map/pins/kernel-white.png');
	$pins['kernel-yellow'] = array('title' => 'kernel-yellow', 'file' => 'kernel-yellow.png', 'path' => '/sites/all/modules/_custom/sirius/images/map/pins/kernel-yellow.png');

	foreach ($pins as $key => $ignore) {
		$pins[$key]['key'] = $key;
		if (preg_match('/^bulb/', $key)) {
			$pins[$key]['iconSize'] = array('x' => 30, 'y' => 46);
			$pins[$key]['iconAnchor'] = array('x' => 15, 'y' => 46);
			$pins[$key]['popupAnchor'] = array('x' => 0, 'y' => -48);
			$pins[$key]['tooltipAnchor'] = array('x' => 17, 'y' => -23);
		} else if (preg_match('/^dot/', $key))  {
			$pins[$key]['width'] = 23;
			$pins[$key]['height'] = 22;
			$pins[$key]['iconSize'] = array('x' => 23, 'y' => 22);
			$pins[$key]['iconAnchor'] = array('x' => 11, 'y' => 11);
			$pins[$key]['popupAnchor'] = array('x' => 0, 'y' => -13);
			$pins[$key]['tooltipAnchor'] = array('x' => 13, 'y' => 0);
		} else if (preg_match('/^fuzzy/', $key))  {
			$pins[$key]['iconSize'] = array('x' => 48, 'y' => 50);
			$pins[$key]['iconAnchor'] = array('x' => 24, 'y' => 25);
			$pins[$key]['popupAnchor'] = array('x' => 0, 'y' => -27);
			$pins[$key]['tooltipAnchor'] = array('x' => 27, 'y' => 0);
		} else if (preg_match('/^kernel/', $key))  {
			$pins[$key]['iconSize'] = array('x' => 15, 'y' => 35);
			$pins[$key]['iconAnchor'] = array('x' => 7, 'y' => 35);
			$pins[$key]['popupAnchor'] = array('x' => 0, 'y' => -37);
			$pins[$key]['tooltipAnchor'] = array('x' => 9, 'y' => -18);
		}
	}

	return $pins;
}

function sirius_map_pin($pin_key) {
	$all_pins = sirius_map_pins();
	if ($all_pins[$pin_key]) { 
		return $all_pins[$pin_key];
	} else {
		return sirius_map_pin_default();
	}
}

function sirius_map_pin_default() {
	return sirius_map_pin('bulb-blue');
}

function sirius_map_pin_picker($default_value = NULL) {
	drupal_add_css(drupal_get_path('module', 'sirius') . '/css/map.css');
	$pins = sirius_map_pins();
	$options = array();
	foreach ($pins as $pin_key => $pin) {
		$options[$pin_key] = '<div class="wrap"><div class="image">' . sirius_map_pin_render($pin) . '</div><div class="label">' . $pin['title'] . '</div></div>';
	}
	$form_element = array(
		'#type' => 'radios',
		'#title' => 'Pin',
		'#options' => $options,
		'#prefix' => '<div class="sirius_map_pin_picker">',
		'#suffix' => '</div><br clear="all" />',
	);

	if ($default_value) { $form_element['#default_value'] = $default_value; }
	return $form_element;
}

function sirius_map_pin_render($pin) {
	$html = "<img src=\"$pin[path]\" alt=\"$pin[title]\" width=\"$pin[iconSize][x]\" height=\"$pin[iconSize][y]\">";
	return $html;
}

function sirius_map_pin_render_leaflet($pin) {
	$elt = array();
	$elt['iconUrl'] = $pin['path'];
	$elt['iconSize'] = $pin['iconSize'];
	$elt['iconAnchor'] = $pin['iconAnchor'];
	$elt['popupAnchor'] = $pin['popupAnchor'];
	$elt['tooltipAnchor'] = $pin['tooltipAnchor'];
	return $elt;
}

/****************
 * Legacy
 ****************/

function sirius_map_icons() {
	return array(
		'marker-icon-blue.png' => 'Blue',
		'marker-icon-black.png' => 'Black',
		'marker-icon-gold.png' => 'Gold',
		'marker-icon-green.png' => 'Green',
		'marker-icon-grey.png' => 'Grey',
		'marker-icon-orange.png' => 'Orange',
		'marker-icon-red.png' => 'Red',
		'marker-icon-violet.png' => 'Violet',
		'marker-icon-yellow.png' => 'Yellow',
	);
}

function sirius_map_icon_name($icon) {
	$icons = sirius_map_icons();
	return $icons[$icon];
}

function sirius_map_icon_default() {
	return 'marker-icon-blue.png';
}

function sirius_map_icon_render($icon) {
	$name = htmlspecialchars(sirius_map_icon_name($icon));
	return "<img src=\"/sites/all/modules/_custom/sirius/images/map/$icon\" alt=\"$name\">";
}

function sirius_map_icon_url($icon) {
	return "/sites/all/modules/_custom/sirius/images/map/$icon";
}

