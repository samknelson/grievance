<?php

function sirius_employer_bu_list() {
	$domain_nid = sirius_domain_nid();

	$sql_args = array();
	$sql = "select nid, title from node ";
	$sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
	$sql .= "where type = 'sirius_bu' ";
	if ($domain_nid) {
		$sql .= "and field_sirius_domain_target_id = :domain_nid ";
		$sql_args[':domain_nid'] = $domain_nid;
	} else {
		$sql .= "and field_sirius_domain_target_id is null ";
	}
	$sql .= "order by title";

	$stmt = db_query($sql, $sql_args);
	$options = array();
	while ($hr = $stmt->fetchAssoc()) {
		$options[$hr['nid']] = $hr['title'];
	}

	return $options;
}