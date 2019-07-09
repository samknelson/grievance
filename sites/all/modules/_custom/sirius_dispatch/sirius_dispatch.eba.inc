<?php

function sirius_dispatch_eba_dates_upcoming() {
	$dates = array();
	for ($i=0; $i<8; ++$i) {
		$ts = strtotime("+$i days");
		$date = date('Y-m-d', $ts);
		$label = date('l, F j', $ts);
		$dates[$date] = $label;
	}
	return $dates;
}

function sirius_dispatch_eba_dates_worker($worker_node) {
	$dates = array();
	if ($worker_node->field_sirius_dispatch_eba_dates['und']) {
		foreach ($worker_node->field_sirius_dispatch_eba_dates['und'] as $tmp) {
			$date = date('Y-m-d', strtotime($tmp['value']));
			$dates[$date] = $date;
		}
	}
	sort(array_unique($dates));
	return $dates;
}
