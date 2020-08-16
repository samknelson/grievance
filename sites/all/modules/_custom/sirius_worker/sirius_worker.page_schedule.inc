<?php

function sirius_worker_node_tab_schedule_access($worker_node) {
	return node_access('view', $worker_node);
}

function sirius_worker_node_tab_schedule($worker_node) {
	$schedule = sirius_worker_schedule($worker_node->nid);

	$html = '<h2>Upcoming Schedule (next 30 days)</h2>';
	$html .= sirius_worker_schedule_render($schedule);
	return $html;
}