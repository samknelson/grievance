<?php

function sirius_map_leaflet_create($settings = array()) {
	if (!$settings) {
		$settings['zoom'] = 16;
		// $settings['fullscreenControl'] = TRUE;
		// $settings['fullscreenControlOptions'] = array('position' => 'topleft');
	}
	// $map = leaflet_map_get_info('google-roadmap');
	// dpm(leaflet_map_get_info());
	// drupal_add_css('sites/all/libraries/leaflet_fullscreen/Control.FullScreen.css');
 	// drupal_add_js('sites/all/libraries/leaflet_fullscreen/Control.FullScreen.js');
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