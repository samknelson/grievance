<?php

function sirius_btu_node_tab_worker_csg($worker_node) {
	$csg = Sirius_Btu_Csg::getInstance();

	$search_params = array();
	$search_params['worker_nid'] = $worker_node->nid;
	$csg_nodes = $csg->search($search_params);

	$render_params = array();
	$render_params['hide_worker'] = TRUE;
	$html = $csg->render_list($csg_nodes, $render_params);
	$form['results'] = array('#markup' => $html);
	return $form;
}