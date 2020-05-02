<?php

function sirius_training_node_tab_worker_access($worker_node) {
	// Training admin can access
	if (user_access('sirius training admin')) { return TRUE; }

	// Worker can access their own
	$my_worker_node = sirius_worker_get();
	if ($my_worker_node) { return $my_worker_node->nid == $worker_node->nid; }

	return FALSE;
}

function sirius_training_node_tab_worker($worker_node) {
	drupal_goto('node/' . $worker_node->nid . '/sirius_worker_tc/level');
}
