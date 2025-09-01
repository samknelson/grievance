<?php

function sirius_worker_schedule($worker_nid) {
	$params = array();
	$params['entity_nid'] = $worker_nid;
	if ($_REQUEST['all']) { return sirius_sched_search($params); }

	$ts = time();
	$params['interval'] = array('start_ts' => $ts, 'end_ts' => $ts + 60*60*24*30);

	return sirius_sched_search($params);
}

function sirius_worker_schedule_render($sched, $format = 'full') {
	return sirius_sched_render($sched, $format);
}

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

	if ($end_date) {
		$end_date .= '23:59:59 ';
		$end_ts = strtotime($end_date);
	} else {
		// $end_date = date('Y-m-d', $start_ts); }
		// $end_ts = strtotime($end_date);
		$end_ts = NULL;
	}

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
