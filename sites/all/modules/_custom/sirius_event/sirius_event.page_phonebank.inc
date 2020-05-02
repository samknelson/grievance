<?php

function sirius_node_tab_event_phonebank_access($event_node) {
	return sirius_event_phonebank_enabled($event_node);
}

function sirius_node_tab_event_phonebank($event_node) {
	return drupal_goto('node/' . $event_node->nid . '/sirius_event_phonebank/call');
}

