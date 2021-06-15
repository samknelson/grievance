<?php

function sirius_event_class_enabled_nid($event_nid) { return sirius_event_class_enabled(node_load($event_nid)); }

function sirius_event_class_enabled($event_node) { return sirius_event_class_enabled_tid($event_node->field_sirius_event_type['und'][0]['tid']); }

function sirius_event_class_enabled_tid($event_type_tid) { return sirius_event_class_enabled_type(taxonomy_term_load($event_type_tid)); }

function sirius_event_class_enabled_type($event_type_term) {
  $json = sirius_jsonfield_util_term_json_get($event_type_term);
  return $json['class']['enabled'] == 'Yes';
}

function sirius_event_class_ts($event_node) {
	$ts = sirius_date_ts($event_node, 'field_sirius_daterepeat');
	$signin_opens_self = $ts - variable_get('sirius_dispatch_class_signin_opens', 60*60*24*5);
	$signin_opens_others = $ts - variable_get('sirius_dispatch_class_signin_opens_others', 60*60);
	$signin_closes = $ts + variable_get('sirius_dispatch_class_signin_grace', 60*15);

	return array(
		'job_ts' => $ts,
		'signin_opens_self' => $signin_opens_self,
		'signin_opens_others' => $signin_opens_others,
		'signin_closes' => $signin_closes,
	);
}

function sirius_event_get_class_types() {
	$class_type_terms = &drupal_static(__FUNCTION__);

	if (!isset($class_type_terms)) {
		$event_type_terms = sirius_taxonomy_load('sirius_event_type', 'tid', 'full');
		$class_type_terms = array();
		foreach ($event_type_terms as $event_type_tid => $event_type_term) {
			if (sirius_event_class_enabled_type($event_type_term)) { $class_type_terms[$event_type_tid] = $event_type_term; }
		}
	}

	return $class_type_terms;
}



function sirius_event_class_auto_skill($participant_node) {
	// Contact
	$contact_nid = $participant_node->field_sirius_contact['und'][0]['target_id'];
	if (!$contact_nid) { return; }

	// Worker
	$worker_node = sirius_worker_get_by_contact_nid($contact_nid);
	if (!$worker_node) { return; }

	// Class settings
	$event_node = node_load($participant_node->field_sirius_event['und'][0]['target_id']);
	if (!$event_node) { return; }
	$event_type_term = taxonomy_term_load($event_node->field_sirius_event_type['und'][0]['tid']);
	$event_type_json = sirius_json_get($event_type_term);
	if (!$event_type_json || !$event_type_json['class']['skills']['skills'] || !$event_type_json['class']['skills']['paccess']) { return; }
	$skill_tids = array_filter(array_values($event_type_json['class']['skills']['skills']));
	if (!$skill_tids) { return; }
	$paccess = array_filter(array_values($event_type_json['class']['skills']['paccess']));
	if (!$paccess) { return; }

	// Did I pass?
	$my_prole_tid = $participant_node->field_sirius_event_prole['und'][0]['tid'];
	$my_pstatus_tid = $participant_node->field_sirius_event_pstatus['und'][0]['tid'];
	$my_paccess = "$my_prole_tid|$my_pstatus_tid";
	if (!in_array($my_paccess, $paccess)) { return; }
	$my_pstatus_term = taxonomy_term_load($my_pstatus_tid);

	// Yay. Grant the skills. We need to look up expiration dates.
	$skill_terms = taxonomy_term_load_multiple($skill_tids);
	foreach ($skill_terms as $skill_tid => $skill_term) {
		$days = $skill_term->field_grievance_days['und'][0]['value'];
		if ($days) { $expires = gmdate('m/d/Y', strtotime("+$days days")); } else { $expires = NULL; } 
		$msg = "Achieved status " . $my_pstatus_term->name . " in class " . $event_node->title;

		sirius_worker_skill_set($worker_node, array($skill_tid), $expires, 'add', $msg, array(), NULL, $participant_node->nid);
	}
}
