<?php

function sirius_dispatch_plugin_training_evaluations_data($prefs, $options = array()) {
	$evaluation_nodes = sirius_training_evaluations_fetch_by_type('draft');
	if (!$evaluation_nodes) { return array(); }

	$tools = '';
	$tools .= l('Queue', 'sirius/training/evaluations/queue', array('attributes' => array('class' => array('tool-button'))));
	$first_evaluation_node = reset($evaluation_nodes);
	$row = array(
		'count' => count($evaluation_nodes),
		'tools' => $tools,
		'details' => 'E.g. ' . sirius_training_evaluations_title($first_evaluation_node),
	);

	return array($row);
}

