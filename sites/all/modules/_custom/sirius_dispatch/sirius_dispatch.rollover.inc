<?php

function sirius_dispatch_rollover_nid($employer_nid, $worker_nid, $cutoff) {
	$sql .= "select max(nid) as nid "; 
	$sql .= "from node, ";
	$sql .= "field_data_field_sirius_log_handler roll_h1, ";
	$sql .= "field_data_field_sirius_log_handler roll_h2, ";
	$sql .= "field_data_field_sirius_category roll_c ";
	$sql .= "where type = 'sirius_log' ";
	$sql .= "and created >= :cutoff ";
	$sql .= "and roll_h1.entity_type = 'node' and roll_h1.entity_id = nid and roll_h1.field_sirius_log_handler_target_id = :worker_nid ";
	$sql .= "and roll_h2.entity_type = 'node' and roll_h2.entity_id = nid and roll_h2.field_sirius_log_handler_target_id = :employer_nid ";
	$sql .= "and roll_c.entity_type = 'node' and roll_c.entity_id = nid and roll_c.field_sirius_category_value = 'dispatch:rollover' ";

	$args[':cutoff'] = $cutoff;
	$args[':employer_nid'] = $employer_nid;
	$args[':worker_nid'] = $worker_nid;

	$stmt = db_query($sql, $args);
	$hr = $stmt->fetchAssoc();
	$log_nid = $hr['nid'];
	if (!$log_nid) { return NULL; }
	return $log_nid;
}

function sirius_dispatch_rollover_dates($employer_nid, &$workers, $cutoff) {
	if (!$workers) { return; }

	$sql .= "select roll_h1.field_sirius_log_handler_target_id as worker_nid, field_sirius_json_value as json "; 
	$sql .= "from node, ";
	$sql .= "field_data_field_sirius_log_handler roll_h1, ";
	$sql .= "field_data_field_sirius_log_handler roll_h2, ";
	$sql .= "field_data_field_sirius_category roll_c, ";
	$sql .= "field_data_field_sirius_json roll_j ";
	$sql .= "where type = 'sirius_log' ";
	$sql .= "and created >= :cutoff ";
	$sql .= "and roll_h1.entity_type = 'node' and roll_h1.entity_id = nid and roll_h1.field_sirius_log_handler_target_id in (:worker_nids) ";
	$sql .= "and roll_h2.entity_type = 'node' and roll_h2.entity_id = nid and roll_h2.field_sirius_log_handler_target_id = :employer_nid ";
	$sql .= "and roll_c.entity_type = 'node' and roll_c.entity_id = nid and roll_c.field_sirius_category_value = 'dispatch:rollover' ";
	$sql .= "and roll_j.entity_type = 'node' and roll_j.entity_id = nid ";

	$args[':cutoff'] = $cutoff;
	$args[':employer_nid'] = $employer_nid;
	$args[':worker_nids'] = array_keys($workers);
	$stmt = db_query($sql, $args);
	while ($hr = $stmt->fetchAssoc()) {
		$json = json_decode($hr['json'], TRUE);
		$ts = strtotime($json['end_date']);
		if ($ts > 1) {
			$workers[$hr['worker_nid']]['msg'] .= "Rollover until: " . gmdate('l, F j, Y', $ts);
		}
	}
}