<?php

function sirius_lang_options_global() {
	require_once DRUPAL_ROOT . '/includes/iso.inc';
	$x = _locale_get_predefined_list();
	$options = array();
	foreach ($x as $key => $y) {
		$options[$key] = $y[0];
	}
	return $options;
}

function sirius_lang_options() {
	$options = sirius_str_to_options(sirius_domain_variable_get('sirius_lang_options'));
	if (!$options) { return sirius_lang_options_global(); }
	return $options;
}
