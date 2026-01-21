<?php

function sirius_trust_dashboard_plugin_elections_data($prefs, $options = array()) {
	$count = sirius_trust_election_wizard_search(array('status' => 'submitted', 'count_only' => TRUE));

	$tools = l(
		'Submitted Elections',
		'sirius/trust/election/wizard/list',
		array('attributes' => array('class' => array('tool-button')))
	);

	$details = "There are <strong>$count</strong> submitted elections waiting approval.";

	$row = array(
		'details' => $details,
		'count' => $count,
		'tools' => $tools,
	);

	return array($row);
}
