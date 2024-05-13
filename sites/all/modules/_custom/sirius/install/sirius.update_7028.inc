<?php

function sirius_update_7028_impl() {
	sirius_minilog_echo_active(TRUE);
	sirius_minilog("==== Migrating sirius gender field. ====");

	////////////////////////////////////////////////////
	// If there are no values in the legacy field, we don't have to do anything
	////////////////////////////////////////////////////

	$sql = "select count(*) as c from field_data_field_sirius_gender";
	$stmt = db_query($sql, array());
	$hr = $stmt->fetchAssoc();
	$count = $hr['c'];
	sirius_minilog("Found $count legacy gender records.");
	if ($count < 1) {
		sirius_minilog("... Since there aren't any legacy gender records, there's nothing left to do.");
		return;
	}

	////////////////////////////////////////////////////
	// Update our legacy data
	////////////////////////////////////////////////////

	$sql = "select nid, field_data_field_sirius_gender.* from node ";
	$sql .= "left join field_data_field_sirius_gender on field_data_field_sirius_gender.entity_type = 'node' and field_data_field_sirius_gender.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_gender_nota_calc on field_data_field_sirius_gender_nota_calc.entity_type = 'node' and field_data_field_sirius_gender_nota_calc.entity_id = node.nid ";
	$sql .= "where field_sirius_gender_tid is not null ";
	$sql .= "and field_sirius_gender_nota_calc_value is null ";
	$sql .= "order by nid ";
	$stmt = db_query($sql, array());

	$records_to_fix = array();
	while ($hr = $stmt->fetchAssoc()) {
		$records_to_fix[] = $hr;
	}
	if (!$records_to_fix) {
		sirius_minilog("There are no legacy gender entries that haven't been updated. Nothing left to do.");
		return;
	}
	$total = count($records_to_fix);
	sirius_minilog("Found $total legacy gender entries.");

	$cache = array();
	$count = 0;
	foreach ($records_to_fix as $record) {
		$count++;
		$nid = $record['nid'];
		$tid = $record['field_sirius_gender_tid'];
		if (!$cache[$tid]) { $cache[$tid] = sirius_term_title($tid); }
		sirius_minilog("Fixing record $count / $total (node #$nid)");

		$tables = array('field_data_field_sirius_gender_nota_calc', 'field_revision_field_sirius_gender_nota_calc');
		foreach ($tables as $table) {
			$sql = "insert ignore into $table ";
			$sql .= "(entity_type, bundle, deleted, entity_id, revision_id, language, delta, field_sirius_gender_nota_calc_value) ";
			$sql .= " values ";
			$sql .= "(:entity_type, :bundle, :deleted, :entity_id, :revision_id, :language, :delta, :field_sirius_gender_nota_calc_value) ";
			$sql_args = array(
				':entity_type' => $record['entity_type'],
				':bundle' => $record['bundle'],
				':deleted' => $record['deleted'],
				':entity_id' => $record['entity_id'],
				':revision_id' => $record['revision_id'],
				':language' => $record['language'],
				':delta' => $record['delta'],
				':field_sirius_gender_nota_calc_value' => $cache[$tid],
			);

			$stmt = db_query($sql, $sql_args);
			sirius_minilog("... Inserted into $table");
			break;
		}
	}
}
