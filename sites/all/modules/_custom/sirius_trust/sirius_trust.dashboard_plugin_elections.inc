<?php

function sirius_trust_dashboard_plugin_elections_data($prefs, $options = array()) {
	$records = sirius_trust_election_wizard_search(array('status' => 'submitted'));
	if (!$records) { return array(); }

	$tools = '';
	$tools = l(
		'Submitted Elections',
		'sirius/trust/election/wizard/list',
		array('attributes' => array('class' => array('tool-button')))
	);

	$details = reset($records);

	$row = array(
		'details' => '',
		'count' => count($records),
		'tools' => $tools,
	);

	return array($row);
}
