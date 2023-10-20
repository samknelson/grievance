<?php

function sirius_dispatch_plugin_worker_schedule_tor_prefs($json) {
	$type_status_options = sirius_worker_schedule_tor_type_status_options();
	$form['type_status_key'] = array(
		'#type' => 'select',
		'#title' => 'Type and Status',
		'#options' => array(NULL => t('-- Select --')) + $type_status_options,
		'#default_value' => $json['type_status_key'],
	);

	return $form;
}


function sirius_dispatch_plugin_worker_schedule_tor_data($prefs, $options = array()) {
	$type_status_key = $prefs['type_status_key'];
	list ($type_tid, $status_uuid) = explode('|', $type_status_key);

	$params = array();
	$params['status'] = $status_uuid;
	$params['type_tid'] = $type_tid;
	$tor_nodes = sirius_worker_schedule_tor_list_all($params);
	if (!$tor_nodes) { return array(); }

	$tools = '';
	$tools .= l('Manage', 'sirius/worker/schedule/tor', array('attributes' => array('class' => array('tool-button'))));
	$first_tor_node = reset($tor_nodes);
	$row = array(
		'count' => count($tor_nodes),
		'tools' => $tools,
		'details' => 'E.g. ' . $first_tor_node->title,
	);

	return array($row);
}

