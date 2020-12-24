<?php

function sirius_node_tab_event_map_access($event_node) {
	if (!sirius_event_map_enabled($event_node)) { return FALSE; } 

	if (user_access('sirius admin') || user_access('sirius event admin')) { return TRUE; }

	$participant_nid = sirius_event_participant_nid_from_user($event_node->nid);
	if (!$participant_nid) { return FALSE; }
	$participant_node = node_load($participant_nid);
	$type_json = sirius_event_type_json($event_node);
	$event_json = sirius_jsonfield_util_node_json_get($event_node);

	$prole_tid = $participant_node->field_sirius_event_prole['und'][0]['tid'];
	$caller_prole_tids = array_values($type_json['map']['caller_prole_tids']);
	if (!in_array($prole_tid, $caller_prole_tids)) { return FALSE; }

	return TRUE;
}

function sirius_node_tab_event_map($event_node) {
	$active = sirius_map_active($event_node);

	if (!$active['success']) { drupal_set_message($active['msg']); }

	$event_json = sirius_jsonfield_util_node_json_get($event_node);
	$type_json = sirius_event_type_json($event_node);
	$prole_tid = $type_json['map']['prole_tid'];
	if (!$prole_tid) {
		return 'Please go to the settings page and select the mapping role.';
	}

	$html .= $event_json['map']['caller_intro'];
	$html .= '<br />';
	$html .= sirius_fakerender_field('Current Time:', date('l, F d Y - g:ia'));
	$html .= '<br />';
	$html .= sirius_dispatch_view_render_field($event_node, 'field_sirius_daterepeat', 'Mapping Access Opens');
	$html .= '<br />';

	$html .= sirius_event_map_legend_render($event_node->nid);

	if ($active['success']) { 
		$html .= l(t('View Map'), 'node/' . $event_node->nid . '/sirius_event_map/view',  array('attributes' => array('class' => array('tool-button'))));
	}

	return $html;
}

