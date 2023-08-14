<?php

function sirius_sched_db_search($params) {
	if (!$params) { return; }

	$sql_args = array();
	$sql = "select * from sirius_sched ";
	$sql .= "where 1=1 ";
	foreach (array('id', 'entity_nid', 'handler_nid', 'type', 'start_ts', 'end_ts') as $key) {
		if (isset($params[$key])) {
			$sql .= "and $key = :$key ";
			$sql_args[":$key"] = $params[$key];
		}
	}
	if (isset($params['start_ts_min'])) {
		$sql .= "and start_ts >= :start_ts_min ";
		$sql_args[':start_ts_min'] = $params['start_ts_min'];
	}
	if (isset($params['start_ts_max'])) {
		$sql .= "and start_ts <= :start_ts_max ";
		$sql_args[':start_ts_max'] = $params['start_ts_max'];
	}
	if (isset($params['end_ts_min'])) {
		$sql .= "and end_ts >= :end_ts_min ";
		$sql_args[':end_ts_min'] = $params['end_ts_min'];
	}
	if (isset($params['end_ts_max'])) {
		$sql .= "and end_ts <= :end_ts_max ";
		$sql_args[':end_ts_max'] = $params['end_ts_max'];
	}


	if (isset($params['interval'])) {

		// We want any schedule entries which overlap a given interval. There are five possibilities.
		// Interval: [...]
		// Event:    O---O
		//
		//    .....[..........]......
		// 1. .....[....O.....]......  A
		// 2. .....[..O----O..]......  A
		// 3. .....[......O---]--O...  A
		// 4. ..O--[--O.......]......  B
		// 5. ..O--[----------]--O...  B
		//
		// But all of those possibilities are covered by the following two criteria:
		// A: The start date of the event falls within the interval, OR
		// B: The start date of the interval falls within the event
		//
		// Re: https://stackoverflow.com/questions/325933/determine-whether-two-date-ranges-overlap
		// there's actually a simpler implementation, but it's problematic to implement because we can't assume that our 
		// event has an end date. The interval will always have a start and end, but the event itself might have a NULL end.

		$sql .= "and ( ";
		$sql .= "     (:interval_start <= start_ts and start_ts <= :interval_end) ";  // The start date of the event falls within the interval
		$sql .= "  or (start_ts <= :interval_start and :interval_start <= end_ts) ";  // The start date of the interval falls within the event
		$sql .= ") ";
		$sql_args[':interval_start'] = $params['interval']['start_ts'];
		$sql_args[':interval_end'] = $params['interval']['end_ts'];
	}

	$order = $params['order'];
	if (!$order) { $order = "start_ts, end_ts, entity_nid"; }
	$limit = $params['limit'];
	if (!$limit) { $limit = 1000; }
	$offset = $params['offset'];
	if (!$offset) { $offset = 0; }

	$sql .= "order by $order limit $limit offset $offset ";
	$stmt = sirius_sql_query($sql, $sql_args);
	$scheds = array();
	while ($hr = $stmt->fetchAssoc()) {
		$hr['json'] = json_decode($hr['json'], TRUE);
		$scheds[] = $hr;
	}

	return $scheds;
}

function sirius_sched_db_delete($params) {
	if (!$params) { return; }

	$sql_args = array();
	$sql = "delete from sirius_sched ";
	$is_first = TRUE;
	foreach ($params as $key => $value) {
		if ($is_first) { $sql .= "where "; $is_first = FALSE; } else { $sql .= "and "; }
		$sql .= "$key = :$key";
		$sql_args[":$key"] = $value;
	}
	sirius_sql_query($sql, $sql_args);
}

function sirius_sched_db_insert($sched) {
	$keys = array();
	if ($sched['id']) { $keys[] = 'id'; }
	$keys[] = 'entity_nid';
	$keys[] = 'handler_nid';
	$keys[] = 'type';
	$keys[] = 'start_ts';
	$keys[] = 'end_ts';
	$keys[] = 'title';
	$keys[] = 'json';

	$sql_args = array();
	foreach ($keys as $key) { $sql_args[":$key"] = $sched[$key]; }
	$sql_args[':json'] = json_encode($sched['json']);

	$sql = "insert into sirius_sched (";
	$sql .= join(', ', $keys);
	$sql .= ") values (";
	$sql .= join(', ', array_keys($sql_args));
	$sql .= ") "; 
	sirius_sql_query($sql, $sql_args);
}

function sirius_sched_db_cleanup($mode = 'test') {
	$count = 0;

	sirius_minilog('Missing node scan: searching for schedule entries with handlers that have been deleted.');
	$sql = "select id, handler_nid from sirius_sched ";
	$sql .= "left join node on node.nid = sirius_sched.handler_nid ";
	$sql .= "where node.nid is null ";
	$sql .= "limit 1000 ";
	$stmt = sirius_sql_query($sql, array());
	$records = array();
	$count_missing = 0;
	while ($hr = $stmt->fetchAssoc()) { $records[] = $hr; }
	foreach ($records as $hr) {
		++$count_missing;
		++$count;
		$sched_id = $hr['id'];
		$nid = $hr['handler_nid'];
		sirius_minilog("-- Deleting schedule record $sched_id with handler $nid (mode = $mode)");
		if ($mode == 'live') {
			sirius_sched_db_delete(array('id' => $sched_id));
		} 
	}

	sirius_minilog('Bad timestamp scan: searching for schedule entries with timestamps that are missing or invalid.');
	$sql = "select id, handler_nid from sirius_sched ";
	$sql .= "left join node on node.nid = sirius_sched.handler_nid ";
	$sql .= "where start_ts is null "; 
	$sql .= "or (end_ts is not null and end_ts < start_ts) ";
	$sql .= "limit 1000 ";
	$stmt = sirius_sql_query($sql, array());
	$records = array();
	$count_badts = 0;
	while ($hr = $stmt->fetchAssoc()) { $records[] = $hr; }
	foreach ($records as $hr) {
		++$count_badts;
		++$count;
		$sched_id = $hr['id'];
		$nid = $hr['handler_nid'];
		sirius_minilog("-- Deleting schedule record $sched_id with handler $nid (mode = $mode)");
		if ($mode == 'live') {
			sirius_sched_db_delete(array('id' => $sched_id));
		} 
	}

	return array('success' => TRUE, 'count' => 0, 'msg' => "Deleted $count_missing records with missing nodes; deleted $count_badts records with bad timestamps.");
}
