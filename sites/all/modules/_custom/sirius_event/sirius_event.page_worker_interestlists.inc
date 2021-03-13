<?php

function sirius_event_node_tab_worker_interestlists_access($worker_node) {
	sirius_require('sirius_event', 'sirius_event.page_contact_interestlists.inc');
	return sirius_event_node_tab_contact_interestlists_access(node_load($worker_node->field_sirius_contact['und'][0]['target_id']));
}

function sirius_event_node_tab_worker_interestlists($worker_node) {
	sirius_require('sirius_event', 'sirius_event.page_contact_interestlists.inc');
	return sirius_event_node_tab_contact_interestlists(node_load($worker_node->field_sirius_contact['und'][0]['target_id']));
}