<?php

function sirius_flood_page_test() {
	return drupal_get_form('sirius_flood_page_test_form');
}

function sirius_flood_page_test_form($form, &$form_state) {
	$info_all = sirius_flood_info_all();
	$options = array();
	foreach ($info_all as $event => $info) {
		$options[$event] = $info['title'] . ' (' . $event . ')';
	}

	$form['event'] = array(
		'#type' => 'select',
		'#title' => t('Event'),
		'#options' => array(NULL => t('-- Select --')) + $options,
		'#required' => TRUE,
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => 'Submit',
	);

	return $form;
}

function sirius_flood_page_test_form_submit($form, &$form_state) {
	$event = $form_state['values']['event'];
	$info = sirius_flood_info($event);

	sirius_flood_register_event($event);
	drupal_set_message("Created flood event: " . $info['title'] . ' (' . $event . ')');

	$is_allowed = sirius_flood_is_allowed($event);
	if ($is_allowed) {
		drupal_set_message("The event IS NOT currently blocked.");
	} else {
		drupal_set_message("The event IS currently blocked.", 'warning');
	}
}