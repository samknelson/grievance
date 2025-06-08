<?php

function sirius_smf_wym_fetch($worker_nid, $year, $month, $create = FALSE) {
	if (strlen($month) < 2) { $month = "0$month"; }

	// Time zones are a pain in the butt. I want a 
	$date = "$year-$month-01";

	$sql_args = array();
	$sql = "select nid from node ";
	$sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_worker on field_data_field_sirius_worker.entity_type = 'node' and field_data_field_sirius_worker.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_date_start on field_data_field_sirius_date_start.entity_type = 'node' and field_data_field_sirius_date_start.entity_id = node.nid ";
	$sql .= "where type = 'smf_worker_month' ";

	$sql .= "and field_sirius_domain_target_id = :domain_nid ";
	$sql_args[':domain_nid'] = sirius_domain_nid(); 

	$sql .= "and field_sirius_worker_target_id = :worker_nid ";
	$sql_args[':worker_nid'] = $worker_nid;

	$sql .= "and field_sirius_date_start_value = :date ";
	$sql_args[':date'] = $date;

	$stmt = sirius_sql_query($sql, $sql_args);
	$hr = $stmt->fetchAssoc();
	if ($hr['nid']) { return node_load($hr['nid']); }
	if (!$create) { return NULL; }

	global $user;
	$wym_node = new stdClass();
	$wym_node->type = 'smf_worker_month';
	$wym_node->language = 'und';
	$wym_node->uid = $user->uid;
	$wym_node->status = 1;
	$wym_node->promote = 0;
	$wym_node->comment = 0;
	$wym_node->title = sirius_node_title($worker_nid) . " / $year / $month";
	$wym_node->field_sirius_worker['und'][0]['target_id'] = $worker_nid;
	$wym_node->field_sirius_domain['und'][0]['target_id'] = sirius_domain_nid();
	$wym_node->field_sirius_date_start['und'][0]['value'] = $date;

	node_object_prepare($wym_node);
	$wym_node = node_submit($wym_node);
	node_save($wym_node);

	return $wym_node;
}

function sirius_smf_wym_fetch_all($worker_nid, $nids_only = FALSE) {
	$sql_args = array();
	$sql = "select nid from node ";
	$sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_worker on field_data_field_sirius_worker.entity_type = 'node' and field_data_field_sirius_worker.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_date_start on field_data_field_sirius_date_start.entity_type = 'node' and field_data_field_sirius_date_start.entity_id = node.nid ";
	$sql .= "where type = 'smf_worker_month' ";

	$sql .= "and field_sirius_domain_target_id = :domain_nid ";
	$sql_args[':domain_nid'] = sirius_domain_nid(); 

	$sql .= "and field_sirius_worker_target_id = :worker_nid ";
	$sql_args[':worker_nid'] = $worker_nid;

	$sql .= "order by field_sirius_date_start_value ";

	$stmt = sirius_sql_query($sql, $sql_args);
	while ($hr = $stmt->fetchAssoc()) {
		$nids[] = $hr['nid'];
	}
	if ($nids_only) { return $nids; }
	return node_load_multiple($nids);
}
