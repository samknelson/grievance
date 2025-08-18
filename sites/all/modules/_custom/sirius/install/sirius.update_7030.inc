<?php 

// while true ; do time drush --user=1 php-eval "require_once('sites/all/modules/_custom/sirius/install/sirius.update_7030.inc'); sirius_update_7030_impl(2000000);" ; sleep 10; done

function sirius_update_7030_impl($offset = 0) {
	sirius_minilog_echo_active(TRUE);

	/*

	sirius_minilog("==== Creating index on start date ====");
  $sqls = array(
    'alter table field_data_field_sirius_date_start add index sirius_date_start (field_sirius_date_start_value)',
  );

  foreach ($sqls as $sql) {
    try {
      print "Executing: $sql\n";
      $stmt = db_query($sql, array());
      print "... Done.\n";
    } catch (exception $ignore) { print "Error: " . $ignore->getMessage() . "\n"; }
  }
  */

	sirius_minilog("==== Migrating payperiod fields ====");

	////////////////////////////////////////////////////
	// Update our legacy data
	////////////////////////////////////////////////////

	$sql = "select count(nid) as c from node ";
	$sql .= "left join field_data_field_sirius_datetime on field_data_field_sirius_datetime.entity_type = 'node' and field_data_field_sirius_datetime.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_date_start on field_data_field_sirius_date_start.entity_type = 'node' and field_data_field_sirius_date_start.entity_id = node.nid ";
	$sql .= "where field_sirius_datetime_value is not null ";
	$sql .= "and field_sirius_date_start_value is null ";
	$sql .= "and type in ('sirius_payperiod', 'sirius_employer_payperiod') ";
	$sql .= "order by nid ";
	$stmt = db_query($sql, array());
	$hr = $stmt->fetchAssoc();
	$grand_total = $hr['c'];

	$sql = "select nid from node ";
	$sql .= "left join field_data_field_sirius_datetime on field_data_field_sirius_datetime.entity_type = 'node' and field_data_field_sirius_datetime.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_date_start on field_data_field_sirius_date_start.entity_type = 'node' and field_data_field_sirius_date_start.entity_id = node.nid ";
	$sql .= "where field_sirius_datetime_value is not null ";
	$sql .= "and field_sirius_date_start_value is null ";
	$sql .= "and type in ('sirius_payperiod', 'sirius_employer_payperiod') ";
	$sql .= "order by nid ";
	$sql .= "limit 10000 offset $offset";
	$stmt = db_query($sql, array());

	$nids = array();
	while ($hr = $stmt->fetchAssoc()) {
		$nids[] = $hr['nid'];
	}

	$total = count($nids);
	sirius_minilog("Found $total / $grand_total records to fix.");

	// $all_records = "select * from field_data_field_sirius_datetime where entity_type = 'node' and entity_id = $nid and deleted = 0";

	$start_ts = time();

	$cache = array();
	$count = 0;
	foreach ($nids as $nid) {
		$count++;

		$now_ts = time();
		$elapsed_ts = $now_ts - $start_ts;
		$ts_per_record = $elapsed_ts / $count;
		$ts_remaining = $ts_per_record * ($grand_total - $count);

		sirius_minilog("Fixing record $count / $total / " . number_format($grand_total) . ": Payperiod $nid. Elapsed " . format_interval($elapsed_ts) . " out of " . format_interval($ts_remaining));

		$sql = "select * from field_data_field_sirius_datetime where entity_type = 'node' and entity_id = $nid and deleted = 0 ";
		$stmt = db_query($sql, array());
		$records = array();
		while ($hr = $stmt->fetchAssoc()) {
			$records[] = $hr;
		}

		foreach ($records as $record) {
			$record['field_sirius_date_start_value'] = date('Y-m-01 00:00:00', strtotime($record['field_sirius_datetime_value']) + 60*60*24*15);

			$sql = "insert into field_data_field_sirius_date_start (entity_type, bundle, deleted, entity_id, revision_id, language, delta, field_sirius_date_start_value) ";
			$sql .= "values (:entity_type, :bundle, :deleted, :entity_id, :revision_id, :language, :delta, :field_sirius_date_start_value) ";
			$sql .= "on duplicate key update field_sirius_date_start_value = :field_sirius_date_start_value ";
			$sql_args = array();
			$sql_args[':entity_type'] = $record['entity_type'];
			$sql_args[':bundle'] = $record['bundle'];
			$sql_args[':deleted'] = $record['deleted'];
			$sql_args[':entity_id'] = $record['entity_id'];
			$sql_args[':revision_id'] = $record['revision_id'];
			$sql_args[':language'] = $record['language'];
			$sql_args[':delta'] = $record['delta'];
			$sql_args[':field_sirius_date_start_value'] = $record['field_sirius_date_start_value'];
			$stmt = db_query($sql, $sql_args);
		}






		$sql = "select * from field_revision_field_sirius_datetime where entity_type = 'node' and entity_id = $nid and deleted = 0  ";
		$stmt = db_query($sql, array());
		$records = array();
		while ($hr = $stmt->fetchAssoc()) {
			$records[] = $hr;
		}

		foreach ($records as $record) {
			$record['field_sirius_date_start_value'] = date('Y-m-01 00:00:00', strtotime($record['field_sirius_datetime_value']) + 60*60*24*15);

			$sql = "insert into field_revision_field_sirius_date_start (entity_type, bundle, deleted, entity_id, revision_id, language, delta, field_sirius_date_start_value) ";
			$sql .= "values (:entity_type, :bundle, :deleted, :entity_id, :revision_id, :language, :delta, :field_sirius_date_start_value) ";
			$sql .= "on duplicate key update field_sirius_date_start_value = :field_sirius_date_start_value ";
			$sql_args = array();
			$sql_args[':entity_type'] = $record['entity_type'];
			$sql_args[':bundle'] = $record['bundle'];
			$sql_args[':deleted'] = $record['deleted'];
			$sql_args[':entity_id'] = $record['entity_id'];
			$sql_args[':revision_id'] = $record['revision_id'];
			$sql_args[':language'] = $record['language'];
			$sql_args[':delta'] = $record['delta'];
			$sql_args[':field_sirius_date_start_value'] = $record['field_sirius_date_start_value'];
			$stmt = db_query($sql, $sql_args);
		}







		$sql = "select * from field_data_field_sirius_datetime_completed where entity_type = 'node' and entity_id = $nid and deleted = 0  ";
		$stmt = db_query($sql, array());
		$records = array();
		while ($hr = $stmt->fetchAssoc()) {
			$records[] = $hr;
		}

		foreach ($records as $record) {
			$record['field_sirius_date_end_value'] = date('Y-m-t 00:00:00', strtotime($record['field_sirius_datetime_completed_value']) - 60*60*24*15);

			$sql = "insert into field_data_field_sirius_date_end (entity_type, bundle, deleted, entity_id, revision_id, language, delta, field_sirius_date_end_value) ";
			$sql .= "values (:entity_type, :bundle, :deleted, :entity_id, :revision_id, :language, :delta, :field_sirius_date_end_value) ";
			$sql .= "on duplicate key update field_sirius_date_end_value = :field_sirius_date_end_value ";
			$sql_args = array();
			$sql_args[':entity_type'] = $record['entity_type'];
			$sql_args[':bundle'] = $record['bundle'];
			$sql_args[':deleted'] = $record['deleted'];
			$sql_args[':entity_id'] = $record['entity_id'];
			$sql_args[':revision_id'] = $record['revision_id'];
			$sql_args[':language'] = $record['language'];
			$sql_args[':delta'] = $record['delta'];
			$sql_args[':field_sirius_date_end_value'] = $record['field_sirius_date_end_value'];
			$stmt = db_query($sql, $sql_args);
		}

		$sql = "select * from field_revision_field_sirius_datetime_completed where entity_type = 'node' and entity_id = $nid and deleted = 0  ";
		$stmt = db_query($sql, array());
		$records = array();
		while ($hr = $stmt->fetchAssoc()) {
			$records[] = $hr;
		}

		foreach ($records as $record) {
			$record['field_sirius_date_end_value'] = date('Y-m-t 00:00:00', strtotime($record['field_sirius_datetime_completed_value']) - 60*60*24*15);

			$sql = "insert into field_revision_field_sirius_date_end (entity_type, bundle, deleted, entity_id, revision_id, language, delta, field_sirius_date_end_value) ";
			$sql .= "values (:entity_type, :bundle, :deleted, :entity_id, :revision_id, :language, :delta, :field_sirius_date_end_value) ";
			$sql .= "on duplicate key update field_sirius_date_end_value = :field_sirius_date_end_value ";
			$sql_args = array();
			$sql_args[':entity_type'] = $record['entity_type'];
			$sql_args[':bundle'] = $record['bundle'];
			$sql_args[':deleted'] = $record['deleted'];
			$sql_args[':entity_id'] = $record['entity_id'];
			$sql_args[':revision_id'] = $record['revision_id'];
			$sql_args[':language'] = $record['language'];
			$sql_args[':delta'] = $record['delta'];
			$sql_args[':field_sirius_date_end_value'] = $record['field_sirius_date_end_value'];
			$stmt = db_query($sql, $sql_args);
		}







	}
}

