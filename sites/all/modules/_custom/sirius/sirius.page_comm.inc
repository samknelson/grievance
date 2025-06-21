<?php

function sirius_node_tab_comm_access($comm_node) {
	// Must be one of the log types that has a comm instance associated (i.e. 'sms', 'voice', 'email', maybe 'postal')
	$instance = sirius_comm_instance($comm_node);
	if (!$instance) { return FALSE; }

	return TRUE;
}

function sirius_node_tab_comm($comm_node) {
	$medium = $comm_node->field_sirius_category['und'][0]['value'];
	$instance = sirius_comm_instance($medium);
	if (!$instance) { return FALSE; }
	return $instance->render($comm_node);
}