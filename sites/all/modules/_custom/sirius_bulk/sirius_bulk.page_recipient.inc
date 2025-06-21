<?php

function sirius_bulk_node_tab_recipient_access($recipient_node) {
	if ($recipient_node->field_sirius_category['und'][0]['value'] != 'bulk:queue') { return FALSE; }
	return TRUE;
}

function sirius_bulk_node_tab_recipient($recipient_node) {
	$renderable = node_view($recipient_node);
	$html = drupal_render($renderable);
	
	$instance = Sirius_Comm::getInstance();
	$comm_nodes = $instance->list_from_handler($recipient_node->nid);
	if ($comm_nodes) {
		$html .= $instance->render_multiple($comm_nodes);
	}

	return $html;
}