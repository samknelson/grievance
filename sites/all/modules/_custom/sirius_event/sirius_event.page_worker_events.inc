<?php

function sirius_event_node_tab_worker_events($worker_node) {
	sirius_require('sirius_event', 'sirius_event.page_contact_events.inc');
	return sirius_event_node_tab_contact_events(node_load($worker_node->field_sirius_contact['und'][0]['target_id']));
}