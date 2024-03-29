<?php

// drush --user=1 php-eval "require(drupal_get_path('module', 'sirius') . '/sirius.clutil.inc'); sirius_minilog_echo_active(TRUE); sirius_clutil_optimize_all();"

function sirius_clutil_optimize_all() {
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
		print "Deleting $count / $count_total: $nid... ";
		if ($mode == 'live') {
			node_delete($nid);
			print "Ok.";
		} else {
			print " [test mode]";
		}
		print "\n";
	}
}