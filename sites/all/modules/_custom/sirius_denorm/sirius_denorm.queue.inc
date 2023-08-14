<?php

function sirius_denorm_queue_execute_item($item) {
	module_invoke_all('sirius_denorm', $item);
}

function sirius_denorm_queue_run() {
	$items = sirius_denorm_queue_search(array('status' => 'pending'));
	foreach ($items as $item) {
		sirius_denorm_queue_execute_item($item);
		sirius_denorm_queue_remove($item['denorm_id']);
	}
	sirius_denorm_queue_cleanup();
}

function sirius_denorm_queue_remove($denorm_id) {
	$sql = "update sirius_denorm_queue set denorm_status = :status, denorm_updated_ts = :updated_ts where denorm_id = :denorm_id ";

	$ts = time();
	$stmt = db_query($sql, array(
		':updated_ts' => $ts, 
		':status' => 'complete',
		':denorm_id' => $denorm_id,
	));
}

function sirius_denorm_queue_add($source_nid, $target_nid, $trigger) {
	if (!$target_nid) { return NULL; }

	$probe = sirius_denorm_queue_search(array('target_nid' => $target_nid, 'status' => 'pending', 'trigger' => $trigger));
	if ($probe) { return $probe[0]['denorm_id']; }

	$sql = "insert into sirius_denorm_queue(denorm_created_ts, denorm_updated_ts, denorm_source_nid, denorm_target_nid, denorm_trigger, denorm_status) ";
	$sql .= "values (:created_ts, :updated_ts, :source_nid, :target_nid, :trigger, :status) ";

	$ts = time();

	$stmt = db_query($sql, array(
		':created_ts' => $ts, 
		':updated_ts' => $ts, 
		':source_nid' => $source_nid, 
		':target_nid' => $target_nid, 
		':trigger' => $trigger,
		':status' => 'pending',
	), array('return' => Database::RETURN_INSERT_ID));

	$queue_id = $stmt;
	return $queue_id;
}

function sirius_denorm_queue_cleanup() {
	$sql = "delete from sirius_denorm_queue where denorm_status = 'complete' and denorm_created_ts < :created_ts and denorm_updated_ts < :updated_ts ";
	$ts = time();
	$created_ts = $updated_ts = $ts- 24*60*60;
	$stmt = db_query($sql, array(
		':created_ts' => $created_ts,
		':updated_ts' => $updated_ts,
	));
}

function sirius_denorm_queue_search($params) {
	$sql_params = array();

	$sql = "select * from sirius_denorm_queue ";
	$sql .= "where 1 = 1 ";
	if ($params['source_nid']) {
		$sql .= "and denorm_source_nid = :source_nid ";
		$sql_params[':source_nid'] = $params['source_nid'];
	}
	if ($params['target_nid']) {
		$sql .= "and denorm_target_nid = :target_nid ";
		$sql_params[':target_nid'] = $params['target_nid'];
	}
	if ($params['denorm_id']) {
		$sql .= "and denorm_id = :denorm_id ";
		$sql_params[':denorm_id'] = $params['denorm_id'];
	}
	if ($params['status']) {
		$sql .= "and denorm_status = :status ";
		$sql_params[':status'] = $params['status'];
	}
	if ($params['trigger']) {
		$sql .= "and denorm_trigger = :trigger ";
		$sql_params[':trigger'] = $params['trigger'];
	}
	if ($params['order']) {
		$sql .= "order by " . $params['order'] . ' ';
	} else {
		$sql .= "order by denorm_id ";
	}
	$sql .= "limit 1000 ";

	$stmt = db_query($sql, $sql_params);
	$result = array();
	while ($hr = $stmt->fetchAssoc()) {
		$result[] = $hr;
	}
	return $result;
}

function sirius_denorm_queue_render($items) {
	$header = array('ID', 'Created', 'Updated', 'Source', 'Target', 'Trigger', 'Status');
	$rows = array();
	foreach ($items as $item) {
		$row = array();
		$row[] = $item['denorm_id'];
		$row[] = date('Y-m-d H:i:s', $item['denorm_created_ts']);
		$row[] = date('Y-m-d H:i:s', $item['denorm_created_ts']);
		$row[] = l($item['denorm_source_nid'], 'node/' . $item['denorm_source_nid']);
		$row[] = l($item['denorm_target_nid'], 'node/' . $item['denorm_target_nid']);
		$row[] = $item['denorm_trigger'];
		$row[] = $item['denorm_status'];
		$rows[] = $row;
	}

	$attributes = array('class' => array('sirius_denorm_queue'));
	return theme_table(array('header' => $header, 'rows' => $rows, 'attributes' => $attributes));
}