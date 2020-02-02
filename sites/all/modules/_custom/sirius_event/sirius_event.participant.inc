<?php

function sirius_event_participant_pstatus_set_possible($participant_node, $prole_tid, $pstatus_tid) {
	return array('success' => TRUE);	
}

function sirius_event_participant_pstatus_set($participant_node, $prole_tid, $pstatus_tid) {
	$possible = sirius_event_participant_pstatus_set_possible($participant_node, $prole_tid, $pstatus_tid);
	if (!$possible['success']) { return $possible; }

	if (!$prole_tid) { $prole_tid = $participant_node->field_sirius_event_prole['und'][0]['tid']; }
	if (!$pstatus_tid) { $pstatus_tid = $participant_node->field_sirius_event_pstatus['und'][0]['tid']; }

	if ($prole_tid == $participant_node->field_sirius_event_prole['und'][0]['tid'] && 
			$pstatus_tid == $participant_node->field_sirius_event_pstatus['und'][0]['tid']) {
		// return array('success' => TRUE, 'msg' => 'No changes.');
	}

	$participant_node->field_sirius_event_prole['und'][0]['tid'] = $prole_tid;
	$participant_node->field_sirius_event_pstatus['und'][0]['tid'] = $pstatus_tid;

	global $user;
	static $term_cache;
	if (!$term_cache[$prole_tid]) { $term_cache[$prole_tid] = taxonomy_term_load($prole_tid); }
	if (!$term_cache[$pstatus_tid]) { $term_cache[$pstatus_tid] = taxonomy_term_load($pstatus_tid); }
	sirius_jsonfield_log($participant_node, 'status', "Set prole = " . $term_cache[$prole_tid]->name . ", pstatus = " . $term_cache[$pstatus_tid]->name);
	node_save($participant_node);
}