<?php

function sirius_shortlink_clicked_error($msg) {
	print $msg;
	exit();
}

function sirius_shortlink_clicked($uuid) {
	// Fetch the node
	$shortlink_nodes = entity_uuid_load('node', array($uuid));
	if (!$shortlink_nodes) { sirius_shortlink_clicked_error("Sorry, that link was not found."); }
	$shortlink_node = array_values($shortlink_nodes)[0];
	if (!$shortlink_node) { sirius_shortlink_clicked_error("Sorry, that node was not found."); }

	dpm($shortlink_node);
	$json = sirius_jsonfield_util_node_json_get($shortlink_node);
	dpm($json);
	return "I was clicked.";
}
