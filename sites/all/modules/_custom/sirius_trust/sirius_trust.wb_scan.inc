<?php

function sirius_trust_worker_benefit_scan($mode = 'test', $force = FALSE) {
	$count = 0;
	sirius_minilog("Worker Benefit Scan Starting");

	$status = sirius_domain_variable_get('sirius_trust_wb_scan_status', array());

	// Get our limit
	$limit = $status['limit'];
	if (!$limit || !is_int($limit+0)) { $limit = $status['limit'] = 10; }

	// Make sure we're in the right month. If we're not, reset.
	$month = $status['month'];
	if ($month == 'next_month') {
		$ym = date('Y-m', strtotime(date('Y-m-15', strtotime('+1 month'))));
	} else {
		$ym = date('Y-m');
	}

	if ($ym != $status['ym']) {
		sirius_minilog("Starting month $ym. Resetting the count.");
		$status['ym'] = $ym;
		$status['last_worker_nid'] = 0;
	}

	// Get the last worker NID.
	$last_worker_nid = $status['last_worker_nid'];
	if (!$last_worker_nid) { $last_worker_nid = 0; }
	sirius_minilog("Continuing after worker NID #$last_worker_nid");

	$sql_args = array();
	$sql_args[':domain_nid'] = sirius_domain_nid();

	$sql = "select count(nid) as c from node ";
	$sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
	$sql .= "where node.type = 'sirius_worker' ";
	$sql .= "and field_sirius_domain_target_id = :domain_nid ";
	$stmt = sirius_sql_query($sql, $sql_args);
	$hr = $stmt->fetchAssoc();
	$status['total'] = $hr['c'];

	$sql_args[':last_worker_nid'] = $last_worker_nid;

	$sql = "select count(nid) as c from node ";
	$sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
	$sql .= "where node.type = 'sirius_worker' ";
	$sql .= "and field_sirius_domain_target_id = :domain_nid ";
	$sql .= "and nid <= :last_worker_nid ";
	$stmt = sirius_sql_query($sql, $sql_args);
	$hr = $stmt->fetchAssoc();
	$status['complete'] = $hr['c'];

	$sql = "select count(nid) as c from node ";
	$sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
	$sql .= "where node.type = 'sirius_worker' ";
	$sql .= "and field_sirius_domain_target_id = :domain_nid ";
	$sql .= "and nid > :last_worker_nid ";
	$stmt = sirius_sql_query($sql, $sql_args);
	$hr = $stmt->fetchAssoc();
	$status['pending'] = $hr['c'];

	$sql = "select nid from node ";
	$sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
	$sql .= "where node.type = 'sirius_worker' ";
	$sql .= "and field_sirius_domain_target_id = :domain_nid ";
	$sql .= "and nid > :last_worker_nid ";
	$sql .= "order by nid ";
	$sql .= "limit $limit ";
	$stmt = sirius_sql_query($sql, $sql_args);
	$worker_nids = array();
	while ($hr = $stmt->fetchAssoc()) {
		$worker_nids[] = $hr['nid'];
	}

	$count = 0;
	$total = count($worker_nids);
	if (!$total) {
		sirius_minilog("No workers found; we must have scanned them all.");
		return array('success' => TRUE, 'count' => 0);
	}

	sirius_minilog("Status: " . print_r($status, 1));
	sirius_minilog("Processing the next batch of workers ($total records)");

	// "Scan mode" if we want to run this function in live mode (to generate change entries) 
	// but aren't ready to actually terminate benefits

	$scan_mode = $status['mode'];
	if ($mode != 'live') {
		sirius_minilog("Forcing scan mode $mode");
		$scan_mode = $mode;
	}

	foreach ($worker_nids as $worker_nid) {
		++$count;
		sirius_minilog("Processing worker $count / $total: $worker_nid");

		sirius_minilog_indent();
		sirius_trust_wb_scan_worker($scan_mode, node_load($worker_nid), array('month' => $status['month']));
		sirius_minilog_outdent();

		$status['last_worker_nid'] = $worker_nid;
		++$status['complete'];
		--$status['pending'];
		if ($mode == 'live') {
			sirius_domain_variable_set('sirius_trust_wb_scan_status', $status);
		}
	}

	sirius_minilog("Worker Benefit Scan Complete");
	return array('success' => TRUE, 'count' => $count);
}

function sirius_trust_wb_scan_worker($mode, $worker_node, $options = array()) {
	if ($options['month'] == 'next_month') {
		$ts = strtotime(date('Y-m-15', strtotime('+1 month')));
	} else {
		$ts = strtotime(date('Y-m-15'));
	}
	$options['ts'] = $ts;

	sirius_minilog("Worker Benefit Scan: " . $worker_node->title . " as of " . date('Y-m-d', $ts));

	sirius_minilog("Worker Benefit Scan: START Benefits:  " . $worker_node->title);
	sirius_minilog_indent();
	$result = sirius_trust_wb_scan_worker_start($mode, $worker_node, $options);
	sirius_minilog_outdent();
	if ($result['msg']) { sirius_minilog($result['msg']); }

	sirius_minilog("Worker Benefit Scan: STOP Benefits: " . $worker_node->title);
	sirius_minilog_indent();
	$result = sirius_trust_wb_scan_worker_stop($mode, $worker_node, $options);
	if ($result['msg']) { sirius_minilog($result['msg']); }
	sirius_minilog_outdent();

	sirius_minilog("Worker Benefit Scan: Complete: " . $worker_node->title);
}