<?php

//
// UNDER DEVELOPMENT DO NOT USE
//

function sirius_node_tab_event_section_sort_access($event_node) {
	return sirius_node_tab_event_section_settings_access($event_node);
}

function sirius_node_tab_event_section_sort($event_node) {
  sirius_event_disclaimer_redirect($event_node);
 	return drupal_get_form('sirius_node_tab_event_section_sort_form', $event_node);
}

function sirius_node_tab_event_section_sort_form($form, &$form_state, $event_node) {
	$form_state['event_nid'] = $event_node->nid; 

	$json = sirius_jsonfield_util_node_json_get($event_node);
	$section_list = $json['section_list'];
	if (!$section_list) { $section_list = array(); }
	uksort($section_list, function($a, $b) use ($section_list) { return $section_list[$a]['weight'] <=> $section_list[$b]['weight']; });

	return $form;
}

function sirius_node_tab_event_section_sort_form_submit($form, &$form_state) {
	$event_nid = $form_state['event_nid'];
	$event_node = node_load($event_nid);
	$json = sirius_jsonfield_util_node_json_get($event_node);

	$section_list = array();
	$section_list_input = $form_state['values']['section_list'];
	if (!$section_list_input) { $section_list_input = array(); }

	foreach ($section_list_input as $section) {
		if (!$section['date'] && !$section['title']) { continue; }
    if ($section['delete']) { continue; }
		$uuid = $section['uuid'];
		if (!$uuid) { 
			$uuid = uuid_generate();
			$section['uuid'] = $uuid;
		}
		// $section['ts'] = strtotime($section['date']);
		$section_list[$uuid] = $section;
	}

	$json['section_list'] = $section_list;
	sirius_jsonfield_util_node_json_set($event_node, $json);
	node_save($event_node);
	drupal_set_message("Sections saved.");
}


