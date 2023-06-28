<?php

function sirius_dispatch_emphist_search($worker_nid, $types = array('Primary')) {
	$params = array();
	$params['worker_nid'] = $worker_nid;
	$params['types'] = $types;
	return sirius_dispatch_search($params);
}

function sirius_dispatch_emphist_render($emphist) {
	if (!$emphist) {
		$results_html = "<em>" . t("No employment history found.") . "</em>";
	}

	$rows = array();
	foreach ($emphist as $dispatch_nid => $dispatch_node) {
		$job_node = node_load($dispatch_node->field_sirius_dispatch_job['und'][0]['target_id']);

		$ts = sirius_fieldval($dispatch_node, 'field_sirius_date_start', 'ts');
		if ($ts === NULL) { 
			$start_date = '';
		} else {
			$start_date = gmdate('Y-m-d', $ts);
		}

		$ts = sirius_fieldval($dispatch_node, 'field_sirius_date_end', 'ts');
		if ($ts === NULL) { 
			$end_date = '';
		} else {
			$end_date = gmdate('Y-m-d', $ts);
		}

		if ($start_date) {
			if ($end_date) { $ts = strtotime($end_date); } else { $ts = time(); }
			$interval = format_interval($ts - strtotime($start_date) + (24*60*60));
		} else {
			$interval = '';
		}

		if ($start_date && !$end_date) {
			$class = 'green';
		} else {
			$class = 'gray';
		}

		$tools = '';
		$tools .= sirius_tool_link("node/$dispatch_nid/sirius_dispatch_emphist", 'Edit');

		$row = array();
		$row[] = sirius_node_title($job_node->field_grievance_shop['und'][0]['target_id']);
		$row[] = sirius_term_title($job_node->field_sirius_industry['und'][0]['tid']);
		$row[] = $dispatch_node->field_sirius_dispatch_type['und'][0]['value'];
		$row[] = $start_date;
		$row[] = $end_date;
		$row[] = $interval;
		$row[] = $tools;

		$rows[] = array('data' => $row, 'class' => array($class));
	}

	$header = array();
	$header[] = t('Employer');
	$header[] = t('Industry');
	$header[] = t('Type');
	$header[] = t('Start Date');
	$header[] = t('End Date');
	$header[] = t('Duration');
	$header[] = t('Tools');

	return sirius_table_render($header, $rows);
}

function sirius_dispatch_emphist_save($record, $dispatch_nid = NULL) {
	if (!$record['start']) {
		return array('success' => FALSE, 'msg' => 'Must have a start date.');
	}
	$start_ts = strtotime($record['start'] . ' 00:00:00');

	if ($record['end']) {
		$end_ts = strtotime($record['end'] . ' 23:59:59');
	} else {
		$start_ts === NULL;
	}

	// Can't end before it starts
	if ( ($start_ts !== NULL) && ($end_ts !== NULL) && ($start_ts >= $end_ts)) {
		return array('success' => FALSE, 'msg' => 'End date must be after the start date.');
	}

	// Must have a worker
	$worker_nid = $record['worker_nid'];
	if ($dispatch_nid) {
		$dispatch_node = node_load($dispatch_nid);
		$worker_nid = $dispatch_node->field_sirius_worker['und'][0]['target_id'];
	}
	if (!$worker_nid) { $worker_nid = $record['worker_nid']; }
	if (!$worker_nid) { return array('success' => FALSE, 'msg' => 'No worker.'); }

	// Check to see if our new record conflicts with any existing records.
	if ($record['type'] == 'Primary') {
		$emphist = sirius_dispatch_emphist_search($worker_nid);
		unset($emphist[$dispatch_nid]);
		foreach ($emphist as $probe_dispatch_nid => $probe_dispatch_node) {
			if (!$probe_dispatch_node->field_sirius_date_start['und'][0]['value']) { return array('success' => FALSE, 'msg' => 'There is a conflicting employment history record (a record with no start date).'); }

			$probe_start_ts = strtotime($probe_dispatch_node->field_sirius_date_start['und'][0]['value']);
			$probe_end_ts = strtotime("+1 day", strtotime($probe_dispatch_node->field_sirius_date_end['und'][0]['value']));

			if ($start_ts < $probe_start_ts) {
				if ($end_ts === NULL || $end_ts > $probe_start_ts) {
					return array('success' => FALSE, 'msg' => 'There is a conflicting employment history (a record which starts before this entry ends).');
				}
			} else if ($start_ts < $probe_end_ts) {
				return array('success' => FALSE, 'msg' => 'There is a conflicting employment history (a record which overlaps this entry\'s start date).');
			}
		}
	}

	$now_ts = time();
	if (
			($start_ts <= $now_ts) && 
			(
				!$record['end'] || 
				($end_ts >= $now_ts)
			)
		) {
		if ($record['type'] == 'Primary') {
			$status = 'accepted';
		} else {
			$status = 'accepted_secondary';
		}
	} else {
		$status = 'terminated';
	}

	if (!$dispatch_node) {
		$job_node = node_load($record['job_nid']);
		$result = sirius_dispatch_actions_create($job_node, $worker_nid, 'No', $status);
		if (!$result['success']) { return $result; }
		$dispatch_node = $result['dispatch_node'];
	}

	if ($record['start']) {
		$dispatch_node->field_sirius_date_start['und'][0]['value'] = $record['start'];
	} else {
		unset($dispatch_node->field_sirius_date_start['und']);
	}

	if ($record['end']) {
		$dispatch_node->field_sirius_date_end['und'][0]['value'] = $record['end'];
	} else {
		unset($dispatch_node->field_sirius_date_end['und']);
	}

	$dispatch_node->field_sirius_dispatch_type['und'][0]['value'] = $record['type'];
	$dispatch_node->field_sirius_dispatch_status['und'][0]['value'] = $status;

	node_save($dispatch_node);

	return array('success' => TRUE, 'msg' => 'Looks good.');
}

