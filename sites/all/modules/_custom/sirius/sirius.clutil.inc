<?php

// drush --user=1 php-eval "require(drupal_get_path('module', 'sirius') . '/sirius.clutil.inc'); sirius_minilog_echo_active(TRUE); sirius_clutil_optimize_all();"

function sirius_clutil_optimize_all() {
	sirius_minilog_echo_active(TRUE);
	$sql = "show tables";
	$stmt = db_query($sql, array());
	$tables = array();
	while ($hr = $stmt->fetchAssoc()) {
		$tables[] = reset(array_values($hr));
	}

	$total_reduction = 0;
	foreach ($tables as $table) {
		sirius_minilog("Examining table: $table");

		$sql = "SELECT data_length, index_length FROM information_schema.TABLES where table_name = '$table'";
		$stmt = db_query($sql, array());
		$hr = $stmt->fetchAssoc();
		$data_length = round($hr['data_length'] / (1024*1024), 2);
		$index_length = round($hr['data_length'] / (1024*1024), 2);
		$total_length = $data_length + $index_length;
		sirius_minilog("... Before optimize: $total_length MB ($data_length data and $index_length index)");

		sirius_minilog("... Optimizing ...");
		$sql = "OPTIMIZE TABLE $table";
		$stmt = db_query($sql, array());

		$sql = "SELECT data_length, index_length FROM information_schema.TABLES where table_name = '$table'";
		$stmt = db_query($sql, array());
		$hr = $stmt->fetchAssoc();
		$new_data_length = round($hr['data_length'] / (1024*1024), 2);
		$new_index_length = round($hr['data_length'] / (1024*1024), 2);
		$new_total_length = $new_data_length + $new_index_length;
		$reduction = ($data_length - $new_data_length);
		$total_reduction += $reduction;
		sirius_minilog("... After optimize: $new_total_length MB ($new_data_length data and $new_index_length index)");
		sirius_minilog("... Size reduction: $reduction (Total $total_reduction)");
	}

	sirius_minilog("=== TOTAL REDUCTION $total_reduction ===");
}

// drush --user=1 php-eval "require(drupal_get_path('module', 'sirius') . '/sirius.clutil.inc'); sirius_clutil_log_purge('test', 'worker:member_status', NULL, 2468132);"

function sirius_clutil_log_purge($mode, $category, $type = NULL, $handler_nid = NULL) {
	sirius_minilog_echo_active(TRUE);

	$sql_args = array();
	$sql = "select count(distinct nid) as c from node ";
	$sql .= "left join field_data_field_sirius_category on field_data_field_sirius_category.entity_type = 'node' and field_data_field_sirius_category.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_type on field_data_field_sirius_type.entity_type = 'node' and field_data_field_sirius_type.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_log_handler on field_data_field_sirius_log_handler.entity_type = 'node' and field_data_field_sirius_log_handler.entity_id = node.nid ";
	$sql .= "where type = 'sirius_log' ";
	$sql .= "and field_sirius_category_value = :category ";
	$sql_args[':category'] = $category;
	if ($type) {
		$sql .= "and field_sirius_type_value = :type ";
		$sql_args[':type'] = $type;
	}
	if ($handler_nid) {
		$sql .= "and field_sirius_log_handler_target_id = :handler_nid ";
		$sql_args[':handler_nid'] = $handler_nid;
	}
	$stmt = db_query($sql, $sql_args);
	$hr = $stmt->fetchAssoc();
	$grand_total = $hr['c'];
	sirius_minilog("Grand total: " . number_format($grand_total, 0));


	$sql_args = array();
	$sql = "select distinct nid from node ";
	$sql .= "left join field_data_field_sirius_category on field_data_field_sirius_category.entity_type = 'node' and field_data_field_sirius_category.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_type on field_data_field_sirius_type.entity_type = 'node' and field_data_field_sirius_type.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_log_handler on field_data_field_sirius_log_handler.entity_type = 'node' and field_data_field_sirius_log_handler.entity_id = node.nid ";
	$sql .= "where type = 'sirius_log' ";
	$sql .= "and field_sirius_category_value = :category ";
	$sql_args[':category'] = $category;
	if ($type) {
		$sql .= "and field_sirius_type_value = :type ";
		$sql_args[':type'] = $type;
	}
	if ($handler_nid) {
		$sql .= "and field_sirius_log_handler_target_id = :handler_nid ";
		$sql_args[':handler_nid'] = $handler_nid;
	}
	$sql .= "order by nid limit 1000 ";

	$nids = array();
	$stmt = db_query($sql, $sql_args);
	while ($hr = $stmt->fetchAssoc()) {
		$nids[] = $hr['nid'];
	}

	$count = 0;
	$count_total = count($nids);
	foreach ($nids as $nid) {
		++$count;
		sirius_minilog("Deleting $count / $count_total / $grand_total: $nid... ");
		if ($mode == 'live') {
			node_delete($nid);
		} else {
			sirius_minilog("... Skipping because mode = $mode");
		}
	}
}

// while true ; do  drush --user=1 php-eval "sirius_require('sirius', 'sirius.clutil.inc'); sirius_clutil_delete_all_revisions(10000);" ; sleep 10 ; done
function sirius_clutil_delete_all_revisions($limit = 10000, $offeset = 0, $prefix = 'NOPREFIX') {

	$sql = "SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED;";
	$stmt = db_query($sql, array());

	$sql = "select count(*) as c ";
	$sql .= "from node_revision ";
	$stmt = db_query($sql, array());
	$hr = $stmt->fetchAssoc();
	$grand_total = $hr['c'];

	$sql = "select node_revision.vid ";
	$sql .= "from node, node_revision ";
	$sql .= "where node.nid = node_revision.nid ";
	$sql .= "and node.vid != node_revision.vid ";
	$sql .= "order by node_revision.vid limit $limit offset $offeset";
	$stmt = db_query($sql, array());

	$vids = array();
	while ($hr = $stmt->fetchAssoc()) {
		$vids[] = $hr['vid'];
	}

	$sql = "SET TRANSACTION ISOLATION LEVEL REPEATABLE READ ;";
	$stmt = db_query($sql, array());


	$total = count($vids);
	$start_ts = time();
	foreach ($vids as $vid) {
		++$count;
		$result = node_revision_delete($vid);

		$now_ts = time();
		$elapsed_ts = $now_ts - $start_ts;
		if ($count) {
			$per_ts = $elapsed_ts / $count;
			$remain_ts = ($total - $count) * $per_ts;
			$str = "Estimate " . format_interval($elapsed_ts + $remain_ts);
		}
		if (!($count % 100)) {
			print "$prefix: $count / $total / " . number_format($grand_total, 0) . ": $vid ($result) $str\n";
		}
	}
}
