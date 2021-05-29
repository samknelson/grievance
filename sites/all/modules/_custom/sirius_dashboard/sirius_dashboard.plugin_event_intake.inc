<?php

function sirius_dispatch_plugin_event_intake_prefs($json) {
	$form['assignment_name'] = array(
		'#type' => 'textfield',
		'#title' => 'Assignee',
		'#description' => 'Enter the assignment name. This must exactly match the assignment name in the intake configuration.',
		'#default_value' => $json['assignment_name'],
	);

	$form['statuses'] = array(
		'#type' => 'checkboxes',
		'#title' => 'Statuses',
		'#description' => 'Select the event intake statuses which should be counted',
		'#options' => sirius_event_intake_application_status_options(),
	);
	if ($json['statuses']) { $form['statuses']['#default_value'] = $json['statuses']; }

	return $form;
}

function sirius_dispatch_plugin_event_intake_data($prefs, $options = array()) {
	$assignment_name = $prefs['assignment_name'];

	$domain_nid = sirius_domain_nid();

	// @todo: I iterate through all the relevant nodes here, which could be expensive
	//        It would be nice to just fetch the count() and event_nid.
	//        Unfortunately, it's cumbersome to find the event_nid, because it's in log_handler, there might
	//        be multiple log handlers.
	//        So I can't be bothered right at the moment.

	$sql_args = array();
	if (!$prefs['statuses']) { return NULL; }
	$sql_args[':statuses'] = array_filter(array_values($prefs['statuses']));
	$sql = "select node.nid from node ";
	$sql .= "left join field_data_field_sirius_json on field_data_field_sirius_json.entity_type = 'node' and field_data_field_sirius_json.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_category on field_data_field_sirius_category.entity_type = 'node' and field_data_field_sirius_category.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_type on field_data_field_sirius_type.entity_type = 'node' and field_data_field_sirius_type.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
	$sql .= "where node.type = 'sirius_log' ";
	$sql .= "and field_sirius_category_value = 'event:intake_application' ";
	if ($domain_nid) {
		$sql_args[':domain_nid'] = $domain_nid;
		$sql .= "and field_sirius_domain_target_id = :domain_nid ";
	}
	if ($assignment_name) {
		$sql_args[':assignment_name'] = $assignment_name;
		$sql .= "and json_unquote(json_extract(field_sirius_json_value, '\$.assignment.name')) = :assignment_name ";

	}
	$sql .= "and field_sirius_type_value in (:statuses) ";
	$sql .= "order by node.nid desc limit 1000 ";

	$stmt = sirius_sql_query($sql, $sql_args);

	$events = array();
	while ($hr = $stmt->fetchAssoc()) {
		$application_node = node_load($hr['nid']);
		$event_node = sirius_log_handler($application_node, 'sirius_event');
		if (!$events[$event_node->nid]) { $events[$event_node->nid] = array('title' => $event_node->title, 'count' => 0); }
		++$events[$event_node->nid]['count'];
	}

	$rows = array();
	foreach ($events as $event_nid => $info) {
		$row = array();
		$row['details'] = $info['title'];
		$row['count'] = $info['count'];
		$row['tools'] = l('Applications', "node/$event_nid/sirius_event_intake/applications", array('attributes' => array('class' => array('tool-button')), 'query' => array('assignment_name' => $assignment_name)));
		$rows[] = $row;
	}

	return $rows;
}

