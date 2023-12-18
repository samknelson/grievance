<?php

/**
 * @todo: Someday maybe there will be a base class for Sirius logging, and this class will extend it.
 */

function sirius_edls_log_instance() {
	return Sirius_Edls_Dblog::getInstance();
}

function sirius_edls_dblog_cleanup($mode = 'test') {
	return sirius_edls_log_instance()->cleanup($mode);
}

class Sirius_Edls_Dblog extends Sirius_Singleton {
	public function log(
		$category,
		$type,
		$message,
		$summary,
		$notes,
		$handler_nids,
		$files = NULL,
		$json = NULL
	) {
		global $user;
		if ($user) { $uid = $user->uid; } else { $uid = 0; }
		$ts = time();

		$sql = 'insert into sirius_edls_dblog ';
		$sql .= '(log_uid, log_ts, log_category, log_type, log_handler_nid, log_msg, log_json) ';
		$sql .= 'values ';
		$sql .= '(:uid, :ts, :category, :type, :handler_nid, :msg, :json) ';

		$sql_args = array();
		$sql_args[':uid'] = $uid;
		$sql_args[':ts'] = $ts;
		$sql_args[':category'] = $category;
		$sql_args[':type'] = $type;
		$sql_args[':handler_nid'] = $handler_nids[0];
		$sql_args[':msg'] = $message;
		$sql_args[':json'] = json_encode($json);
		$stmt = db_query($sql, $sql_args);
	}

	public function assignment_log($type, $assignment) {
		return $this->log(
	    'edls:assignment',
	    $type,
	    "$type: " . $assignment['worker_name'] . ' (' . $assignment['worker_id'] . ')',
	    NULL,
	    NULL,
	    array($assignment['sheet_nid']),
	    NULL,
	    array('assignment' => $assignment)
	  );
	}

	public function fetch($sheet_nid, $limit = 0, $offset = 0) {
		if (!$limit) { $limit = 1000; }
		$sql = "select * from sirius_edls_dblog where log_handler_nid = :sheet_nid order by log_ts desc limit $limit offset $offset";
		$stmt = db_query($sql, array(':sheet_nid' => $sheet_nid));

		$result = array();
		while ($hr = $stmt->fetchAssoc()) {
			$json = json_decode($hr['log_json'], TRUE);
			if (!$json) { $json = array(); }
			$hr['log_json'] = $json;
			$result[] = $hr;
		}

		return $result;
	}

	public function purge($log_id) {
		$sql = "delete from sirius_edls_dblog where log_id = :log_id";
		$stmt = db_query($sql, array(':log_id' => $log_id));
	}

	public function type_list($category) {
		return array(
			'assign' => t('Assign'),
			'update' => t('Update'),
			'unassign' => t('Unassign'),
		);
	}

	public function cleanup($mode) {
		// Clean up log entries where the node no longer exists
		$sql = "select log_id from sirius_edls_dblog ";
		$sql .= "left join node on node.nid = log_handler_nid ";
		$sql .= "where nid is null ";
		$sql .= "order by log_id limit 5000 ";
		$stmt = db_query($sql, array());

		$count = 0;
		while ($hr = $stmt->fetchAssoc()) {
			++$count;
			$log_id = $hr['log_id'];
			if ($mode == 'live') {
				sirius_minilog("Purging log entry $log_id");
				$this->purge($log_id);
			} else {
				sirius_minilog("Purging log entry $log_id (skipping purge because mode = $mode)");
			}
		}

		return array('success' => TRUE, 'count' => $count);
	}

}
