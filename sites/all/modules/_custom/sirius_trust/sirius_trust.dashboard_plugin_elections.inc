<?php

function sirius_trust_dashboard_plugin_elections_data($prefs, $options = array()) {
	$records = sirius_trust_election_wizard_search(array('status' => 'submitted'));
	$count = count($records);

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
