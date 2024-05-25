<?php

function sirius_edls_sheet_search($params) {
	$domain_nid = sirius_domain_nid();

	$sql_args = array();
	$sql = "select node.nid from node ";
	$sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_date_start on field_data_field_sirius_date_start.entity_type = 'node' and field_data_field_sirius_date_start.entity_id = node.nid ";
	$sql .= "left join field_data_field_grievance_shop on field_data_field_grievance_shop.entity_type = 'node' and field_data_field_grievance_shop.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_job_number on field_data_field_sirius_job_number.entity_type = 'node' and field_data_field_sirius_job_number.entity_id = node.nid ";
	$sql .= "left join field_data_field_grievance_rep_assignee on field_data_field_grievance_rep_assignee.entity_type = 'node' and field_data_field_grievance_rep_assignee.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_edls_sheet_status on field_data_field_sirius_edls_sheet_status.entity_type = 'node' and field_data_field_sirius_edls_sheet_status.entity_id = node.nid ";
	$sql .= "left join field_data_field_grievance_department_tid on field_data_field_grievance_department_tid.entity_type = 'node' and field_data_field_grievance_department_tid.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_json on field_data_field_sirius_json.entity_type = 'node' and field_data_field_sirius_json.entity_id = node.nid ";
	$sql .= "where node.type = 'sirius_edls_sheet' ";

	if ($domain_nid) {
		$sql .= "and field_sirius_domain_target_id = :domain_nid ";
		$sql_args[':domain_nid'] = $domain_nid;
	}

	if ($params['grievance_shop']) {
		$sql .= "and field_grievance_shop_target_id = :grievance_shop ";
		$sql_args[':grievance_shop'] = $params['grievance_shop'];
	}

	if ($params['employer_nid']) {
		$sql .= "and field_grievance_shop_target_id = :employer_nid ";
		$sql_args[':employer_nid'] = $params['employer_nid'];
	}

	if ($params['date']) {
		$sql .= "and field_sirius_date_start_value = :date ";
		$sql_args[':date'] = date('Y-m-d', strtotime($params['date']));
	}

	if ($params['date_min']) {
		$sql .= "and field_sirius_date_start_value >= :date_min ";
		$sql_args[':date_min'] = date('Y-m-d', strtotime($params['date_min']));
	}

	if ($params['date_max']) {
		$sql .= "and field_sirius_date_start_value <= :date_max ";
		$sql_args[':date_max'] = date('Y-m-d', strtotime($params['date_max']));
	}

	if ($params['job_number']) {
		$sql .= "and field_sirius_job_number_value = :job_number ";
		$sql_args[':job_number'] = date('Y-m-d', strtotime($params['job_number']));
	}

	if ($params['facility']) {
		$sql .= "and field_sirius_dispatch_facility_target_id = :facility ";
		$sql_args[':facility'] = $params['facility'];
	}

	if ($params['job_number']) {
		$sql .= "and field_sirius_job_number_value = :job_number ";
		$sql_args[':job_number'] = $params['job_number'];
	}

	if ($params['supervisor']) {
		$sql .= "and field_grievance_rep_assignee_target_id = :supervisor ";
		$sql_args[':supervisor'] = $params['supervisor'];
	}

	if ($params['status']) {
		$sql .= "and field_sirius_edls_sheet_status_value = :status ";
		$sql_args[':status'] = $params['status'];
	}

	if ($params['department']) {
		$sql .= "and field_grievance_department_tid_tid = :department ";
		$sql_args[':department'] = $params['department'];
	}

	if ($params['reserved']) {
		$sql .= "and json_unquote(json_extract(field_sirius_json_value, '\$.edls.sheet.reserved.active')) = 'Yes' ";
	}

	if (!$params['include_trash']) {
		$sql .= "and field_sirius_edls_sheet_status_value != 'trash' ";
	}

  $sql .= "order by field_sirius_date_start_value ";
  $limit = $params['limit'];
  if (!$limit) { $limit = 100; }
  $sql .= "limit $limit";

  $stmt = sirius_sql_query($sql, $sql_args);
  $nids = array();
  while ($hr = $stmt->fetchAssoc()) {
  	$nids[] = $hr['nid'];
  }

  if (!$nids) { return array(); }
  
  if ($params['nids_only']) { return $nids; }
	return node_load_multiple($nids);
}

