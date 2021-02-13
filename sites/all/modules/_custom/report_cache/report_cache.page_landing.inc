<?php

function report_cache_page_landing() {
	// Get all the report types
	$report_type_infos = module_invoke_all('report_cache_info');

	$rows = array();
	foreach ($report_type_infos as $report_type => $report_type_info) {
		$tools = l('List', "admin/reports/report-cache/$report_type", array('attributes' => array('class' => array('tool-button'))));

		$path = $report_type_info['path'];
		if (!$path) { 
			$path = 'sirius/report-cache/' . $report_type . '/form';
		}
		$tools .= l(t('Generate'), $path, array('attributes' => array('class' => array('tool-button'))));

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
