<?php

/**
 * Implements hook_sirius_cron_scan_info()
 */

function report_cache_sirius_cron_scan_info() {
	$items['report_cache'] = array(
		'title' => 'Report Auto Generation',
		'description' => 'Run any reports which are set to run automatically',
		'schedule' => 'daily',
		'callback' => 'report_cache_auto_all',
		'multidomain' => TRUE,
	);

	return $items;
}

function report_cache_auto_all($mode = 'test') {
	$report_info_all = report_cache_info_all();

	$count_run = 0; 
	foreach ($report_info_all as $report_type => $report_info) {
		$settings = report_cache_settings($report_type);
    if ($settings['auto']['enabled'] != 'Yes') { continue; }
		sirius_minilog("$report_type: Report generation starting.");	
		$result = report_cache_auto_run($report_type, $mode);
		$result = report_cache_auto_purge($report_type, $mode);
		$count_run++;
	}

	return array('success' => TRUE, 'msg' => "Ran $count_run reports");
}

function report_cache_auto_run($report_type, $mode = 'test') {

	$info = report_cache_info($report_type);
	if ($info['file']) { sirius_require($info['module'], $info['file']); }

	$form_state = array();
	$form_state['values'] = array();
	$form_state['values']['report_type'] = $report_type;
	$form_state['values']['report_id'] = 'Auto';
	$form_state['values']['nobatch'] = TRUE;
	$form_state['values']['noninteractive'] = TRUE;
	if ($mode != 'live') {
		sirius_minilog("-- Skipping run because mode = $mode");
	} else {
		$result = report_cache_form_submit(array(), $form_state);
	}

	sirius_minilog("-- Report generation complete.");	
	return array('success' => TRUE);
}

function report_cache_auto_purge($report_type, $mode = 'test') {
	sirius_minilog("-- Purging old reports.");
	$start_ts = 0;
	$end_ts = strtotime('-30 days');
	$suffix = 'Auto';
	if ($mode != 'live') {
		sirius_minilog("-- Skipping purge because mode = $mode");
	} else {
		$result = report_cache_purge_date($report_type, $start_ts, $end_ts, $suffix);
		sirius_minilog("-- purged $result[count] records");
	}
}

