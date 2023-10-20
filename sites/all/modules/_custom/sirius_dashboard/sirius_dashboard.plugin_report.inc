<?php

function sirius_dashboard_plugin_report_prefs($json) {
	$report_info_all = report_cache_info_all();
	$report_options = array();
	foreach ($report_info_all as $report_type => $info) {
		if (!$info['dashboard']) { continue; }
		$report_options[$report_type] = $info['name'];
	}
	$form['report_type'] = array(
		'#type' => 'select',
		'#title' => 'Report',
		'#options' => array(NULL => t('-- Select --')) + $report_options,
		'#default_value' => $json['report_type'],
	);

	return $form;
}

function sirius_dashboard_plugin_report_data($prefs, $options = array()) {
	$report_type = $prefs['report_type'];
	if (!$report_type) { return array(array('count' => 0, 'details' => '[Missing report type]')); }

	$report_id = report_cache_most_recent($report_type);
	if (!$report_id) { 
		return array(
			array(
				'count' => 0, 
				'tools' => l('Generate', "sirius/reports/$report_type/form", array('attributes' => array('class' => array('tool-button')))),
				'details' => 'Report has never been run',
			)
		);
	}

	$parameters = report_cache_parameters($report_type, $report_id);
	if (!$parameters['meta']['dashboard']['count']) { return array(); }

	$details = $parameters['meta']['dashboard']['details'];

	$result = array();
	$result['tools'] = l('View', "sirius/reports/$report_type", array('query' => array('report_id' => $report_id, 'action' => 'render', 'callback-name' => 'View'), 'attributes' => array('class' => array('tool-button'))));
	$count = $parameters['meta']['dashboard']['count'];
	$result['count'] = $count;
	$ts = $parameters['meta']['ts']['completed'];
	$result['details'] = "<strong>$count records</strong> found " . format_interval(time() - $ts) . ' ago.';
	if ($details) { $result['details'] .= '<br>' . $details; }

	return array($result);

	return $rows;
}

