<?php

function sirius_dashboard_node_tab_worker_access($worker_node) {
	// Must have an account
	$account = sirius_worker_get_user($worker_node->nid);
	if (!$account) { return FALSE; }
	return TRUE; 
}

function sirius_dashboard_node_tab_worker($worker_node) {
	global $user;
	$user_orig = $user;
	$user = sirius_worker_get_user($worker_node->nid);

	$html .= '<div class="sirius_dashboard_wrapper" id="sirius_dashboard_content">';
	$items = sirius_dashboard_content();
	$html = sirius_dashboard_render($items);
	$html .= '</div>';

	$user = $user_orig;

	return $html;
}
