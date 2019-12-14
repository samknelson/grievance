<?php

function sirius_node_tab_event_bullpen_access($event_node) {
	return sirius_event_bullpen_enabled($event_node);
}

function sirius_node_tab_event_bullpen($event_node) {
	return drupal_goto('node/' . $event_node->nid . '/sirius_event_bullpen/view');
}