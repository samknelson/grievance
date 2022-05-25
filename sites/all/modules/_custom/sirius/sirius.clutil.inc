<?php

// drush --user=1 php-eval "require(drupal_get_path('module', 'sirius') . '/sirius.clutil.inc'); sirius_clutil_optimize_all();"

function sirius_clutil_optimize_all() {
	$sql = "show tables";
	$stmt = db_query($sql, array());
	$tables = array();
	while ($hr = $stmt->fetchAssoc()) {
		$tables[] = reset(array_values($hr));
	}

	$total_reduction = 0;
	foreach ($tables as $table) {
		print "Examining table: $table\n";

		$sql = "SELECT data_length, index_length FROM information_schema.TABLES where table_name = '$table'";
		$stmt = db_query($sql, array());
		$hr = $stmt->fetchAssoc();
		$data_length = round($hr['data_length'] / (1024*1024), 2);
		$index_length = round($hr['data_length'] / (1024*1024), 2);
		$total_length = $data_length + $index_length;
		print "... Before optimize: $total_length MB ($data_length data and $index_length index)\n";

		print "... Optimizing ...\n";
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
		print "... After optimize: $new_total_length MB ($new_data_length data and $new_index_length index)\n";
		print "... Size reduction: $reduction (Total $total_reduction)\n";
	}

	print "=== TOTAL REDUCTION $total_reduction ===\n";
}