<?php

/**
 * Implements hook_sirius_sched_info
 */

function sirius_dispatch_sirius_sched_info() {
	$info['dispatch'] = array(
		'title' => 'Dispatch Schedule',
		'callback_denorm_check' => 'sirius_dispatch_sched_check',
		'callback_denorm' => 'sirius_dispatch_sched',
	);
	return $info;
}

/**
 * Called on every node save; determines whether we need to queue it for scheduling
 */

function sirius_dispatch_sched_check($dispatch_node) {
	// Just if it's a dispatch
  if ($dispatch_node->type != 'sirius_dispatch') { return FALSE; }
  return TRUE;
}


function sirius_dispatch_sched($dispatch_node) {
	$job_node = node_load($dispatch_node->field_sirius_dispatch_job['und'][0]['target_id']);
	$job_type_tid = $job_node->field_sirius_dispatch_job_type['und'][0]['tid'];
	$job_type_term = taxonomy_term_load($job_type_tid);
	$job_type_json = sirius_json_get($job_type_term);

  // Just if it's the right status
  $status = $dispatch_node->field_sirius_dispatch_status['und'][0]['value'];
  if (!in_array($status, array('accepted', 'accepted_secondary', 'resigned', 'vlri', 'terminated'))) { return array(); }

  // Just if it's supposed to show on the schedule
	$job_node = node_load($dispatch_node->field_sirius_dispatch_job['und'][0]['target_id']);
	if (!$job_node) { return FALSE; }
	$job_type_tid = $job_node->field_sirius_dispatch_job_type['und'][0]['tid'];
	$job_type_term = taxonomy_term_load($job_type_tid);
	$job_type_json = sirius_json_get($job_type_term);
	if ($job_type_json['plugin']['sched_noconflict']['settings']['show_on_sched'] != 'Yes') { return array(); }

	$worker_nid = $dispatch_node->field_sirius_worker['und'][0]['target_id'];
	if (!$worker_nid) { return array(); }

	$ts = sirius_fieldval($job_node, 'field_sirius_datetime', 'ts');
	$end_ts = sirius_fieldval($job_node, 'field_sirius_datetime_completed', 'ts');
	if (!$end_ts || ($end_ts <= $ts)) { $end_ts = NULL; }

  $status = $dispatch_node->field_sirius_dispatch_status['und'][0]['value'];

	$sched_json['display']['type'] = $job_type_term->name;
	$sched_json['display']['status'] = $status;
	$sched_json['display']['title_short'] = $job_type_term->name;
	$sched_json['display']['details'] = $job_node->title;
	$sched_json['display']['path'] = "node/" . $dispatch_node->nid . "/sirius_dispatch_manage";
	$sched_json['display']['granularity'] = 'min';
	$sched_json['job_nid'] = $job_node->nid;

	$sched = array(
		'entity_nid' => $worker_nid,
		'start_ts' => $ts,
		'json' => $sched_json,
	);
	if ($end_ts) { $sched['end_ts'] = $end_ts; }

	return array($sched);
}
