<?php

function sirius_teamsters631_page_m12_month($ym) {
	list ($year, $month) = explode('-', $ym);
	if (!($year > 0) || ($year < 1900) || ($year > 2100) ) { $year = date('Y'); }
	if (!($month > 0) || ($month < 1) || ($month > 12) ) { $month = date('n'); }

	drupal_set_title("12th of the Month: $month/$year");

	$employers = sirius_teamsters631_m12_employers($year, $month);

	return 'Boo';
}

function sirius_teamsters631_m12_employers($year = NULL, $month = NULL) {
	if (!$year) { $year = date('Y'); }
	if (!$month) { $month = date('n'); }

	// Get the industry
	$industry_tid = sirius_teamsters631_m12_industry();
	if (!$industry_tid) {
		drupal_set_message("Industry (CONV) not found.", 'error');
		return array();
	}

	// Get employers with that industry
	$employers = sirius_employer_list(TRUE, $industry_tid);

	// Only employers with active dispatches as of the end of the last month
	$dispatch_year = $year;
	$dispatch_month = $month;
	--$dispatch_month;
	if ($dispatch_month < 1) { $dispatch_month = 12; --$dispatch_year; }
	$dispatch_day = date('t', strtotime("$dispatch_year-$dispatch_month-15"));
	drupal_set_message("Here with " . "$dispatch_year-$dispatch_month-$dispatch_day 23:59:59");
	$ts = strtotime("$dispatch_year-$dispatch_month-$dispatch_day 23:59:59");

	$search_params = array();
	$search_params['statuses'] = array('active', 'active_secondary');
	$search_params['ts'] = $ts;
	$search_params['limit'] = 1;
	foreach ($employers as $employer_nid => $employer_name) {
		$search_params['employer_nid'] = $employer_nid;
		$probe = sirius_dispatch_search($search_params);
		if (!$probe) { unset($employers[$employer_nid]); }
	}
	drupal_set_message('<pre>' . print_r($employers, 1) . '</pre>');
}

function sirius_teamsters631_m12_industry() {
	return sirius_taxonomy_lookup_by_anything('sirius_industry', 'CONV');
}