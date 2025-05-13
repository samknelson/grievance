<?php

function sirius_worker_tos_node_tab_worker_archive_access($worker_node) {
	return Sirius_Worker_Tos_Access::getInstance()->access('worker_view_archive', array('worker_node' => $worker_node));
}

function sirius_worker_tos_node_tab_worker_archive($worker_node) {
	$tos = Sirius_Worker_Tos::getInstance();

	$search_params = array();
	$search_params['worker_nid'] = $worker_node->nid;
	$tos_nodes = $tos->search($search_params);

	$display_params = array();
	$display_params['hide_worker'] = TRUE;
	return $tos->render_list($tos_nodes, $display_params);
}
