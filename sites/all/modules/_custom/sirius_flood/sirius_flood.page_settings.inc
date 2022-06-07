<?php

function sirius_flood_page_settings() {
	return drupal_get_form('sirius_flood_page_settings_form');
}

function sirius_flood_page_settings_form($form, &$form_state) {
	$info_all = sirius_flood_info_all();

	$is_first = TRUE;
	foreach ($info_all as $event => $info) {
		$form[$event . "_info"] = array('#markup' => '<h2>' . $info['title'] . '</h2>' . '<p>' . $info['description'] . '</p>');

		$form[$info['window_variable']] = array(
			'#type' => 'textfield',
			'#size' => 6,
			'#title' => 'Window',
			'#default_value' => variable_get($info['window_variable'], $info['window_default']),
			'#description' => $is_first ? 'Window (in seconds) before flood events expire' : '',
		);

		$form[$info['threshold_variable']] = array(
			'#type' => 'textfield',
			'#size' => 6,
			'#title' => 'Threshold',
			'#default_value' => variable_get($info['threshold_variable'], $info['threshold_default']),
			'#description' => $is_first ?'Number of flood events permitted before the action is blocked' : '',
		);

		$is_first = FALSE;
	}

	return system_settings_form($form);
}