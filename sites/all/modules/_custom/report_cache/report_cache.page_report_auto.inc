<?php

/**
 * @todo: The whole "auto" thing is basically a legacy, and should be handled within the "instances" framework. But for now, it's 
 *        here for report without a parameters callback.
 */

function report_cache_page_report_auto_access($report_type) {
	// Must have a report type
	if (!$report_type) { return FALSE; }
	$info = report_cache_info($report_type);
	if (!$info) { return FALSE; }

	// Must have suitable privileges
	$access = $info['access'];
	if (!$access) { $access = 'access site reports'; }
	if (!user_access($access)) { return FALSE; }

	// Must NOT have a parameteres callback; otherwise, it's handled in instances
	if ($info['generator']->form()) { return FALSE; }

	return TRUE;
}

function report_cache_page_report_auto($report_type) {
	return drupal_get_form('report_cache_page_report_auto_form', $report_type);
}

function report_cache_page_report_auto_form($form, &$form_state, $report_type) {
	$form_state['report_type'] = $report_type;

	$info = report_cache_info($report_type);
	if ($info['file']) { sirius_require($info['module'], $info['file']); }
	if (!$info) { return array('#markup' => 'Report type not found.'); }
	drupal_set_title("Reports: " . $info['name']);
	sirius_backlink_add('sirius/reports', array('name' => 'All Reports'));

	$report_settings = report_cache_settings($report_type);

	$form['#tree'] = TRUE;

	$form['auto']['enabled'] = array(
		'#type' => 'select',
		'#title' => 'Auto-Generation Enabled?',
		'#options' => array(
			NULL => t('-- Select --'),
			'No' => t('No'),
			'Yes' => t('Yes'),
		),
		'#default_value' => $report_settings['auto']['enabled'],
	);


	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit')
	);

	return $form;
}

function report_cache_page_report_auto_form_submit($form, &$form_state) {
	$report_type = $form_state['report_type'];
	$report_settings = report_cache_settings($report_type);
	$report_settings['auto'] = $form_state['values']['auto'];
	report_cache_settings($report_type, $report_settings);
	drupal_set_message("Settings saved.");
}