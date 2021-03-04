<?php

function sirius_node_tab_event_section_access($event_node) {
	// If we can access the settings page, we're good
	if (sirius_node_tab_event_section_settings_access($event_node)) { return TRUE; }

	// If it's disabled, deny access
	if (!sirius_event_section_enabled($event_node)) { return FALSE; }

	// Get our participant role
	$participant_nid = sirius_event_participant_nid_from_user($event_node->nid);
	if (!$participant_nid) { return FALSE; }
	$participant_node = node_load($participant_nid);
	$prole_tid = $participant_node->field_sirius_event_prole['und'][0]['tid'];
	if (!$prole_tid) { return FALSE; }

	// Get available participant roles
	$event_json = sirius_jsonfield_util_node_json_get($event_node);
	if (!$event_json['section']['prole_tids']) { return FALSE; }
	$prole_tids = array_filter(array_values($event_json['section']['prole_tids']));

	// Are we one of the available participant roles?
	if (in_array($prole_tid, $prole_tids)) { return TRUE; }

	// Oh well
	return FALSE;
}

function sirius_node_tab_event_section($event_node) {
	return drupal_get_form('sirius_node_tab_event_section_form', $event_node);
}

function sirius_node_tab_event_section_form($form, &$form_state, $event_node) {
	$form_state['event_nid'] = $event_node->nid;

	$participant_nid = sirius_event_participant_nid_from_user($event_node->nid);
	$participant_node = node_load($participant_nid);
	$participant_json = sirius_jsonfield_util_node_json_get($participant_node);

	$counts = sirius_event_section_counts($event_node);
	$options_attributes = array();
	$options = sirius_event_section_options($event_node, $counts, $options_attributes);
	unset($options_attributes[$participant_json['section']['uuid']]);

	$form['#tree'] = TRUE;

	$form['section']['uuid'] = array(
		'#type' => 'select',
		'#options' => $options,
		'#title' => t('Section'),
		'#default_value' => $participant_json['section']['uuid'],
		'#options_attributes' => $options_attributes,
	);

	$event_json = sirius_jsonfield_util_node_json_get($event_node);
	foreach ($options as $uuid => $title) {
		$section = $event_json['section_list'][$uuid];
		if (!$section || !$section['comments']) { continue; }
		$form['section'][$uuid]['comments'] = array(
			'#type' => 'item',
			'#markup' => '<strong>' . sirius_event_section_title($section) . ':</strong> ' . $section['comments'],
			'#states' => array('visible' => array(":input[name=\"section[uuid]\"]" => array('value' => $uuid))),
		);
	}

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;
}

function sirius_node_tab_event_section_form_submit($form, &$form_state) {
	$event_node = node_load($form_state['event_nid']);
	$participant_nid = sirius_event_participant_nid_from_user($event_node->nid);
	if (!$participant_nid) { 
		drupal_set_message("You are not registered for this event.", 'error'); 
		return;
	}
	$uuid = $form_state['values']['section']['uuid'];
	$result = sirius_event_section_assign($participant_nid, $uuid);
	if ($result['success']) { drupal_set_message($result['msg']); } else { drupal_set_message($result['msg'], 'warning'); }
}