<?php

function sirius_edls_node_tab_sheet_view_access($sheet_node) {
  return sirius_access_instance('edls')->access('sheet_view', array('sheet_node' => $sheet_node));
}

function sirius_edls_node_tab_sheet_view($sheet_node) {
	// module_load_include('inc', 'diff', 'diff.pages');
	//  return diff_diffs_overview($sheet_node);
	return sirius_edls_sheet_render($sheet_node);
}