<?php

function sirius_node_tab_letter_access($letter_node) {
	if ($letter_node->field_sirius_category['und'][0]['value'] != 'letter') { return FALSE; }

 	$handlers = sirius_log_handlers($letter_node);
 	foreach ($handlers as $handler_node) {
		if (sirius_letter_access('view', $handler_node, $letter_node)) { return TRUE; }
	}

	return FALSE;
}

function sirius_node_tab_letter($letter_node) {
	return sirius_node_tab_wizard($letter_node, 'letter');
}