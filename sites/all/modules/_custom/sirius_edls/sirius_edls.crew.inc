<?php

function sirius_edls_crew_render($sheet_node, $crew, $options) {
	if ($options['fetch_workers_from_json']) {
		$sheet_json = sirius_json_get($sheet_node);
		$all_assignments = $sheet_json['edls']['assignments'];
		$assignments = $all_assignments[$crew['uuid']];
		if (!$assignments) { $assignments = array(); }
	} else {
		$assignments = sirius_edls_assignment_fetch_crew($sheet_node->field_sirius_date_start['und'][0]['value'], $sheet_node->nid, $crew['uuid']);
	}
	$assigned_count = count($assignments);

	$html = '<div class="sirius_edls_crew sirius_callout_basic">';
	$html .= '<h2>' . sirius_edls_crew_title($crew) . '</h2>';
	$html .= sirius_fakerender_field('Time:', $crew['start_time'] . ' - ' . $crew['end_time']);
	$html .= sirius_fakerender_field('Task:', $crew['task']);
	$html .= sirius_fakerender_field('Count:', "$assigned_count / $crew[count]");

	$html .= '<div class="sirius_edl_crew_assignments">';
	$assignments_by_position = array();
	foreach ($assignments as $assignment) {
		$assignments_by_position[$assignment['assignment_position']] = $assignment;
	}

	$rows = array();
	for ($position = 0; $position < $crew['count']; ++$position) {
		$assignment = $assignments_by_position[$position];
		if (!$assignment) { $assignment = array(); }

		$row = array();
		$row[] = ($position+1);
		$row[] = $assignment['worker_id'];
		$row[] = ucwords(strtolower($assignment['worker_name']));
		$row[] = sirius_edls_crew_render_extra($assignment['assignment_extra']);
		$rows[] = $row;
	}
	$html .= sirius_table_render(array(), $rows, array('class' => array('sirius_table_basic sirius_table_align')));
	$html .= '</div>';
	$html .= '</div>';

	return $html;
}

function sirius_edls_crew_render_extra($extra) {
	$extra = json_decode($extra, TRUE);
	if (!$extra) { return ''; }

	$classification_options = sirius_edls_worker_classification_options();

	$parts = array();
	if ($extra['classification']) {
		$parts[] = $classification_options[$extra['classification']];
	}
	if ($extra['time']) {
		$parts[] = date('g:i a', strtotime($extra['time']));
	}
	return join(' ', array_filter($parts));
}

function sirius_edls_crews($sheet_node) {
	$sheet_json = sirius_json_get($sheet_node);
	$crews = $sheet_json['edls']['crews'];
	if (!$crews) { $crews = array(); }
	uksort($crews, function($a, $b) use ($crews) { return $crews[$a]['weight'] <=> $crews[$b]['weight']; });
	$crews = array_values($crews);
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
