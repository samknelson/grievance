<?php

function sirius_edls_node_tab_sheet_edit_access($sheet_node) {
  return sirius_access_instance('edls')->access('sheet_edit', array('sheet_node' => $sheet_node));
}

function sirius_edls_node_tab_sheet_edit($sheet_node) {
	return drupal_get_form('sirius_edls_sheet_form', $sheet_node);
}