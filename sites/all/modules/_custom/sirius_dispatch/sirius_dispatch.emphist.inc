<?php

function sirius_dispatch_emphist_search($worker_nid, $types = array('Primary'), $params = array()) {
	$params['worker_nid'] = $worker_nid;
	$params['types'] = $types;
	$result = sirius_dispatch_search($params);
	return $result;
}

function sirius_dispatch_emphist_record_from_node($dispatch_node) {
	$record = array();
	$record['dispatch_nid'] = $dispatch_node->nid;
	$record['worker_nid'] = $dispatch_node->field_sirius_worker['und'][0]['target_id'];
	$record['type'] = $dispatch_node->field_sirius_dispatch_type['und'][0]['value'];
	$record['job_nid'] = $dispatch_node->field_sirius_dispatch_job['und'][0]['target_id'];
	$job_node = node_load($record['job_nid']);
	$record['employer_nid'] = $job_node->field_grievance_shop['und'][0]['target_id'];

	$ts = sirius_fieldval($dispatch_node, 'field_sirius_date_start', 'ts');
	if ($ts === NULL) { 
		$record['start'] = NULL;
	} else {
		$record['start'] = gmdate('Y-m-d', $ts);
	}

	$ts = sirius_fieldval($dispatch_node, 'field_sirius_date_end', 'ts');
	if ($ts === NULL) { 
		$record['end'] = NULL;
	} else {
		$record['end'] = gmdate('Y-m-d', $ts);
	}
	return $record;
}

function sirius_dispatch_emphist_render($emphist) {
	if (!$emphist) {
		$results_html = "<em>" . t("No employment history found.") . "</em>";
	}

	// Add the job and employer to our results, for sorting and rendering
	foreach ($emphist as $dispatch_nid => $dispatch_node) {
		$emphist[$dispatch_nid]->job_node = node_load($dispatch_node->field_sirius_dispatch_job['und'][0]['target_id']);
		$emphist[$dispatch_nid]->employer_node = node_load($emphist[$dispatch_nid]->job_node->field_grievance_shop['und'][0]['target_id']);
	}

	/*
	// Work in progress ... I started playing with rendering a timeline for each employer,
	// then decided not to work on it any more until I had feedback on what we actually want.

	// Restructure the data: broken out by employer, then by primary / secondary, then by date
	$data = array();
	foreach ($emphist as $dispatch_nid => $dispatch_node) {
		$employer_title = $dispatch_node->employer_node->title;
		$type = $dispatch_node->field_sirius_dispatch_type['und'][0]['value'];
		$start_ts = sirius_fieldval($dispatch_node, 'field_sirius_date_start', 'ts');
		if ($start_ts === NULL) { $start_date = ''; } else { $start_date = gmdate('Y-m-d', $start_ts); }
		$end_ts = sirius_fieldval($dispatch_node, 'field_sirius_date_end', 'ts');
		if ($end_ts === NULL) { $end_date = ''; } else { $end_date = gmdate('Y-m-d', $end_ts); }

		$data[$employer_title][$start_date][] = array('type' => $type, 'start' => TRUE);

		if ($end_date) { $data[$employer_title][$end_date][] = array('type' => $type, 'start' => FALSE); }
	}

	$html = '';
	foreach ($data as $employer_name => $types) {
		$html .= '<table class="sirius_table">';
		$html .= '<tr>';
		$html .= '<th colspan="2">';
		$html .= $employer_name;
		$html .= '</th>';
		$html .= '</tr>';

		$html .= '<tr>';
		$html .= '<th width="50%">';
		$html .= t('Primary');
		$html .= '</th>';
		$html .= '<th width="50%">';
		$html .= t('Secondary');
		$html .= '</th>';
		$html .= '</table>';
		$html .= '</tr>';

		$dates = array_keys($)
	}
	return $html;
	*/

	// sort
	uksort($emphist, function($a, $b) use ($emphist) { 
		// First by employer
		$c = strcmp($emphist[$a]->employer_node->title, $emphist[$b]->employer_node->title);
		if ($c) { return $c; }

		// Next by type
		$c = strcmp($emphist[$a]->field_sirius_dispatch_type['und'][0]['value'], $emphist[$b]->field_sirius_dispatch_type['und'][0]['value']);
		if ($c) { return $c; }

		// Next by start date, reverse order
		$ts1 = sirius_fieldval($emphist[$a], 'field_sirius_date_start', 'ts');
		$ts2 = sirius_fieldval($emphist[$b], 'field_sirius_date_start', 'ts');
		$c = ($ts2 <=> $ts1);
		if ($c) { return $c; }

		// Finally sort by nid
		return $emphist[$a]->nid <=> $emphist[$b]->nid;
	});

	$rows = array();
	foreach ($emphist as $dispatch_nid => $dispatch_node) {
		$job_node = $dispatch_node->job_node;
		$employer_node = $dispatch_node->employer_node;
		$type = $dispatch_node->field_sirius_dispatch_type['und'][0]['value'];

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

		if ($type == 'Primary') {
			if ($start_date && !$end_date) {
				$class = 'green';
			} else {
				$class = 'white';
			}
		} else {
			$class = 'grey';
		}

		$tools = '';
		$tools .= sirius_tool_link("node/$dispatch_nid/sirius_dispatch_emphist", 'Edit');

		$row = array();
		$row[] = $employer_node->title;
		$row[] = sirius_term_title($job_node->field_sirius_industry['und'][0]['tid']);
		$row[] = $type;
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

	return array('success' => TRUE, 'msg' => 'Looks good.', 'dispatch_node' => $dispatch_node);
}

function sirius_dispatch_emphist_employer_nid_asof($worker_nid, $ts, $options = array()) {
	$records = sirius_dispatch_emphist_search($worker_nid, array('Primary'), $params = array('ts' => $ts, 'limit' => 1));
	if (!$records) { return NULL; }
	$dispatch_node = reset($records);
	$job_node = node_load($dispatch_node->field_sirius_dispatch_job['und'][0]['target_id']);
	$employer_nid = $job_node->field_grievance_shop['und'][0]['target_id'];
	return $employer_nid;
}