<?php

/**
 * Implements hook_element_info_alter
 */

function sirius_element_info_alter(&$type) {
	// The date_popup by default only lets you pick years 3 years past or future. 
	// This is a weird default, and is frustrating because e.g. we have birthday fields all over. 
	// So let's override the built-in default.
	if (isset($type['date_popup']['#date_year_range'])) {
		$type['date_popup']['#date_year_range'] = '-100:+100';
	}
}

function sirius_date_ts($node, $field, $delta = 0, $col = 'value') {
	if (!$node->{$field}['und'][$delta][$col]) { return 0; }
	return strtotime($node->{$field}['und'][$delta][$col] . ' ' . $node->{$field}['und'][$delta]['timezone_db']);
}

function sirius_date_date($node, $field, $format = 'Y-m-d H:i:s') {
	return date($format, sirius_date_ts($node, $field));
}

/**
 * Allow a new date formats mmddyy and mmddyyyy.
 */

function sirius_strtotime($time, $now = NULL) {
	$matches = array();
	if (preg_match('/^\s*(\d\d)(\d\d)(\d\d\d?\d?)\s*$/', $time, $matches)) {
		$month = $matches[1];
		$day = $matches[2];
		$year = $matches[3];
		$time = "$year-$month-$day";
	}

	if ($now) { return strtotime($time, $now); } else { return strtotime($time); }
}

function sirius_dob_format($dob) {
	if (!$dob) { return ''; }

	$ts = strtotime($dob);
	return date('m/d/Y') . ' (' . format_interval(time() - $ts, 1) . ')';
}

function sirius_date_yearmonth_widget($params) {
	drupal_add_css(drupal_get_path('module', 'sirius') . '/css/form.css');

	$form = array(
		'#type' => 'container',
		'#title' => t('Month'),
		'#attributes' => array('class' => array('sirius_form_inline_field_collection')),
		'#tree' => TRUE,
	);

	$default_year = $params['default_year'];
	if (!$default_year) { $default_year = date('Y'); }
	$year_options = array();
	for ($i=$default_year-10; $i<=$default_year+1; ++$i) {
		$year_options[$i] = $i;
	}
	$form['year'] = array(
		'#type' => 'select',
		'#options' => $year_options,
		'#default_value' => $default_year,
		'#title' => t('Year'),
	);

	$month_options = array(
		1 => t('January'),
		2 => t('February'),
		3 => t('March'),
		4 => t('April'),
		5 => t('May'),
		6 => t('June'),
		7 => t('July'),
		8 => t('August'),
		9 => t('September'),
		10 => t('October'),
		11 => t('November'),
		12 => t('December'),
	);

	$default_month = $params['default_month'];
	if (!$default_month) { $default_month = date('n'); }
	$form['month'] = array(
		'#type' => 'select',
		'#options' => $month_options,
		'#default_value' => $default_month,
		'#title' => t('Month'),
	);

	return $form;
}

function sirius_tz_indicator($tz) {
	$orig_tz = date_default_timezone_get();
	date_default_timezone_set($tz);
	$current_time = date("h:i A");
	date_default_timezone_set($orig_tz);
	return '<div class="sirius_tz_indicator">' . 
		'<i class="far fa-clock"></i>' .
		"&nbsp;&nbsp;Using time zone <strong>$tz</strong>. Current time <strong>$current_time.</strong><br /><br />" . '</strong>' .
		'</div>';
}