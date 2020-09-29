<?php

function sirius_ledger_account_list() {
	$cache = &drupal_static(__FUNCTION__, array());

	$domain_nid = sirius_domain_nid();

	if (!isset($cache[$domain_nid])) {
		$sql_args = array();
		$sql = "select nid, title, field_sirius_name_short_value ";
		$sql .= "from node ";
		$sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
		$sql .= "left join field_data_field_sirius_name_short on field_data_field_sirius_name_short.entity_type = 'node' and field_data_field_sirius_name_short.entity_id = node.nid ";
		$sql .= "where node.type = 'sirius_ledger_account' ";
		if ($domain_nid) {
			$sql .= "and field_sirius_domain_target_id = :domain_nid ";
			$sql_args[':domain_nid'] = $domain_nid;
		} else {
			$sql .= "and field_sirius_domain_target_id IS NULL ";
		}
		$sql .= "order by title ";

		$cache[$domain_nid] = array();
		$stmt = db_query($sql, $sql_args);
		while ($hr = $stmt->fetchAssoc()) {
			if ($hr['field_sirius_name_short_value']) { $title = $hr['field_sirius_name_short_value']; } else { $title = $hr['title']; }
			$cache[$domain_nid][$hr['nid']] = $title;
		}
	}

	return $cache[$domain_nid];
}