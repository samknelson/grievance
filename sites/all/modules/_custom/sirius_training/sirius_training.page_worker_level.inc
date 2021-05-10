<?php

function sirius_training_node_tab_worker_level_access($worker_node) {
	// Training admin can access
	if (user_access('sirius training admin')) { return TRUE; }

	// Worker can access their own
	$my_worker_node = sirius_worker_get();
	if ($my_worker_node) { return $my_worker_node->nid == $worker_node->nid; }

	return FALSE;
}

function sirius_training_node_tab_worker_level($worker_node) {
	$is_admin = user_access('sirius training admin');

	if ($is_admin) {
		$ms_tid = $_REQUEST['ms_tid'];
		$graduate = $_REQUEST['graduate'];
		if ($ms_tid && $graduate) {
			return sirius_training_node_tab_worker_graduate($worker_node, $ms_tid, $graduate);
		}
	}

	return sirius_training_render_levels($worker_node, array('is_admin' => $is_admin));

}

