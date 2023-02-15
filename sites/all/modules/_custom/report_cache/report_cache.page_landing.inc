<?php

function report_cache_page_landing_access() {
	$report_type_infos = report_cache_info_all();

	foreach ($report_type_infos as $report_type => $report_type_info) {
		if (report_cache_access($report_type)) { return TRUE; }
	}
	return FALSE;
}

function report_cache_page_landing() {
	// Get all the report types
	$report_type_infos = report_cache_info_all();

	$rows = array();
	foreach ($report_type_infos as $report_type => $report_type_info) {
		if (!report_cache_access($report_type)) { continue; }

		$tools = l('List', "sirius/reports/$report_type", array('attributes' => array('class' => array('tool-button'))));
		$tools .= l('Generate', "sirius/reports/$report_type/form", array('attributes' => array('class' => array('tool-button'))));

		$name = $report_type_info['name'];
		if ($report_type_info['description']) { $name .= '<br />' . $report_type_info['description']; }

		$row = array();
		$row[] = $name;
		$row[] = $tools;
		$rows[] = $row;
	}

	$header = array(
		'Report Type',
		'Tools'
	);
	$result .= theme_table(array('header' => $header, 'rows' => $rows, 'attributes' => array()));
	return $result;
}
