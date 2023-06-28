<?php

function sirius_dispatch_type_compute(&$dispatch_node) {
	$type = $dispatch_node->field_sirius_dispatch_type['und'][0]['value'];
	$status = $dispatch_node->field_sirius_dispatch_status['und'][0]['value'];

	// "accepted" dispatches are always primary
	if ($status == 'accepted') { $type = 'Primary'; }

	// "accepted_secondary" is always secondary
	if ($status == 'accepted_secondary') { $type = 'Secondary'; }

	// If we don't have a type, set type = "None";
	if (!$type) { $type = 'None'; }

	$dispatch_node->field_sirius_dispatch_type['und'][0]['value'] = $type;
}