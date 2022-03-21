<?php

function unite_airports_list() {
	$sql = "select nid, title, field_sirius_name_short_value ";
	$sql .= "from node ";
	$sql .= "left join field_data_field_sirius_name_short on field_data_field_sirius_name_short.entity_type = 'node' and field_data_field_sirius_name_short.entity_id = node.nid ";
	$sql .= "where type = 'hdb_airport' ";
	$sql .= "order by field_sirius_name_short_value ";
	$stmt = sirius_sql_query($sql, array());
	$result = array();
	while ($hr = $stmt->fetchAssoc()) {
		$result[$hr['nid']] = $hr['field_sirius_name_short_value'];
	}
	return $result;
}

function unite_conventions_list() {
	$sql = "select nid, title ";
	$sql .= "from node ";
	$sql .= "where type = 'hdb_convention_center' ";
	$sql .= "order by title ";
	$stmt = sirius_sql_query($sql, array());
	$result = array();
	while ($hr = $stmt->fetchAssoc()) {
		$result[$hr['nid']] = $hr['title'];
	}
	return $result;
}

