<?php

function grievance_id_set($node, $op) {
	// Assign an ID if one hasn't been set already.
	if ($node->type != 'grievance') { return; }
	if ($op != 'update' && $op != 'insert') { return; }
	if ($node->field_grievance_id['und'][0]['value']) { return; }

	watchdog('whatever', 'Setting ID.');
}