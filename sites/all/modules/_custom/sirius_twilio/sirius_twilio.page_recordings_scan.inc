<?php

function sirius_twilio_page_recordings_scan() {
	return drupal_get_form('sirius_twilio_page_recordings_scan_form');
}

function sirius_twilio_page_recordings_scan_form($form, &$form_state) {
	$form['mode'] = array(
		'#type' => 'select',
		'#title' => t('Mode'),
		'#options' => array(
			'test' => t('Test mode - scan but do not archive or delete any recordings'),
			'live' => t('Live mode - execute a live scan to archive or delete'),
		),
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

  $log = sirius_minilog_render_html();
  if ($log) { $form['results'] = array('#markup' => '<hr>' . $log); }

  return $form;
}

function sirius_twilio_page_recordings_scan_form_submit($form, &$form_state) {
	$form_state['rebuild'] = TRUE;
	$mode = $form_state['values']['mode'];
	sirius_twilio_recordings_scan($mode);
}

function sirius_twilio_recordings_scan($mode) {
	$delete_enabled = sirius_domain_variable_get('sirius_twilio_recordings_delete');
	$delete_days = sirius_domain_variable_get('sirius_twilio_recordings_delete_days');

	sirius_minilog("== Starting recording scan. Mode = $mode, Delete = $delete_enabled / $delete_days days");

	// sirius_minilog("Pass 1: Searching for conversations to archive.");
	// sirius_minilog("[not yet implemented]");

	// sirius_minilog("Pass 2: Searching for conversations to delete.");

	if ($delete_enabled != 'Yes') {
		sirius_minilog("Deletions are not enabled. No recordings will be deleted.");
	} else if (!is_numeric($delete_days) || ($delete_days < 1)) {
		sirius_minilog("The number of days is less than 1. No recordings will be deleted.");
	} else {
		sirius_minilog("[not yet implemented]");
	}
}