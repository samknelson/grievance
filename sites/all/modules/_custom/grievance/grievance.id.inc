<?php

function grievance_id_set($node, $op) {
	// Assign an ID if one hasn't been set already.
	/*
	if ($node->type != 'grievance') { return; }
	if ($op != 'update' && $op != 'insert') { return; }
	if ($node->field_grievance_id['und'][0]['value']) { return; }

	$year = date('Y');

	$sql = "select max(field_grievance_id_value) as m ";
	$sql .= "from field_data_field_grievance_id ";
	$sql .= "where entity_type = 'node' and bundle = 'grievance' and field_grievance_id_value like '$year\-%' ";
	$stmt = db_query($sql, array());
	$hr = $stmt->fetchAssoc();
	$last_id = $hr['m'];
	list ($ignore, $number) = explode('-', $last_id);
	watchdog("whatever", "Here with $last_id, $ignore, $number");
	$number++;
	while (strlen($number) < 6) { $number = "0$number"; }
	$next_id = "$year-$number";

	$node->field_grievance_id['und'][0]['value'] = $next_id;
	node_save($node);
	watchdog('grievance', "Setting ID for " . $node->nid . " to $next_id.");
	*/
}