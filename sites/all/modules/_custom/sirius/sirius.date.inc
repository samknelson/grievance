<?php

function sirius_date_ts($node, $field, $delta = 0, $col = 'value') {
	if (!$node->{$field}['und'][$delta]['value']) { return 0; }
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