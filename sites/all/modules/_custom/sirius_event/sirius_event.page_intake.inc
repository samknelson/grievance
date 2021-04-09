<?php

function sirius_node_tab_event_intake_access($event_node) {
	// If it's disabled at the event type level, not enabled
	if (!sirius_event_intake_enabled_tid($event_node->field_sirius_event_type['und'][0]['tid'])) { return FALSE; }

	// Admin user roles can access
	if (user_access('sirius admin') || user_access('sirius domain admin') || user_access('sirius event admin')) { return TRUE; }

	// Admin participant roles can access
	$participant_nid = sirius_event_participant_nid_from_user($event_node->nid);
	if (!$participant_nid) { return FALSE; }
	$participant_node = node_load($participant_nid);
	$type_json = sirius_event_type_json($event_node);
	$event_json = sirius_jsonfield_util_node_json_get($event_node);
	$prole_tid = $participant_node->field_sirius_event_prole['und'][0]['tid'];
	$admin_prole_tids = array_values($type_json['intake']['admin_prole_tids']);
	if (in_array($prole_tid, $admin_prole_tids)) { return TRUE; }

	// Oh well
	return FALSE;
}

function sirius_node_tab_event_intake($event_node) {
	$json = sirius_jsonfield_util_node_json_get($event_node);
	$code = $json['intake']['anonymous']['access_code'];

	global $base_url;
	$url = $base_url;
	$url .= '/node/' . $event_node->nid;
	$url .= '/sirius_event_intake/anonymous';
	$url .= "?access_code=$code";

	$html = '';

	$html .= "<strong>$url</strong>";
	$html .= sirius_qr_url($url);

	return $html;
}
