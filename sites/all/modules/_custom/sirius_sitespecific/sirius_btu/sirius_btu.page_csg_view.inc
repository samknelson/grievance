<?php

function sirius_btu_node_tab_csg_view_access($csg_node) {
	$access = Sirius_Btu_Csg_Access::getInstance();
	return $access->access('view', array('csg_node' => $csg_node));
}

function sirius_btu_node_tab_csg_view($csg_node) {
	$csg = Sirius_Btu_Csg::getInstance();

	$view_options = array();
	return $csg->render_view($csg_node, $view_options);
}