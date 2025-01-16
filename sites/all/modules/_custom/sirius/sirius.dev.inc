<?php

function sirius_dev_server_types() {
	return array(
		'dev' => 'Development Server',
		'stage' => 'Staging Server',
		'live' => 'Live Server',
	);
}

function sirius_dev_server_type_set($type) {
	variable_set('sirius_dev_server_type', $type);
}

function sirius_dev_server_type() {
	return variable_get('sirius_dev_server_type', 'live');
}

function sirius_dpm($variable) {
	if (function_exists('dpm')) {
		return dpm($variable);
	} else {
		drupal_set_message('<pre>' . print_r($variable, 1) . '</pre>');
	}
}