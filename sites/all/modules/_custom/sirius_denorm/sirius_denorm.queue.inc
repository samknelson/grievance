<?php

function sirius_denorm_queue_execute_item($item, $mode = 'test') {
	sirius_minilog("Executing denorm item $item[denorm_id]");
	if ($mode != 'live') {
		sirius_minilog("... skipping because mode = $mode");
		return;
	}

	// Set it to "in progress". It will be set to complete at the very end of the queue run, 
	// to prevent infinite loops.
	$sql = "update sirius_denorm_queue set denorm_status = :status, denorm_updated_ts = :updated_ts where denorm_id = :denorm_id ";
	$stmt = db_query($sql, array(
		':status' => 'in_progress',
		':denorm_id' => $item['denorm_id'],
		':updated_ts' => time(), 
	));

	module_invoke_all('sirius_denorm', $item);
}

function sirius_denorm_queue_run($mode = 'test', $force = FALSE) {
	// return array('success' => TRUE, 'count' => 0);

	// INFINITE LOOP PROTECTION
	// While we are running a batch in the queue, we disable the queue altogether. This means
	// that new denorm items can't be generated during a queue run.
	// 
	// (This makes the system slightly less reliable, because we might be saving records without denormalizing them fully.)
	sirius_denorm_queue_enabled(FALSE);

	$limit = variable_get('sirius_denorm_batch_size', 1000);
	$last_ts = variable_get('sirius_denorm_last_ts', 0);
	$interval = variable_get('sirius_denorm_interval', 10*60);
	$now_ts = time();

	if ( ($now_ts - $last_ts < $interval) && !$force) {
		$msg = "Too soon to run (can only run every $interval seconds).";
		sirius_minilog($msg);
		return array('success' => FALSE, 'msg' => $msg);
	}

	variable_set('sirius_denorm_last_ts', $now_ts);

	// Turn off realtime processing. We don't want hook_postsave() to run on all the things that we've just saved
	sirius_denorm_realtime(FALSE);

	sirius_minilog("Denorm processing up to $limit items.");
	$items = sirius_denorm_queue_search(
		array(
			'status' => 'pending',
			'limit' => $limit,
		)
	);
	$count = 0;

	$total = count($items);
	foreach ($items as $item) {
		++$count;
		sirius_minilog("Executing demorm item $count / $total: $item[denorm_id]");
		sirius_denorm_queue_execute_item($item, $mode);
	}

	$count = 0;
	foreach ($items as $item) {
		++$count;
		sirius_minilog("Removing demorm item $count / $total: $item[denorm_id]");
		sirius_denorm_queue_remove($item['denorm_id'], $mode);
	}

	sirius_minilog("Executing denorm cleanup");
	sirius_denorm_queue_cleanup();

	return array('success' => TRUE, 'count' => $count);
}

function sirius_denorm_queue_remove($denorm_id, $mode = 'test') {
	sirius_minilog("Removing denorm item $denorm_id");

	if ($mode != 'live') {
		sirius_minilog("... skipping because mode = $mode");
		return;
	}

	$sql = "update sirius_denorm_queue set denorm_status = :status, denorm_updated_ts = :updated_ts where denorm_id = :denorm_id ";

	$stmt = db_query($sql, array(
		':updated_ts' => time(), 
		':status' => 'complete',
		':denorm_id' => $denorm_id,
	));
}

function sirius_denorm_queue_enabled($new_value = NULL) {
  static $enabled = TRUE;

  if (isset($new_value)) { $enabled = $new_value; }

  return $enabled;
}

function sirius_denorm_queue_add($source_nid, $target_nid, $trigger, $trigger_args = array()) {
	if (!sirius_denorm_queue_enabled()) { return NULL; }

	if (!$target_nid) { return NULL; }

	$trigger_args_encoded = json_encode($trigger_args);

	$probe = sirius_denorm_queue_search(
		array(
			'target_nid' => $target_nid, 
			'statuses' => array('pending', 'in_progress'),
			'trigger' => $trigger,
			'trigger_args' => $trigger_args_encoded,
		)
	);
	if ($probe) { return $probe[0]['denorm_id']; }

	$sql = "insert into sirius_denorm_queue(denorm_created_ts, denorm_updated_ts, denorm_source_nid, denorm_target_nid, denorm_trigger, denorm_trigger_args, denorm_status) ";
	$sql .= "values (:created_ts, :updated_ts, :source_nid, :target_nid, :trigger, :trigger_args, :status) ";

	$ts = time();

	$stmt = db_query($sql, array(
		':created_ts' => $ts, 
		':updated_ts' => $ts, 
		':source_nid' => $source_nid, 
		':target_nid' => $target_nid, 
		':trigger' => $trigger,
		':trigger_args' => $trigger_args_encoded,
		':status' => 'pending',
	), array('return' => Database::RETURN_INSERT_ID));

	$queue_id = $stmt;
	return $queue_id;
}

