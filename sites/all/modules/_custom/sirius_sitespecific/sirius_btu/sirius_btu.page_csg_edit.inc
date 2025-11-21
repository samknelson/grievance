<?php

function sirius_btu_node_tab_csg_edit_access($csg_node) {
	$access = Sirius_Btu_Csg_Access::getInstance();
	return $access->access('view', array('csg_node' => $csg_node));
}

function sirius_btu_node_tab_csg_edit_form($form, &$form_state, $csg_node) {
	$csg = Sirius_Btu_Csg::getInstance();
	$form = $csg->form($form, $form_state, $csg_node);
	return $form;
}

function sirius_btu_node_tab_csg_edit_form_validate($form, &$form_state) {
	$csg = Sirius_Btu_Csg::getInstance();
	$form = $csg->form_validate($form, $form_state);
	return $form;
}
function sirius_btu_node_tab_csg_edit_form_submit($form, &$form_state) {
	$csg = Sirius_Btu_Csg::getInstance();
	$form = $csg->form_submit($form, $form_state);
	return $form;
}