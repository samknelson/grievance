<?php

function sirius_map_leaflet_create($settings = array()) {
	if (!$settings) {
		$settings['zoom'] = 16;
		$settings['fullscreenControl'] = TRUE;
		$settings['tap'] = FALSE; // See https://github.com/Leaflet/Leaflet/issues/7255
		// settings['fullscreenControlOptions'] = array('position' => 'topleft');
	}
	drupal_add_css('sites/all/libraries/leaflet_fullscreen/dist/leaflet.fullscreen.css');
 	drupal_add_js('sites/all/libraries/leaflet_fullscreen/dist/Leaflet.fullscreen.min.js');
 	// drupal_add_js('sites/all/libraries/leaflet_print/dist/bundle.js');
	$map = leaflet_map_get_info(sirius_domain_variable_get('sirius_map_provider', 'OSM Mapnik'));
	$map['settings'] = $settings;
	return $map;
}

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

  return system_settings_form($form);
}

function sirius_map_icon_render($icon) {
	$name = htmlspecialchars(sirius_map_icon_name($icon));
	return "<img src=\"/sites/all/modules/_custom/sirius/images/map/$icon\" alt=\"$name\">";
}

function sirius_map_icon_url($icon) {
	return "/sites/all/modules/_custom/sirius/images/map/$icon";
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
	$map_feature = array(
	  'type' => 'point',
	  'lat' => $json_checkin['lat'],
	  'lon' => $json_checkin['lon'],
	  'popup' => $popup_html,
    'icon' => array('iconUrl' => '/sites/all/modules/_custom/sirius/images/map/marker-icon-blue.png'),
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