<?php

function sirius_dispatch_plugin_worker_schedule_tor_data($prefs, $options = array()) {
	$tor_nodes = sirius_worker_schedule_tor_list_all(array('status' => 'requested'));
	if (!$tor_nodes) { return array(); }

	$tools = '';
	$tools .= l('Manage', 'sirius/worker/schedule/tor', array('attributes' => array('class' => array('tool-button'))));
	$first_tor_node = reset($tor_nodes);
	$row = array(
		'count' => count($tor_nodes),
		'tools' => $tools,
		'details' => 'E.g. ' . $first_tor_node->title,
	);

	return array($row);
}

