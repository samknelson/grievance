<?php

function sirius_event_page_intake_anonymous($access_code = NULL) {
	if (!$access_code) {
		return drupal_get_form('sirius_event_page_intake_anonymous_landing_form');
	}

	$event_node = sirius_event_intake_fetch_by_access_code($access_code);
	if (!$event_node) {
		drupal_set_message(t("That does not appear to be a valid access code."), 'error');
		drupal_goto('intake');
	}
	$json = sirius_jsonfield_util_node_json_get($event_node);
	if ($json['intake']['enabled'] != 'Yes') {
		drupal_set_message(t('Intake is not enabled for that event.'), 'error');
		drupal_goto('intake');
	}
	if ($json['intake']['anonymous']['enabled'] != 'Yes') {
		drupal_set_message(t('Anonymous intake is not enabled for that event.'), 'error');
		drupal_goto('intake');
	}

	$title = $json['intake']['anonymous']['title'];
	if (!$title) { $title = $event_node->title; }
	drupal_set_title($title);

	$renderable = node_view($event_node, 'full');
  $html .= render(field_view_field('node', $event_node, 'body', 'full'));
	return $html;
}

function sirius_event_page_intake_anonymous_landing_form($form, &$form_state) {
	$form['access_code'] = array(
		'#type' => 'textfield',
		'#title' => t('Access Code'),
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_event_page_intake_anonymous_landing_form_submit($form, &$form_state) {
	$access_code = sirius_event_intake_access_code_anonicalize($form_state['values']['access_code']);
	drupal_goto('intake/' . $access_code);
}