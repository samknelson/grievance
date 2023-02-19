<?php

function report_cache_page_landing($module = NULL) {
	// Get all the report types
	$report_type_infos = report_cache_info_all();

	// Get our modules
	$links = array();
	$links[] = array(
		'path' => 'sirius/reports',
		'title' => t('All Reports'),
	);
	$unique = array();
	foreach ($report_type_infos as $report_type => $report_type_info) {
		if ($unique[$report_type_info['module']]) { continue; }
		$unique[$report_type_info['module']] = TRUE;
	
		$links[] = array(
			'path' => 'sirius/reports/by-module/' . $report_type_info['module'],
			'title' => $report_type_info['module_name'],
		);
	}
	$result .= sirius_quicknav($links);

	$rows = array();
	foreach ($report_type_infos as $report_type => $report_type_info) {
		if ($module && ($module != $report_type_info['module'])) { continue; }
		$tools = l('List', "sirius/reports/$report_type", array('attributes' => array('class' => array('tool-button'))));

		$path = $report_type_info['path'];
		if (!$path) { 
			$path = "sirius/reports/$report_type/form";
		}
		$tools .= l(t('Generate'), $path, array('attributes' => array('class' => array('tool-button'))));

		$name = $report_type_info['name'];
		if ($report_type_info['description']) { $name .= '<br />' . $report_type_info['description']; }

		$module_name = $report_type_info['module_name'];

		$row = array();
		if (!$module) { $row[] = $module_name; }
		$row[] = $name;
		$row[] = $tools;
		$rows[] = $row;
	}

	$header = array();
	if (!$module) { $header[] = t('Module'); }
	$header[] = t('Report Type');
	$header[] = t('Tools');
	$result .= theme_table(array('header' => $header, 'rows' => $rows, 'attributes' => array()));
	return $result;
}
