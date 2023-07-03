<?php

function sirius_sched_render($scheds, $format = 'full') {
	$html = "<div class=\"sirius_sched sirius_sched_$format\">";
	if ($format == 'full') { $html .= sirius_sched_render_full($scheds); }
	else if ($format == 'tiny') { $html .=  sirius_sched_render_tiny($scheds); }
	else { $html .=  'Unknown format: $format'; }
	$html .= '</div>';
	return $html;
}

function sirius_sched_render_tiny($scheds) {
	$shorts = array();
	foreach ($scheds as $ts => $sched) {
		$title_short = $sched['json']['display']['title_short'];
		if (!$title_short) { $title_short = $sched['type']; }
		$shorts[$title_short]++;
	}

	drupal_add_js(drupal_get_path('module', 'sirius') . '/js/sirius.popup.js');
	drupal_add_css(drupal_get_path('module', 'sirius') . '/css/sirius.popup.css');

	$html = '<a href="#" class="sirius_popup_trigger">' . join(',', array_keys($shorts)) . '</a>';
	$html .= '<div style="display:none" class="sirius_popup_overlay"></div>';
	$html .= '<div style="display:none" class="sirius_popup_wrap">';
	$html .= '<div class="sirius_popup_contents">';
	$html .= sirius_sched_render_full($scheds);
	$html .= '</div>';
	$html .= '<div class="sirius_popup_close"></div>';
	$html .= '</div>';
	return $html;
}

function sirius_sched_render_full($scheds) {
	if (!$scheds) { return $html .= 'The schedule is empty.'; }

	$rows = array();
	foreach ($scheds as $sched) {
		$tools = '';
		$path = $sched['json']['display']['path'];
		if (!$path) { $path = 'node/' . $sched['handler_nid']; }
		if (drupal_valid_path($path)) { $tools .= l('View', $path, array('attributes' => array('class' => array('tool-button')))); }

		$start_ts = $sched['start_ts'];
		$end_ts = $sched['end_ts'];

		$granularity = $sched['json']['display']['granularity'];
		if (!$granularity) { $granularity = 'day'; }

		$start_date = date('m/j/Y', $start_ts);
		$end_date = date('m/j/Y', $end_ts);
		$start_time = date('h:i A', $start_ts);
		$end_time = date('h:i A', $end_ts);

		$datestr = $start_date;
		if ($granularity == 'min') { $datestr .= ' ' . $start_time; }

		if ($end_ts) {
			if ($start_date == $end_date) {
				if ($granularity == 'min') { $datestr .= ' - ' . $end_time; }
			} else {
				$datestr .= ' - ' . $end_date;
				if ($granularity == 'min') { $datestr .= ' ' . $end_time; }
			}
		}

		$display_type = $sched['json']['display']['type'];
		if (!$display_type) { $display_type = $sched['type']; }

		$display_status = $sched['json']['display']['status'];
		$details = $sched['json']['display']['details'];

		$row = array();
		$row[] = $datestr;
		$row[] = $display_type;
		$row[] = $display_status;
		$row[] = $details;
		$row[] = $tools;
		$rows[] = $row;
	}

	$header = array(
		'Date',
		'Type',
		'Status',
		'Details',
		'Tools',
	);

	return sirius_table_render($header, $rows);
}
