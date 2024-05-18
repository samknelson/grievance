<?php

/**
 * Implements hook_sirius_sched_info
 */

function sirius_event_sirius_sched_info() {
	$info['event'] = array(
		'title' => 'Participant Records',
		'callback_denorm_check' => 'sirius_event_sched_check',
		'callback_denorm' => 'sirius_event_sched',
	);
	return $info;
}

/**
 * Called on every node save; determines whether we need to queue it for scheduling
 */

function sirius_event_sched_check($participant_node) {
	// Just if it's a participant node
  if ($participant_node->type != 'sirius_event_participant') { return FALSE; }

  // Must have a worker
	$contact_nid = $participant_node->field_sirius_contact['und'][0]['target_id'];
	if (!$contact_nid) { return FALSE; }
	$worker_nid = sirius_worker_get_by_contact_nid($contact_nid, TRUE);
	if (!$worker_nid) { return FALSE; }

	// Not all statuses show on the worker's schedule. 
	$pstatus_tid = $participant_node->field_sirius_event_pstatus['und'][0]['tid'];
	$pstatus_term = taxonomy_term_load($pstatus_tid);
	$pstatus_json = sirius_json_get($pstatus_term);
	if ($pstatus_json['schedule']['display']['active'] != 'Yes') { return FALSE; }

	// Load the event
	$event_node = node_load($participant_node->field_sirius_event['und'][0]['target_id']);
	if (!$event_node) { return FALSE; }
	if (!$event_node->field_sirius_daterepeat['und'][0]) { return FALSE; }

	return TRUE;
}


function sirius_event_sched($participant_node) {
  // Must have a worker
	$contact_nid = $participant_node->field_sirius_contact['und'][0]['target_id'];
	$worker_nid = sirius_worker_get_by_contact_nid($contact_nid, TRUE);

	// Not all statuses show on the worker's schedule. 
	$pstatus_tid = $participant_node->field_sirius_event_pstatus['und'][0]['tid'];
	$pstatus_term = taxonomy_term_load($pstatus_tid);
	$pstatus_json = sirius_json_get($pstatus_term);

	// Load the event
	$event_node = node_load($participant_node->field_sirius_event['und'][0]['target_id']);
	$event_type_tid = $event_node->field_sirius_event_type['und'][0]['tid'];
	$event_type_term = taxonomy_term_load($event_type_tid);

	// Hooray, we can create some schedule entries.
	$scheds = array();
	foreach ($event_node->field_sirius_daterepeat['und'] as $delta => $d) {
		$ts1 = sirius_date_ts($event_node, 'field_sirius_daterepeat', $delta, 'value');
		if (!$d['value2']) {
			$ts2 = $ts1 + 60*60;
		} else {
			$ts2 = sirius_date_ts($event_node, 'field_sirius_daterepeat', $delta, 'value2');
		}

		$title_short = $pstatus_json['schedule']['display']['title_short'];
		if (!$title_short) { $status_display = $pstatus_term->name; }

		$sched = array();
		$sched['entity_nid'] = $worker_nid;
		$sched['start_ts'] = $ts1; 
		$sched['end_ts'] = $ts2; 
		$sched['json']['display']['type'] = 'Event';
		$sched['json']['display']['status'] = $pstatus_term->name;
		$sched['json']['display']['title_short'] = $title_short;
		$sched['json']['display']['details'] = $event_node->title;
		$scheds[] = $sched;
	}
  return $scheds;
}
