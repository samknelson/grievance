<?php

function sirius_event_survey_get_nid($event_node, $which = 'default') {
	$event_json = sirius_jsonfield_util_node_json_get($event_node);
	$survey_nid = $event_json['event']['survey'][$which];

	// Legacy support
	if (!$survey_nid && $which == 'default') { $survey_nid = $event_node->field_sirius_json_definition['und'][0]['target_id']; }

	return $survey_nid;
}

function sirius_event_survey_get($event_node, $which = 'default') {
	$survey_nid = sirius_event_survey_get_nid($event_node, $which);
	return node_load($survey_nid);
}

function sirius_event_survey_list($event_node) {
	$event_json = sirius_jsonfield_util_node_json_get($event_node);
	return $event_json['event']['survey'];
}

function sirius_event_survey_which_name($which) {
	if ($which == 'default') { return 'Default'; }
	if ($which == 'intake') { return 'Intake'; }
	return $which;
}

function sirius_event_survey_which_results($which) {
		if ($which == 'default') { return 'survey'; } else { return "survey_$which"; }
	}