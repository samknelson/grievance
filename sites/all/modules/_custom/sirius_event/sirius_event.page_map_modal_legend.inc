<?php

function sirius_event_map_modal_legend_access($event_nid) {
	$event_node = node_load($event_nid);
	return sirius_node_tab_event_map_access($event_node);
}

function sirius_event_map_modal_legend($event_nid) {
	$event_node = node_load($event_nid);
	$event_json = sirius_jsonfield_util_node_json_get($event_node);
	$type_json = sirius_event_type_json($event_node);
	$prole_tid = $type_json['map']['prole_tid'];
	$pstatus_options = sirius_event_participant_role_status_tids($prole_tid, array('phonebank' => array('during_call' => TRUE, 'current_pstatus' => $pstatus_tid)));
	$pstatus_terms = taxonomy_term_load_multiple(array_keys($pstatus_options));
	$html = $event_json['map']['caller_intro'];
	$html .= sirius_event_map_legend_render($event_nid);
	return $html;
}
