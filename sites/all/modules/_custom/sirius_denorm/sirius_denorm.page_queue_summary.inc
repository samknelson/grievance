<?php

function sirius_denorm_page_queue_summary() {
	$summary = sirius_denorm_queue_summary();

	if (!$summary) {
		return '<p>' . t('The denormalize queue is empty.') . '</p>';
	}

	$statuses = sirius_denorm_queue_statuses();
	$totals = array();
	$rows = array();
	$ts_min = NULL;
	$ts_now = time();
	foreach ($summary as $trigger => $records) {
		$row = array();
		$row[] = $trigger;
		foreach ($statuses as $status => $status_title) {
			$record = $records[$status];
			$count = $record['count'];
			$row[] = number_format($count, 0);
			$totals[$status] += $count;
		}

		$ts = $records['pending']['first_ts'];
		if ($ts) {
			$row[] = format_interval($ts_now - $ts) . ' ago';
			if (!$ts_min || ($ts < $ts_min)) { $ts_min = $ts; }
		} else {
			$row[] = '';
		}
		$rows[] = $row;
	}

	$row = array();
	$row[] = '<strong>' . t('Total') . '</strong>';
	foreach ($statuses as $status => $status_title) {
		$row[] = '<strong>' . number_format($totals[$status]) . '</strong>';
	}
	if ($ts_min) {
		$row[] = '<strong>' . format_interval($ts_now - $ts_min) . ' ago' . '</strong>';
	} else {
		$row[] = '';
	}
	$rows[] = $row;
	
	$header = array();
	$header[] = t('Trigger');
	foreach ($statuses as $status => $status_title) { $header[] = $status_title; }
	$header[] = t('Oldest Pending Record');

	return sirius_table_render($header, $rows);
}