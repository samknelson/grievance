<?php

function sirius_edls_crew_render_extra($extra) {
	if (!$extra) { return ''; }
	if (!is_array($extra)) { $extra = json_decode($extra, TRUE); }

	$classification_options = sirius_edls_worker_classification_options();

	$parts = array();
	if ($extra['classification']) {
		$parts[] = $classification_options[$extra['classification']];
	}
	if ($extra['time']) {
		$parts[] = date('g:i a', strtotime($extra['time']));
	}
	if ($extra['truck']) {
		$parts[] = 'Trk #' . $extra['truck'];
	}
	return join(' ', array_filter($parts));
}

function sirius_edls_crews($sheet_node, $key = NULL) {
	$sheet_json = sirius_json_get($sheet_node);
	$crews = $sheet_json['edls']['crews'];
	if (!$crews) { $crews = array(); }
	uksort($crews, function($a, $b) use ($crews) { return $crews[$a]['weight'] <=> $crews[$b]['weight']; });

	foreach ($crews as $crew_uuid => $crew) {

	}
	if (!$key) { $crews = array_values($crews); }
	return $crews;
}

function sirius_edls_crew($sheet_node, $crew_uuid) {
	$sheet_json = sirius_json_get($sheet_node);
	$crews = $sheet_json['edls']['crews'];
	if (!$crews) { return NULL; }
	return $crews[$crew_uuid];
}

function sirius_edls_crew_title($crew) {
	if ($crew['name']) { return "Crew: " . $crew['name']; }
	
	if ($crew['start_time']) {
		$s = date('h:i a', strtotime($crew['start_time']));
	}
	if ($crew['end_time']) {
		$e = date('h:i a', strtotime($crew['end_time']));
	}

	if ($crew['count']) {
		$c = $crew['count'];
	}

	$title = 'Crew: ';

	if ($s && $e) { $title .= "$s-$e: "; }
	else if ($s) { $title .= "$s: "; }

	if ($c) { $title .= "x/$c: "; }

	if ($crew['task']) {
		$title .= $crew['task'] . ': ';
	}
	if ($crew['checkin_location']) {
		$title .= $crew['checkin_location'];
	}

	return $title;
}
