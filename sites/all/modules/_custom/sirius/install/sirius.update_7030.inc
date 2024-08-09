<?php 

function sirius_update_7030_impl() {
	sirius_minilog_echo_active(TRUE);

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

	sirius_minilog("==== Migrating payperiod fields ====");

	////////////////////////////////////////////////////
	// Update our legacy data
	////////////////////////////////////////////////////

	$sql = "select nid from node ";
	$sql .= "left join field_data_field_sirius_datetime on field_data_field_sirius_datetime.entity_type = 'node' and field_data_field_sirius_datetime.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_date_start on field_data_field_sirius_date_start.entity_type = 'node' and field_data_field_sirius_date_start.entity_id = node.nid ";
	$sql .= "where field_sirius_datetime_value is not null ";
	$sql .= "and field_sirius_date_start_value is null ";
	$sql .= "and type in ('sirius_payperiod', 'sirius_employer_payperiod') ";
	$sql .= "order by nid ";
	$sql .= "limit 1000 ";
	$stmt = db_query($sql, array());

	$records_to_fix = array();
	while ($hr = $stmt->fetchAssoc()) {
		$records_to_fix[] = $hr['nid'];
	}
	print_r($records_to_fix);

	$total = count($records_to_fix);
	sirius_minilog("Found $total records to fix.");

	$cache = array();
	$count = 0;
	foreach ($records_to_fix as $nid) {
		$count++;
		sirius_minilog("Fixing record $count / $total: Payperiod $nid");
		$node = node_load($nid);
		node_save($node);
	}
}
