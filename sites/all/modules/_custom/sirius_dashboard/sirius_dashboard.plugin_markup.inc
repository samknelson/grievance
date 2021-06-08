<?php

function sirius_dashboard_plugin_markup_prefs($prefs) {
	$form['markup'] = array(
		'#type' => 'text_format',
		'#title' => 'Markup',
		'#default_value' => $prefs['markup']['value'],
		'#format' => 'sirius_form_editor',
		'#allowed_formats' => array('sirius_form_editor'),
	);

	$form['link_path'] = array(
		'#type' => 'textfield',
		'#title' => 'Link Path',
		'#default_value' => $prefs['link_path'],
		'#description' => t('Enter the system path that this dashboard item should link to. The pathshould not begin with a slash (/). For example, "sirius/config".'),
	);

	$form['link_text'] = array(
		'#type' => 'textfield',
		'#title' => 'Link Text',
		'#default_value' => $prefs['link_text'],
		'#description' => t('Enter the text that should appear in the link button.'),
	);

	return $form;
}

function sirius_dashboard_plugin_markup_data($prefs, $options = array()) {
	$tools = '';
	if ($prefs['link_path']) {
		$tools = l($prefs['link_text'], $prefs['link_path'], array('attributes' => array('class' => array('tool-button'))));
	}
	$row = array(
		'details' => $prefs['markup']['value'],
		'count' => 0,
		'tools' => $tools,
	);
	return array($row);
}