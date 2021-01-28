<?php

/**
 * @todo: Implement a complicated callback system here so that modules can define their own server paths.
 */

function sirius_server_paths_info_all() {
	$info_all = array(
		'wkhtmltopdf' => array(
			'title' => 'wkhtmltopdf',
			'description' => 'Command-line utility to convert HTML into PDF',
			'default_value' => '/usr/local/bin/wkhtmltopdf',
		),
		'xvfb-run' => array(
			'title' => 'xvfb-run',
			'description' => 'Wrapper for wkhtmltopdf, required to set various internal settings',
			'default_value' => '/usr/bin/xvfb-run',
		),
		'pdftk' => array(
			'title' => 'pdftk',
			'description' => 'PDFTk utility for combining multiple PDF entries.',
			'default_value' => '/usr/bin/pdftk',
		),
		'pdfunite' => array(
			'title' => 'pdfunite',
			'description' => 'PDFUnite utility for combining multiple PDF entries.',
			'default_value' => '/usr/bin/pdfunite',
		),
		'cp' => array(
			'title' => 'cp',
			'description' => 'Linux file copy',
			'default_value' => '/bin/cp',
		),
		'drush' => array(
			'title' => 'cp',
			'description' => 'DRUSH',
			'default_value' => '/usr/bin/drush',
		),
	);


	return $info_all;
}

function sirius_server_paths_info($key) {
	$info_all = sirius_server_paths_info_all();
	return $info_all[$key];
}

function sirius_server_paths($key) {
	$variable_name = 'sirius_server_paths_' . $key;
	$info = sirius_server_paths_info($key);
	$path = variable_get($variable_name, $info['default_value']);
	if (!$path) { $path = '/bin/echo'; }
	return $path;
}

function sirius_server_paths_admin() {

	$info_all = sirius_server_paths_info_all();
	foreach ($info_all as $key => $info) {
		$variable_name = 'sirius_server_paths_' . $key;
		$form[$variable_name] = array(
			'#type' => 'textfield',
			'#default_value' => sirius_server_paths($key),
			'#title' => $info['title'],
			'#description' => $info['description'],
		);
	}

  return system_settings_form($form);
}
