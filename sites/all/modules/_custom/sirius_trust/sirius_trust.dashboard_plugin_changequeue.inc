<?php

function sirius_trust_dashboard_plugin_changequeue_data($prefs, $options = array()) {
	$cq = Sirius_Trust_Changequeue::getInstance();
	$count = $cq->search(array('status' => 'pending', 'count_only' => TRUE));

	$tools = l(
		'Change Queue',
		'https://sirius.benefits11.org/sirius/trust/changequeue',
		array('attributes' => array('class' => array('tool-button')))
	);

	$details = "There are <strong>$count</strong> changes that need review.";

	$row = array(
		'details' => $details,
		'count' => $count,
		'tools' => $tools,
	);

	return array($row);
}
