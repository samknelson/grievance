<?php

function sirius_node_tab_event_intake_application_manage_skill_access($application_node) {
	// Must be an application-type log entry
	if ($application_node->field_sirius_category['und'][0]['value'] != 'event:intake_application') { return FALSE; }

	// There must already be a worker
	if (!sirius_log_handler($application_node, 'sirius_worker')) { return FALSE; }

	return TRUE;
}

function sirius_node_tab_event_intake_application_manage_skill($application_node) {
	return 'UNDER DEVELOPMENT';
}
