<?php

function sirius_trust_eligibility_plugin_hours_settings_form($json) {
	$form['#tree'] = TRUE;

	$form['min'] = array(
		'#title' => t('Minimum Hours'), 
		'#type' => 'textfield',
		'#size' => 4,
		'#default_value' => $json['min'],
		'#required' => TRUE,
	);

	$form['month'] = array(
		'#title' => t('Month'), 
		'#type' => 'textfield',
		'#default_value' => $json['month'],
		'#size' => 4,
		'#description' => t('Enter the month, e.g. "-4" for four months prior to the month that eligiblitiy is being tested.'),
		'#required' => TRUE,
	);

	return $form;
}

function sirius_trust_eligibility_plugin_hours_check($settings, $params) {
	$benefit_node = $params['benefit_node'];
	$subscriber_worker_node = $params['subscriber_worker_node'];
	$ts = $params['ts'];
	$date = gmdate('Y-m-d', $ts);

	$min = $settings['min'];
	$month_rel = $settings['month'];

	$month_ts = strtotime("$month_rel months", $ts);

	$year = date('Y', $month_ts);
	$month = date('m', $month_ts);
	$start_day = 1;
	$end_day = date('t', $month_ts);

	if ($year < 1971 || $year > 2100 || 
		$month < 1 || $month > 12) {
		return array('success' => FALSE, 'msg' => 'Invalid start / end dates. Check the plugin settings.');
	}

	$sql = 'select sum(hours) as c '; 
	$sql .= 'from sirius_hours_cache ';
	$sql .= "where worker_nid = :worker_nid ";
	$sql .= "and year = :year ";
	$sql .= "and month = :month ";
	$sql .= "and day >= :start_day ";
	$sql .= "and day <= :end_day ";
	$sql_args = array(
		':worker_nid' => $subscriber_worker_node->nid,
		':year' => $year,
		':month' => $month,
		':start_day' => $start_day,
		':end_day' => $end_day,
	);
	$stmt = sirius_sql_query($sql, $sql_args);
	$hr = $stmt->fetchAssoc();
	$hours = $hr['c'];

	$result = array();
	if ($hours >= $min) {
		$result['success'] = TRUE;
	} else {
		$result['success'] = FALSE;
	}

	$result['msg'] = "In the month of $year-$month, " . number_format($hours,2) . " hours found (min " . number_format($min, 2) . ").";

	$paths = array(
		"node/" . $subscriber_worker_node->nid . '/sirius_worker_hours' => 'Hours Entries',
	);
	$tools = array();
	foreach ($paths as $path => $label) {
		if (drupal_valid_path($path)) {
			$tools[] = l($label, $path, array('attributes' => array('class' => array('tool-button'))));
		}
	}
	$result['tools'] = $tools;
	return $result;
}