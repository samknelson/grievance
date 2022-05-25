<?php

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