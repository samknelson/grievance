<?php

function sirius_denorm_dashboard_data($prefs, $options = array()) {
	$rows = array();

	$summary = sirius_denorm_queue_summary();
	ksort($summary);

	$total = 0;
	$msgs = array();
	$ts_min = NULL;
	foreach ($summary as $trigger => $record) {
		$ts = $record['pending']['first_ts'];
		if (!$ts_min || ($ts < $ts_min)) { $ts_min = $ts; }

		$trigger_count = $record['pending']['count'];
		$total += $trigger_count;

		$msg = "<strong>$trigger_count</strong> $trigger";
		if ($ts) {
			// $msg .= ' (' . format_interval(time() - $ts) . ' ago)';
		}
		$msgs[] = $msg;
	}

	$msg = join("<br />", $msgs);
	if (!$msg) { $msg = "The denormalize queue is empty."; }

	$tools = l(
		'Queue', 
		'sirius/denorm/queue_summary', 
		array(
			'attributes' => array('class' => array('tool-button')),
		)
	);

	$row = array(
		'title' => 'Summary',
		'count' => $total,
		'tools' => $tools,
		'details' => $msg,
	);
	$rows[] = $row;

	return $rows;
}

