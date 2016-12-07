<?php

function report_cache_page_landing() {
	// Get all the report types
	$report_type_infos = module_invoke_all('report_cache_info');

	$result .= t('The following report types are available.');
	$result .= '<ul>';
	foreach ($report_type_infos as $report_type => $report_type_info) {
		$result .= '<li>';
		$result .= l($report_type_info['name'], "admin/reports/report-cache/$report_type");
		if ($report_type_info['description']) {
			$result .= ' (';
			$result .= $report_type_info['description'];
			$result .= ')';
		}
		$result .= '</li>';
	}
	$result .= '</ul>';
		
	return $result;
}
