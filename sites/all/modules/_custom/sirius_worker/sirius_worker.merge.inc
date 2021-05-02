<?php

function sirius_worker_merge($worker_nid, $from_worker_nid) {
	$worker_node = node_load($worker_nid);
	$from_worker_node = node_load($from_worker_nid);

	if (!$worker_node) { return array('success' => FALSE, 'msg' => t('Target is not a node.')); }
	if (!$from_worker_node) { return array('success' => FALSE, 'msg' => t('Source is not a node.')); }
	if ($from_worker_node->field_sirius_domain['und'][0]['target_id'] != $worker_node->field_sirius_domain['und'][0]['target_id']) { return array('success' => FALSE, 'msg' => t('Cannot merge records from different domains.')); }
	if ($worker_node->type != 'sirius_worker') { return array('success' => FALSE, 'msg' => t('Target is not a worker node.')); }
	if ($from_worker_node->type != 'sirius_worker') { return array('success' => FALSE, 'msg' => t('Source is not a worker node.')); }
	if ($from_worker_nid == $worker_nid) { return array('success' => FALSE, 'msg' => t('Cannot merge a worker with themself.')); }

	// Disable the user associated with the other worker.
	$from_user = sirius_worker_get_user($from_worker_nid);
	if ($from_user) {
		$from_user->status = 0;
		unset($from_user->field_sirius_worker['und']);
		$from_user->name = $from_worker_node->title . ' disabled ' . date('YmdHis');
		$from_user->mail = 'disabled_' . $from_worker_nid . '_'  . date('YmdHis') . '@' . variable_get('sirius_worker_dummy_domain', 'nodomain.com');
		user_save($from_user);
		sirius_minilog(t('Disabled user #@uid', array('@uid' => $from_user->uid)));
	}

	// Create a log entry
	$category = 'sirius_worker';
	$type = 'merge';
	$message = "Merged worker NID #$from_worker_nid";
	$summary = '';
	$notes = '';
	$handler_nids = array($worker_nid);
	$json = $from_worker_node;
	sirius_log($category, $type, $message, $summary, $notes, $handler_nids, $files, $json);
	sirius_minilog(t('Created a log entry with the original node record.'));

	// Anything which pointed to the old worker should point to the new worker.
	$sql = 'update field_data_field_sirius_worker set field_sirius_worker_target_id = :worker_nid where field_sirius_worker_target_id = :from_worker_nid';
	$stmt = db_query($sql, array(':worker_nid' => $worker_nid, ':from_worker_nid' => $from_worker_nid));
	$count = $stmt->rowCount();
	sirius_minilog("Executed: $sql / $count rows affected");

	$sql = 'update field_revision_field_sirius_worker set field_sirius_worker_target_id = :worker_nid where field_sirius_worker_target_id = :from_worker_nid';
	$stmt = db_query($sql, array(':worker_nid' => $worker_nid, ':from_worker_nid' => $from_worker_nid));
	$count = $stmt->rowCount();
	sirius_minilog("Executed: $sql / $count rows affected");

	$sql = 'update field_data_field_sirius_log_handler set field_sirius_log_handler_target_id = :worker_nid where field_sirius_log_handler_target_id = :from_worker_nid';
	$stmt = db_query($sql, array(':worker_nid' => $worker_nid, ':from_worker_nid' => $from_worker_nid));
	$count = $stmt->rowCount();
	sirius_minilog("Executed: $sql / $count rows affected");

	$sql = 'update field_revision_field_sirius_log_handler set field_sirius_log_handler_target_id = :worker_nid where field_sirius_log_handler_target_id = :from_worker_nid';
	$stmt = db_query($sql, array(':worker_nid' => $worker_nid, ':from_worker_nid' => $from_worker_nid));
	$count = $stmt->rowCount();
	sirius_minilog("Executed: $sql / $count rows affected");

	// Create a log entry
	$category = 'sirius_worker';
	$type = 'merge';
	$message = "Merged worker NID #$from_worker_nid";
	$summary = '';
	$notes = '';
	$handler_nids = array($worker_nid);
	$json = $from_worker_node;
	sirius_log($category, $type, $message, $summary, $notes, $handler_nids, $files, $json);
	sirius_minilog(t('Created a log entry with the original node record.'));

	// Delete the old worker
	node_delete($from_worker_nid);
	sirius_minilog(t('Deleted worker node #@nid', array('@nid' => $from_worker_nid)));

	// Resave the new worker
	node_save($worker_node);
	sirius_minilog(t('Updated worker node #@nid', array('@nid' => $worker_nid)));

	// Rebuild the hours caches
	sirius_hour_cache_set_worker($from_worker_nid);
	sirius_minilog(t('Rebuilt hours cache for #@nid', array('@nid' => $from_worker_nid)));
	sirius_hour_cache_set_worker($worker_nid);
	sirius_minilog(t('Rebuilt hours cache for #@nid', array('@nid' => $worker_nid)));

	return array('success' => TRUE, 'msg' => t('Records merged.'));
}