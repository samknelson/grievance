<?php

function sirius_event_map_modal_access($event_nid, $marker_id) {
	// Verify that every participant in this marker is in this event
	$participant_nids = explode('_', $marker_id);
	$participant_nodes = node_load_multiple($participant_nids);
	if (!$participant_nodes) { return FALSE; }
	foreach ($participant_nids as $participant_nid) {
		$participant_node = $participant_nodes[$participant_nid];
		if ($participant_node->field_sirius_event['und'][0]['target_id'] != $event_nid) { return FALSE; }
	}

	// We may also be passed in "all_participant_nids", if we need to set the timestamp for other members of the household
	$all_participant_nids = $_REQUEST['all_participant_nids'];
	if ($all_participant_nids) {
		$all_participant_nids = explode(',', $all_participant_nids);
		$all_participant_nodes = node_load_multiple($all_participant_nids);
		foreach ($all_participant_nids as $participant_nid) {
			$participant_node = $all_participant_nodes[$participant_nid];
			if ($participant_node->field_sirius_event['und'][0]['target_id'] != $event_nid) { return FALSE; }
		}
	}

	// Verify that we have access to the underlying event
	$event_node = node_load($event_nid);
	return sirius_node_tab_event_map_access($event_node);
}

function sirius_event_map_modal($event_nid, $marker_id) {
	$participant_nids = explode('_', $marker_id);
	if (count($participant_nids) == 1) {
		sirius_require('sirius_event', 'sirius_event.page_map_modal_single.inc');
		return drupal_get_form('sirius_event_map_modal_form_single', $event_nid, $marker_id);
	} else {
		sirius_require('sirius_event', 'sirius_event.page_map_modal_multiple.inc');
		return drupal_get_form('sirius_event_map_modal_form_multiple', $event_nid, $marker_id);
	}
}

