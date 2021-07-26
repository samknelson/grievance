<?php

function sirius_worker_node_tab_schedule_tor_edit_access($tor_node) {
	if ($tor_node->field_sirius_category['und'][0]['value'] != 'tor') { return FALSE; }
	return sirius_worker_schedule_tor_access('edit', NULL, $tor_node);
}

function sirius_worker_node_tab_schedule_tor_edit($tor_node) {
	$entity_node = sirius_log_handler($tor_node, 'sirius_worker');
	if (!$entity_node) {
		drupal_set_message("Time-off request is not associated with a worker.", 'error');
		return '';
	}

	return drupal_get_form('sirius_worker_schedule_tor_addedit_form', $entity_node, $tor_node);
}