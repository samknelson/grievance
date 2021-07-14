<?php

function sirius_dashboard_plugin_list() {
	$info = sirius_dashboard_plugin_info_all();
	$options = array();
	foreach ($info as $key => $entry) { $options[$key] = $entry['title']; }
	return $options;
}

function sirius_dashboard_plugin_info($plugin_key) { 
	$info_all = sirius_dashboard_plugin_info_all();
	return $info_all[$plugin_key];
}

function sirius_dashboard_plugin_info_all() {
	$plugins = array();

	$plugins['worker_schedule_tor'] = array(
		'title' => 'Worker: Schedule: Time-Off Requests',
		'file' => 'sirius_dashboard.plugin_worker_schedule_tor.inc',
		'module' => 'sirius_dashboard',
		'callbacks' => array(
			'data' => 'sirius_dispatch_plugin_worker_schedule_tor_data',
		),
	);

	$plugins['events'] = array(
		'title' => 'Event: Upcoming',
		'description' => '',
		'file' => 'sirius_dashboard.plugin_events.inc',
		'module' => 'sirius_dashboard',
		'callbacks' => array(
			'data' => 'sirius_dispatch_plugin_events_data',
			'config' => 'sirius_dispatch_plugin_events_config',
			'prefs' => 'sirius_dispatch_plugin_events_prefs',
		),
	);

	$plugins['event_intake'] = array(
		'title' => 'Event: Intake Applications',
		'description' => '',
		'file' => 'sirius_dashboard.plugin_event_intake.inc',
		'module' => 'sirius_dashboard',
		'callbacks' => array(
			'data' => 'sirius_dispatch_plugin_event_intake_data',
			'config' => 'sirius_dispatch_plugin_event_intake_config',
			'prefs' => 'sirius_dispatch_plugin_event_intake_prefs',
		),
	);

	$plugins['dispatch_jobs'] = array(
		'title' => 'Dispatch: Jobs',
		'description' => '',
		'file' => 'sirius_dashboard.plugin_dispatch_jobs.inc',
		'module' => 'sirius_dashboard',
		'callbacks' => array(
			'data' => 'sirius_dispatch_plugin_dispatch_jobs_data',
			'config' => 'sirius_dispatch_plugin_dispatch_jobs_config',
			'prefs' => 'sirius_dispatch_plugin_dispatch_jobs_prefs',
		),
	);

	$plugins['training_evaluations'] = array(
		'title' => 'Training: Evaluations',
		'description' => '',
		'file' => 'sirius_dashboard.plugin_training_evaluations.inc',
		'module' => 'sirius_dashboard',
		'callbacks' => array(
			'data' => 'sirius_dispatch_plugin_training_evaluations_data',
		),
	);

	$plugins['training_level_counts'] = array(
		'title' => 'Training: Level Counts',
		'description' => '',
		'file' => 'sirius_dashboard.plugin_training_level_counts.inc',
		'module' => 'sirius_dashboard',
		'callbacks' => array(
			'data' => 'sirius_dashboard_plugin_training_level_counts_data',
			'prefs' => 'sirius_dashboard_plugin_training_level_counts_prefs',
		),
	);

	$plugins['markup'] = array(
		'title' => 'Raw Markup',
		'description' => '',
		'file' => 'sirius_dashboard.plugin_markup.inc',
		'module' => 'sirius_dashboard',
		'callbacks' => array(
			'data' => 'sirius_dashboard_plugin_markup_data',
			'config' => 'sirius_dashboard_plugin_markup_config',
			'prefs' => 'sirius_dashboard_plugin_markup_prefs',
		),
	);

	return $plugins;
}