function sirius_denorm_queue_cleanup() {
	$sql = "delete from sirius_denorm_queue where denorm_status in ('complete', 'in_progress') and denorm_created_ts < :created_ts and denorm_updated_ts < :updated_ts ";
	$ts = time();
	$created_ts = $updated_ts = $ts - variable_get('sirius_denorm_cleanup_after', 60*60);
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
	if ($params['statuses']) {
		$sql .= "and denorm_status in (:statuses) ";
		$sql_params[':statuses'] = $params['statuses'];
	}
	if ($params['trigger']) {
		$sql .= "and denorm_trigger = :trigger ";
		$sql_params[':trigger'] = $params['trigger'];
	}
	if ($params['trigger_args']) {
		$sql .= "and denorm_trigger_args = :trigger_args ";
		$sql_params[':trigger_args'] = $params['trigger_args'];
	}
	if ($params['order']) {
		$sql .= "order by " . $params['order'] . ' ';
	} else {
		$sql .= "order by denorm_id ";
	}

	$limit = $params['limit'];
	if (!$limit) { $limit = 1000; }
	$sql .= "limit $limit ";

	$stmt = db_query($sql, $sql_params);
	$result = array();
	while ($hr = $stmt->fetchAssoc()) {
		$result[] = $hr;
	}
	return $result;
}

function sirius_denorm_queue_render($items) {
	if (!$items) { return '<p>' . t('No items found.') . '</p>'; }
	$header = array();
	$header[] = t('ID');
	$header[] = t('Created');
	$header[] = t('Updated');
	$header[] = t('Source');
	$header[] = t('Target');
	$header[] = t('Trigger');
	$header[] = t('Args');
	$header[] = t('Status');
	$header[] = t('Tools');

	$rows = array();
	foreach ($items as $item) {
		$tools = array();
		$tools[] = l('Source', 'node/' . $item['denorm_source_nid'], ['attributes' => ['class' => ['tool-button']]]);
		$tools[] = l('Target', 'node/' . $item['denorm_target_nid'], ['attributes' => ['class' => ['tool-button']]]);

		$row = array();
		$row[] = $item['denorm_id'];
		$row[] = date('Y-m-d H:i:s', $item['denorm_created_ts']);
		$row[] = date('Y-m-d H:i:s', $item['denorm_created_ts']);
		$row[] = $item['denorm_source_nid'];
		$row[] = $item['denorm_target_nid'];
		$row[] = $item['denorm_trigger'];
		$row[] = $item['denorm_trigger_args'];
		$row[] = $item['denorm_status'];
		$row[] = join('', $tools);
		$rows[] = $row;
	}

	$attributes = array('class' => array('sirius_denorm_queue'));
	return sirius_table_render($header, $rows, $attributes);
}

function sirius_denorm_queue_summary() {
	$sql = "select denorm_trigger, ";
	$sql .= "denorm_status, ";
	$sql .= "count(*) as record_count, ";
	$sql .= "min(denorm_created_ts) as first_ts ";
	$sql .= "from sirius_denorm_queue ";
	$sql .= "group by denorm_trigger, denorm_status ";
	$stmt = sirius_sql_query($sql, array());
	$summary = array();
	while ($hr = $stmt->fetchAssoc()) {
		$summary[$hr['denorm_trigger']][$hr['denorm_status']] = array(
			'count' => $hr['record_count'],
			'first_ts' => $hr['first_ts'],
		);
	}
	return $summary;
}

function sirius_denorm_queue_statuses() {
	return array(
		'pending' => t('Pending'),
		'in_progress' => t('In Progress'),
		'complete' => t('Complete'),
	);
}