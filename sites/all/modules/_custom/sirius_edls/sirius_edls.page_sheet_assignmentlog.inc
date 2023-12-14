<?php

function sirius_edls_node_tab_sheet_assignmentlog_access($sheet_node) {
  return sirius_access_instance('edls')->access('sheet_assignmentlog', array('sheet_node' => $sheet_node));
}

function sirius_edls_node_tab_sheet_assignmentlog($sheet_node) {
	return 'Coming soon.';
}