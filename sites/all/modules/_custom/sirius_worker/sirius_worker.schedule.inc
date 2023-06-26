<?php

function sirius_worker_schedule($worker_nid) {
	$params = array();
	$params['entity_nid'] = $worker_nid;
	if ($_REQUEST['all']) { return sirius_sched_search($params); }

	$ts = strtotime(date('Y-m-d 00:00:00'));
	$params['interval'] = array('start_ts' => $ts, 'end_ts' => $ts + 60*60*24*30);

	return sirius_sched_search($params);
}

function sirius_worker_schedule_render($sched, $format = 'full') {
	return sirius_sched_render($sched, $format);
}


function sirius_worker_schedule_legacy($worker_nid) {
	// Lots of lookups. Let's cache them.
	$lookups = &drupal_static(__FUNCTION__);
	$ts_now = time();

	// Get the contact
	$contact_nid = sirius_contact_get_nid_from_worker($worker_nid);
	if (!$contact_nid) { return array(); }

	// Results array
	$schedule = array();

	// Event Schedule. 
	// @todo: This loads all the event and participant nodes for all the contacts. We should probably refactor to use a raw SQL. sirius_event_participants_by_contact() is very expensive.

	$events_by_type = sirius_event_participants_by_contact($contact_nid);
	if ($events_by_type) {
		if (!isset($lookups['prole_terms'])) { $lookups['prole_terms'] = sirius_taxonomy_load('sirius_event_participant_role', 'tid', 'full'); }
		if (!isset($lookups['pstatus_terms'])) { $lookups['pstatus_terms'] = sirius_taxonomy_load('sirius_event_participant_status', 'tid', 'full'); }

		foreach ($events_by_type as $event_type_tid => $records) {
			foreach ($records as $delta => $record) {
				$pstatus_tid = $record['participant_node']->field_sirius_event_pstatus['und'][0]['tid'];
				$json = sirius_json_get($lookups['pstatus_terms'][$pstatus_tid]);
				if ($json['schedule']['display']['active'] != 'Yes') { continue; }

				if (!$record['event_node']->field_sirius_daterepeat['und'][0]) { continue; }
				foreach ($record['event_node']->field_sirius_daterepeat['und'] as $delta => $d) {
					$ts1 = sirius_date_ts($record['event_node'], 'field_sirius_daterepeat', $delta, 'value');
					$ts2 =  sirius_date_ts($record['event_node'], 'field_sirius_daterepeat', $delta, 'value2');

					$is_good = ($ts1 >= $ts_now || $ts2 >= $ts_now);
					if (!$is_good) { continue; }

					$schedule_entry = array();
					$schedule_entry['type'] = 'Event';
					$schedule_entry['title'] = $json['schedule']['display']['title'];
					$schedule_entry['title_short'] = $json['schedule']['display']['title_short'];
					$schedule_entry['handler_nid'] = $record['event_node']->nid;
					$schedule_entry['handler_title'] = $record['event_node']->title;
					$schedule_entry['start_ts'] = $ts1; 
					$schedule_entry['start_date'] = date('Y-m-d H:i:s', $ts1);
					if ($ts2 > 0) { 
						$schedule_entry['end_ts'] = $ts2; 
						$schedule_entry['end_date'] = date('Y-m-d H:i:s', $ts2);
					}
					$schedule[$ts1] = $schedule_entry;
				}
			}
		}
	}

	// Time-off requests
	// @todo: Big query, probably should optimize
	$query_params = array(
		'worker_nid' => $worker_nid,
		// 'statuses' => array('requested', 'approved')
	);
	$tor_nodes = sirius_worker_schedule_tor_list($query_params);

	if ($tor_nodes) {
		$tor_type_lookup = sirius_taxonomy_load('sirius_worker_tor_type', 'tid', 'full');
		foreach ($tor_nodes as $tor_nid => $tor_node) {
			$tor_json = sirius_json_get($tor_node);

			// Only interested if the TOR is in the future.
			$ts1 = strtotime($tor_json['tor']['start_date']);
			$ts2 = strtotime($tor_json['tor']['end_date']);
			$is_good = ($ts1 >= $ts_now || $ts2 >= $ts_now || $ts2 <= 0);
			if (!$is_good) { continue; }

			// Only interested if the status entry should appear on the schedule
			$status = sirius_worker_schedule_tor_status($tor_node);
			if ($status['schedule'] != 'Yes') { continue; }

			$schedule_entry = array();

			$tor_type_name_short = 'TOR';
			$tor_type_tid = $tor_json['tor']['type_tid'];
			$tor_type_term = $tor_type_lookup[$tor_type_tid];
			if ($tor_type_term && $tor_type_term->field_sirius_name_short['und'][0]['value']) {
				$tor_type_name_short = $tor_type_term->field_sirius_name_short['und'][0]['value'];
			}

			$title = $status['title'];

			$schedule_entry['type'] = $tor_type_name_short;
			$schedule_entry['title'] = $title;
			$schedule_entry['status'] = $tor_json['tor']['status'];
			$schedule_entry['title_short'] = $tor_type_name_short;
			$schedule_entry['handler_nid'] = $tor_node->nid;
			$schedule_entry['handler_title'] = $tor_json['tor']['summary'];
			$schedule_entry['start_ts'] = $ts1; 
			$schedule_entry['start_date'] = date('Y-m-d H:i:s', $ts1);
			$schedule_entry['end_ts'] = $ts2; 
			$schedule_entry['end_date'] = date('Y-m-d H:i:s', $ts2);
			$schedule_entry['path'] = "node/" . $tor_node->nid . "/sirius_worker_schedule_tor";
			$schedule[$ts1] = $schedule_entry;
		}
	}

	ksort($schedule);
	return $schedule;
}


