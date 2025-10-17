<?php

function sirius_btu_page_csg_add() {
	return drupal_get_form('sirius_btu_page_csg_add_form');
}

function sirius_btu_page_csg_add_form($form, &$form_state) {
	$csg = Sirius_Btu_Csg::getInstance();
	$form = $csg->form($form, $form_state);
	return $form;
}

function sirius_btu_page_csg_add_form_validate($form, &$form_state) {
	$csg = Sirius_Btu_Csg::getInstance();
	$csg->switch_into_main_domain();
	return $csg->form_validate($form, $form_state);
}

function sirius_btu_page_csg_add_form_submit($form, &$form_state) {
	$csg = Sirius_Btu_Csg::getInstance();
	$csg->switch_into_main_domain();

	$csg_node = $csg->form_submit($form, $form_state);
	drupal_goto('node/' . $csg_node->nid . '/sirius_btu_csg_view');
}