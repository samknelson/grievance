<?php

function sirius_node_tab_worker_contact_customfields_access($worker_node) {
	return node_access('view', $worker_node);
}

function sirius_node_tab_worker_contact_customfields($worker_node) { 
	$contact_node = node_load($worker_node->field_sirius_contact['und'][0]['target_id']);
	if (!$contact_node) { return 'There is no contact associated with this worker.'; }
	return sirius_node_tab_contact_json($contact_node);
}
