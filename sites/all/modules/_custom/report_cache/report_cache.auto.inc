<?php

/**
 * Implements hook_cron
 */

function report_cache_cron() {
	report_cache_auto_all();
}

// Here's a useful command:
// drush --user=1 php-eval "sirius_minilog_echo_active(TRUE); print_r(report_cache_auto_all());"

function report_cache_auto_all($force = FALSE) {
	// Poll once an hour
	$ts = time();
	if (!$force) {
		$last_ts = variable_get('report_cache_auto_all', 0);
		$next_ts = $last_ts + (60*60);
		if ($ts < $next_ts) {
			$msg = "Too soon to run. Next run at " . date('c') . " in " . format_interval($next_ts - $ts);
			sirius_minilog($msg);
			return array('success' => FALSE, 'msg' => $msg);
		}
	}
	variable_set('report_cache_auto_all', $ts);

	$report_info_all = report_cache_info_all();

	$count_scan = $count_run = 0; 
	foreach ($report_info_all as $report_type => $report_info) {
		$result = report_cache_auto($report_type, $force);
		$count_scan++;
		$count_run += $result['count_run'];
	}

	return array('success' => TRUE, 'msg' => "Scanned $count_scan reports, ran $count_run across all reports and domains.");
}

function report_cache_auto($report_type, $force = FALSE) {
	// Poll once a day
	$ts = time();
	if (!$force) {
		$last_ts = variable_get("report_cache_auto_$report_type", 0);
		$next_ts = $last_ts + (24*60*60);
		if ($ts < $next_ts) { 
			$msg = "$report_type: Too soon to run. Next run at " . date('c') . " in " . format_interval($next_ts - $ts);
			sirius_minilog($msg);
			return array('success' => FALSE, 'msg' => $msg);
		}
	}
	variable_set("report_cache_auto_$report_type", $ts);

	sirius_minilog("$report_type: Checking to see if any domain has auto enabled for this report.");
	$domains = sirius_domain_tree_render_as_select();
	$original_domain_nid = sirius_domain_nid();
	$count_scan = $count_run = 0;
	foreach ($domains as $domain_nid => $domain_name) {
		++$count_scan;
		// sirius_minilog("$report_type: $domain_name: checking");
		sirius_domain($domain_nid);
		$settings = report_cache_settings($report_type);
		if ($settings['auto']['enabled'] != 'Yes') { continue; }

		sirius_minilog("$report_type: $domain_name: Auto-generation is enabled. Running the report.");
		report_cache_auto_run($report_type);
		report_cache_auto_purge($report_type);
		++$count_run;
	}
	sirius_domain($original_domain_nid);
	sirius_minilog("$report_type: Scanned $count_scan domains, ran $count_run reports.");

	return array('success' => TRUE, 'count_run' => $count_run);
}

function report_cache_auto_run($report_type) {
	sirius_minilog("$report_type : Report generation starting.");	

	$info = report_cache_info($report_type);
	if ($info['file']) { sirius_require($info['module'], $info['file']); }

	$form_state = array();
	$form_state['values'] = array();
	$form_state['values']['report_type'] = $report_type;
	$form_state['values']['report_id'] = 'Auto';
	$form_state['values']['nobatch'] = TRUE;
	$form_state['values']['noninteractive'] = TRUE;
	report_cache_form_submit(array(), $form_state);

	sirius_minilog("$report_type: Report generation complete.");	
	return array('success' => TRUE);
}

function report_cache_auto_purge($report_type) {
	$start_ts = 0;
	$end_ts = strtotime('-30 days');
	$suffix = 'Auto';
	report_cache_purge_date($report_type, $start_ts, $end_ts, $suffix);
}

