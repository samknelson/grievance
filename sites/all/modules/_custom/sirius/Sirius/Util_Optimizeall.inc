<?php

sirius_require('sirius', 'Sirius/Batchrunner.inc');

class Sirius_Util_Optimizeall extends Sirius_Batchrunner {
	public $type = 'util_optimizeall';
	public $name = 'Util - Optimize All Tables';

	public function batch_size_default(&$context = array()) { return 10; }

	/**
	 * We never load the record. The record is the pk is the table_name.
	 */

	public function record_pk($table_name) {
		return $table_name;
	}

	public function record_title($table_name) {
		return $table_name;
	}

	public function get_tables(&$context = array()) {
		$sql = "show tables";
		$stmt = db_query($sql, array());
		$tables = array();
		while ($hr = $stmt->fetchAssoc()) {
			$tables[] = reset(array_values($hr));
		}
		ksort($tables);
		return $tables;
	}

	public function get_next_batch(&$context = array()) {
		$batch_size = $this->batch_size($context);
		$last_pk = $this->last_pk();

		$all_tables = $this->get_tables($context);
		$tables = array();
		$count = 0;
		foreach ($all_tables as $table) {
			if (strcmp($table, $last_pk) <= 0) { continue; }
			++$count;
			if ($count > $batch_size) { break; }
			$tables[] = $table;
		}

		return $tables;
	}

	public function grand_total_pending(&$context = array()) {
		$c = array('batch_size' => 1000000);
		return count($this->get_next_batch($c));
	}

	public function record_load($table_name, &$context = array()) {
		return $table_name;
	}

	public function process_record_impl($table, &$context = array()) {
		static $total_reduction;

		$mode = $this->mode($context);
		if ($mode != 'live') {
			sirius_minilog("(Skipping optimization because mode = $mode)");
			return;
		}

		sirius_minilog("Optimizing: $table_name");
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
		$reduction = ($total_length - $new_total_length);
		$total_reduction += $reduction;
		sirius_minilog("... After optimize: $new_total_length MB ($new_data_length data and $new_index_length index)");
		sirius_minilog("... Size reduction: $reduction (Total $total_reduction)");
	}
}
