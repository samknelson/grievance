<?php

function sirius_dev_server_type_info_all() {
	return array(
		'dev' => array(
			'title' => 'Development Server',
			'banner_prefix' => 'DEV',
		),
		'stage' => array(
			'title' => 'Staging Server',
			'banner_prefix' => 'STAGE',
		),
		'live' => array(
			'title' => 'Live Server',
			'banner_prefix' => '',
		),
	);
}

function sirius_dev_server_type_info($key) {
	return sirius_dev_server_type_info_all()[$key];
}

function sirius_dev_server_types() {
	$info = sirius_dev_server_type_info_all();
	$options = array();
	foreach ($info as $key => $record) { $options[$key] = $record['title']; }
	return $options;
}

function sirius_dev_server_type_set($type) {
	variable_set('sirius_dev_server_type', $type);
}

function sirius_dev_server_type() {
	return variable_get('sirius_dev_server_type', 'live');
}

function sirius_dev_make_safe() {
	// Make root safe
	module_invoke_all('sirius_dev_make_safe_root', array('server_type' => 'stage'));

	// Make each domain safe
	$domains = array(-1 => 'root') + sirius_domain_tree_render_as_select();
	if ($context['server_type'] != 'live') {
		foreach ($domains as $domain_nid => $domain_name) {
			sirius_domain_push($domain_nid);
			module_invoke_all('sirius_dev_make_safe_domain', array('server_type' => 'stage'));
			sirius_domain_pop($domain_nid);
		}
	}
}

/**
 * Implements hook_sirius_dev_make_safe_root()
 */

function sirius_sirius_dev_make_safe_root($context) {
	// Add a prefix to the banner
	$server_type_info = sirius_dev_server_type_info($context['server_type']);
	$name = variable_get('sirius_banner_name');
	$name = preg_replace('/^\[.*\]\s*/', '', $name);
	if ($server_type_info['banner_prefix']) {
		$name = '[' . $server_type_info['banner_prefix'] . '] ' . $name;
	}
	variable_set('sirius_banner_name', $name);

	// Disable email globally
	if ($context['server_type'] != 'live') {
		variable_set('sirius_email_global_disable', 'Yes');
	}
}

function sirius_sirius_dev_make_safe_domain($context) {
	sirius_domain_variable_set('sirius_sms_global_disable', 'Yes');
}
