<?php

function sirius_page_log_modal_view($log_node) {
	if (!$log_node->type == 'sirius_log') { return sirius_page_error("Not a log entry."); }

	if (!drupal_valid_path('node/' . $log_node->nid)) { return sirius_page_error("Access Denied."); }

	return node_view($log_node);
}