/******************************************************************************************
 * NEW IMPLEMENTATION STUFF
 * ****************************************************************************************/

/**
 * Implements hook_sirius_sched_info
 */

function sirius_worker_sirius_sched_info() {
	$info['worker_tor'] = array(
		'title' => 'Worker TOR records',
		'callback_denorm_check' => 'sirius_worker_sched_tor_check',
		'callback_denorm' => 'sirius_worker_sched_tor',
	);
	return $info;
}

/**
 * Called on every node save; determines whether we need to queue it for scheduling
 */

function sirius_worker_sched_tor_check($tor_node) {
	// Just if it's a log entry
  if ($tor_node->type != 'sirius_log') { return FALSE; }

  // Must be of the right type
	if ($tor_node->field_sirius_category['und'][0]['value'] != 'tor') { return FALSE; }

	// Must have a "show on schedule" status
	$status = sirius_worker_schedule_tor_status($tor_node);
	if ($status['schedule'] != 'Yes') { return FALSE; }

	return TRUE;
}


function sirius_worker_sched_tor($tor_node) {
	$worker_node = sirius_log_handler($tor_node, 'sirius_worker');
	if (!$worker_node) { return; }

	$json = sirius_json_get($tor_node);

	// Get the start_ts and end_ts. This is a little funky:
	// - If end_ts is missing, making it one year in the future
	// - Since these are dates, we assume that the day starts and ends at midnight, in the timezone of the current user.

	$start_date = $json['tor']['start_date'];
	if (!$start_date) { return; }
	$start_date .= ' 00:00:00';
	$start_ts = strtotime($start_date);

	$end_date = $json['tor']['end_date'];
	if (!$end_date) { $end_date = date('Y-m-d', $start_ts); }
	$end_date .= '23:59:59 ';
	$end_ts = strtotime($end_date);

	$sched_json = array();
	$tor_json = sirius_json_get($tor_node);

	$status = sirius_worker_schedule_tor_status($tor_node);

	$tor_type_name_short = 'TOR';
	$tor_type_tid = $tor_json['tor']['type_tid'];
	$tor_type_term = taxonomy_term_load($tor_type_tid);
	if ($tor_type_term && $tor_type_term->field_sirius_name_short['und'][0]['value']) {
		$tor_type_name_short = $tor_type_term->field_sirius_name_short['und'][0]['value'];
	}

	$sched_json['display']['type'] = $tor_type_name_short;
	$sched_json['display']['status'] = $status['title'];
	$sched_json['display']['title_short'] = $tor_type_name_short;
	$sched_json['display']['details'] = $tor_json['tor']['summary'];
	$sched_json['display']['path'] = "node/" . $tor_node->nid . "/sirius_worker_schedule_tor";

	$sched = array(
		'entity_nid' => $worker_node->nid,
		'start_ts' => $start_ts,
		'end_ts' => $end_ts,
		'json' => $sched_json,
	);

	return array($sched);
}
