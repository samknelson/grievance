<?php

// drush --user=1 php-eval "sirius_require('sirius_teamsters631', 'sirius_teamsters631.oneoff_ws_history.inc'); Sirius_Teamsters631_Oneoff_Ws_History::run('test');"

class Sirius_Teamsters631_Oneoff_Ws_History {
	public static function run($mode = 'test') {
		sirius_minilog_echo_active(TRUE);
		sirius_minilog("Cleanup starting, mode = $mode");

		global $teamsters63_flag;
		$teamsters63_flag = TRUE;

		$start_ts = time();

		$worker_nids = self::get_worker_nids($mode);
		$count = count($worker_nids);
		global $oneoff_grand_total;


		sirius_minilog("Found " . count($worker_nids) . " affected workers.");
		$delta = 0;
		foreach ($worker_nids as $worker_nid) {
			++$delta;
			self::fix($mode, $worker_nid, $delta, $count, $oneoff_grand_total);
		}

		$end_ts = time();

		if ($delta && $oneoff_grand_total) {
			$interval = $end_ts - $start_ts;
			$time_per_record = $interval / $delta;
			$remaining = ($oneoff_grand_total-$delta) * $time_per_record;
			sirius_minilog("=== === === Completed $delta / $oneoff_grand_total records in $interval seconds, estimate " . format_interval($remaining) . " remaining === === ===");
		}
	}

	public static function fix($mode, $worker_nid, $delta, $count, $grand_total) {	
		sirius_minilog("=== FIXING: $delta / $count / $grand_total: $worker_nid (" . sirius_node_title($worker_nid) . ") ===");

		$worker_node = node_load($worker_nid);
		$old_ws_tid = $worker_node->field_sirius_work_status['und'][0]['tid'];
		$result = sirius_worker_history_calc($worker_node, 'worker:work_status');

		$worker_node = node_load($worker_nid);
		if ($old_ws_tid != $worker_node->field_sirius_work_status['und'][0]['tid']) {
			sirius_minilog("************* HISTORY MISMATCH: SETTING CURRENT WORK STATUS " . sirius_term_title($old_ws_tid));
			sirius_worker_work_status_set($worker_node, $old_ws_tid, 'Current work status does not match history', array());
		}

		sirius_minilog($result['msg']);

		sirius_minilog("... Done.");
	}

	public static function get_worker_nids($mode = 'test') {
		$sql = "select count(*) as c ";
		$sql .= "from node ";
		$sql .= "left join field_data_field_sirius_json on field_data_field_sirius_json.entity_type = 'node' and field_data_field_sirius_json.entity_id = node.nid ";
		$sql .= "left join field_data_field_sirius_work_status on field_data_field_sirius_work_status.entity_type = 'node' and field_data_field_sirius_work_status.entity_id = node.nid ";
		$sql .= "where node.type = 'sirius_worker' ";
		$sql .= "and field_sirius_work_status_tid is not null ";
		$sql .= "and ( ";
		$sql .= " json_unquote(json_extract(field_data_field_sirius_json.field_sirius_json_value, '\$.history')) is null ";
		$sql .= " or json_unquote(json_extract(field_data_field_sirius_json.field_sirius_json_value, '\$.history')) = '' ";
		$sql .= ") ";
		$sql .= "order by nid desc ";
		$stmt = db_query($sql, array());
		$hr = $stmt->fetchAssoc();
		global $oneoff_grand_total;
		$oneoff_grand_total = $hr['c'];


		$sql = "select nid, json_unquote(json_extract(field_data_field_sirius_json.field_sirius_json_value, '\$.history')) as raw_history ";
		$sql .= "from node ";
		$sql .= "left join field_data_field_sirius_json on field_data_field_sirius_json.entity_type = 'node' and field_data_field_sirius_json.entity_id = node.nid ";
		$sql .= "left join field_data_field_sirius_work_status on field_data_field_sirius_work_status.entity_type = 'node' and field_data_field_sirius_work_status.entity_id = node.nid ";
		$sql .= "where node.type = 'sirius_worker' ";
		$sql .= "and field_sirius_work_status_tid is not null ";
		$sql .= "and ( ";
		$sql .= " json_unquote(json_extract(field_data_field_sirius_json.field_sirius_json_value, '\$.history')) is null ";
		$sql .= " or json_unquote(json_extract(field_data_field_sirius_json.field_sirius_json_value, '\$.history')) = '' ";
		$sql .= ") ";
		$sql .= "order by nid desc ";
		$sql .= "limit 100 ";
		$stmt = db_query($sql, array());

		$count = 0;
		$worker_nids = array();
		while ($hr = $stmt->fetchAssoc()) {
			$worker_nid = $hr['nid'];
			++$count;
			$worker_nids[] = $worker_nid;
			sirius_minilog("Found worker: $worker_nid");
		}

		return $worker_nids;
	}
}