<?php

function sirius_steward_node_tab_log_sched_access($sched_node) {
	if ($sched_node->field_sirius_category['und'][0]['value'] != 'steward:sched') { return FALSE; }
	return TRUE;
}

function sirius_steward_node_tab_log_sched($sched_node) {
	$options = array();
	$options['hide_edit'] = TRUE;
	return sirius_steward_sched_render($sched_node, $options);
}
