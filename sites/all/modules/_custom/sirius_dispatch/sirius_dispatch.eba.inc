<?php

function sirius_dispatch_eba_dates_upcoming() {
	$dates = array();
	for ($i=0; $i<8; ++$i) {
		$ts = strtotime("+$i days");
		$date = date('Y-m-d', $ts);
		$label = date('l, F j', $ts);
		$dates[$date] = $label;
	}
	return $dates;
}

function sirius_dispatch_eba_dates_worker($worker_node) {
	$dates = array();
	if ($worker_node->field_sirius_dispatch_eba_dates['und']) {
		foreach ($worker_node->field_sirius_dispatch_eba_dates['und'] as $tmp) {
			$date = date('Y-m-d', strtotime($tmp['value']));
			$dates[$date] = $date;
		}
	}
	sort(array_unique($dates));
	return $dates;
}

/**
 * Clean up EBA:
 * - Remove any dates in the past
 * - If there are no dates in the future, set EBA = "No"
 */

function sirius_dispatch_eba_cleanup($mode = 'test') {
	// Find any EBA dates in the past
	$date = date('Y-m-d 00:00:00', strtotime('-1 days'));

	$sql = "select distinct nid from node ";
	$sql .= "left join field_data_field_sirius_dispatch_eba on field_data_field_sirius_dispatch_eba.entity_type = 'node' and field_data_field_sirius_dispatch_eba.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_dispatch_eba_dates on field_data_field_sirius_dispatch_eba_dates.entity_type = 'node' and field_data_field_sirius_dispatch_eba_dates.entity_id = node.nid ";
	$sql .= "where node.type = 'sirius_worker' ";
	$sql .= "and ( ";
	// We have an old date, or
	$sql .= "  (field_sirius_dispatch_eba_dates_value <= :date) or ";
	// We have a "Yes" but no dates, or 
	$sql .= "  (field_sirius_dispatch_eba_dates_value is null and field_sirius_dispatch_eba_value = 'Yes') or ";
	// We have a "No" with dates
	$sql .= "  (field_sirius_dispatch_eba_dates_value is not null and (field_sirius_dispatch_eba_value = 'No' or field_sirius_dispatch_eba_value is null) ) ";
	$sql .= ") ";
	$sql .= "order by node.nid ";
	$stmt = db_query($sql, array(':date' => $date));
	$worker_nids = array();
	while ($hr = $stmt->fetchAssoc()) {
		$worker_nids[] = $hr['nid'];
	}

	$count = 0;
	foreach ($worker_nids as $worker_nid) {
		++$count;
		$result = sirius_dispatch_eba_cleanup_worker($worker_nid, $mode);
	}

	return array('success' => TRUE, 'msg' => "Cleaned up $count workers.", 'count' => $count);
}

function sirius_dispatch_eba_cleanup_worker($worker_nid, $mode = 'test') {
	$worker_node = node_load($worker_nid); 
	$date = date('Y-m-d 00:00:00', strtotime('-1 days'));
	$msg = "EBA cleanup for: " . $worker_node->title . ' (' . $worker_nid . '): ';

	// Remove any old dates
	$new_dates = array();
	if ($worker_node->field_sirius_dispatch_eba_dates) {
		foreach ($worker_node->field_sirius_dispatch_eba_dates['und'] as $tmp) {
			if ($tmp['value'] > $date) { 
				$new_dates['und'][] = $tmp;
			} else {
				$msg .= " Removed old EBA date " . gmdate('Y-m-d', strtotime($tmp['value']));
				$need_save = TRUE;
			}
		}

		$worker_node->field_sirius_dispatch_eba_dates = $new_dates;
	}

	// Can't be "Yes" without a date.
	if ($worker_node->field_sirius_dispatch_eba['und'][0]['value'] == 'Yes' && !$new_dates) {
		$worker_node->field_sirius_dispatch_eba['und'][0]['value'] = 'No';
		$msg .= "Worker has no upcoming EBA dates, setting EBA to 'No'. ";
		$need_save = TRUE;
	}

	// Can't be "No" if we have dates
	if ($worker_node->field_sirius_dispatch_eba['und'][0]['value'] != 'Yes' && $new_dates) {
		$worker_node->field_sirius_dispatch_eba['und'][0]['value'] = 'No';
		unset($worker_node->field_sirius_dispatch_eba_dates);
		$msg .= "Worker's EBA status is 'No', clearing dates. ";
		$need_save = TRUE;
	}

	if (!$need_save) {
		$msg .= "No updated needed. ";
		return array('success' => TRUE, 'msg' => $msg);
	}

	sirius_minilog($msg);
	if ($mode == 'live') {
		node_save($worker_node);
		sirius_log('worker:eba', 'cleanup', $msg, '', '', array($worker_node->nid), array());
	} else {
		sirius_minilog("(Skipping save because mode = $mode)");
	}


	return array('success' => TRUE, 'msg' => $msg);
}

