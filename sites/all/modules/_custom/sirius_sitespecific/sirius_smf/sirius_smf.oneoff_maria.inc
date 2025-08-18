<?php

// drush --user=1 php-eval "sirius_require('sirius_smf', 'sirius_smf.oneoff_maria.inc'); sirius_smf_oneoff_maria();"

function sirius_smf_oneoff_maria() {
	$sql = "select * from node ";
	$sql .= "left join field_data_field_sirius_log_handler on field_data_field_sirius_log_handler.entity_type = 'node' and field_data_field_sirius_log_handler.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_category on field_data_field_sirius_category.entity_type = 'node' and field_data_field_sirius_category.entity_id = node.nid ";
	$sql .= "where node.type = 'sirius_log' ";
	$sql .= "and field_sirius_log_handler_target_id = 2881009 ";
	$sql .= "and field_sirius_category_value = 'worker:work_status' ";
	$sql .= "order by node.nid ";

	$stmt = sirius_sql_query($sql, array());
	while ($hr = $stmt->fetchAssoc()) {
		$nid = $hr['nid'];
		print "Deleting: $nid\n";
		node_delete($nid);
	}
}