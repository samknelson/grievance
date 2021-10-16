<?php

function sirius_bulk_node_tab_recipients_access($bulk_node) {
	if (sirius_domain_nid() && !sirius_domain_component_enabled('bulk')) { return FALSE; }
	return TRUE;
}

function sirius_bulk_node_tab_recipients($bulk_node) {
	drupal_goto('node/' . $bulk_node->nid . '/sirius_bulk_recipients/workers');
}