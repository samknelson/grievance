<?php

function sirius_node_tab_event_intake_application_wizard_access($application_node) {
	if ($application_node->field_sirius_category['und'][0]['value'] != 'event:intake_application') { return FALSE; }
	return TRUE;
}

function sirius_node_tab_event_intake_application_wizard($application_node) {
	return sirius_node_tab_wizard($application_node, 'event_intake');
}
