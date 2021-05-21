<?php

function sirius_event_intake_application_assignment_options($event_node) {
	$event_json = sirius_json_get($event_node);
	$lines = explode("\n", $event_json['intake']['assignments']['options']);

	$options = array();
	foreach ($lines as $delta => $line) {
		$line = trim($line);
		$options[$line] = $line;
	}
	return $options;
}

function sirius_event_intake_application_assignment_get($application_node) {
	$json = sirius_json_get($application_node);
	if (!$json['assignment']) { return array(); }
	return $json['assignment'];
}

function sirius_event_intake_application_assignment_set($application_node, $assignment_name) {
	$json = sirius_json_get($application_node);
	$old_assignment_name = $json['assignment']['name'];
	if ($old_assignment_name == $assignment_name) { return array('success' => TRUE, 'msg' => 'Assignment unchanged.'); }
	$json['assignment']['name'] = $assignment_name;
	$json['assignment']['ts'] = time();
	sirius_json_set($application_node, $json);
	$msg = "Changed assignment from $old_assignment_name to $assignment_name";
	sirius_jsonfield_log($application_node, 'assign', $msg);
	node_save($application_node);
	return array('success' => TRUE, 'msg' => $msg);
}
