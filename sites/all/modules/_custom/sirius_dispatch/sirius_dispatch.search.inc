<?php

function sirius_dispatch_search($params) {
	$domain_nid = sirius_domain_nid();

	$sql_args = array();
  if ($params['count_only']) {
    if ($params['return_workers']) {
      $sql .= "select count(distinct worker_node.nid) as c ";
    } else {
      $sql .= "select count(distinct node.nid) as c ";
    }
  } else {
    $sql .= "select node.nid as dispatch_nid, worker_node.nid as worker_nid ";
  }
  $sql .= "from node ";
	$sql .= "left join field_data_field_sirius_date_start on field_data_field_sirius_date_start.entity_type = 'node' and field_data_field_sirius_date_start.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_date_end on field_data_field_sirius_date_end.entity_type = 'node' and field_data_field_sirius_date_end.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_dispatch_type on field_data_field_sirius_dispatch_type.entity_type = 'node' and field_data_field_sirius_dispatch_type.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_dispatch_job on field_data_field_sirius_dispatch_job.entity_type = 'node' and field_data_field_sirius_dispatch_job.entity_id = node.nid ";
	$sql .= "left join field_data_field_sirius_dispatch_status on field_data_field_sirius_dispatch_status.entity_type = 'node' and field_data_field_sirius_dispatch_status.entity_id = node.nid ";

	$sql .= "left join node job_node on job_node.nid = field_sirius_dispatch_job_target_id ";
	$sql .= "left join field_data_field_grievance_shop on field_data_field_grievance_shop.entity_type = 'node' and field_data_field_grievance_shop.entity_id = job_node.nid ";
	$sql .= "left join field_data_field_sirius_dispatch_job_group on field_data_field_sirius_dispatch_job_group.entity_type = 'node' and field_data_field_sirius_dispatch_job_group.entity_id = job_node.nid ";

	$sql .= "left join node employer_node on employer_node.nid = field_grievance_shop_target_id ";

  $sql .= "left join field_data_field_sirius_worker on field_data_field_sirius_worker.entity_type = 'node' and field_data_field_sirius_worker.entity_id = node.nid ";
  $sql .= "left join node worker_node on worker_node.nid = field_sirius_worker_target_id ";
  $sql .= "left join field_data_field_sirius_name on field_data_field_sirius_name.entity_type = 'node' and field_data_field_sirius_name.entity_id = worker_node.nid ";
  if ($params['worker_ws']) {
    $sql .= "left join field_data_field_sirius_work_status on field_data_field_sirius_work_status.entity_type = 'node' and field_data_field_sirius_work_status.entity_id = worker_node.nid ";
  }
	$sql .= "where node.type = 'sirius_dispatch' ";

	if ($domain_nid) {
		$sql .= "and field_sirius_domain_target_id = :domain_nid ";
		$sql_args[':domain_nid'] = $domain_nid;
	} else {
		$sql .= "and field_sirius_domain_target_id is null ";
	}
  if ($params['worker_nid']) {
  	$sql .= "and field_sirius_worker_target_id = :worker_nid ";
  	$sql_args[':worker_nid'] = $params['worker_nid'];
  }
  if ($params['employer_nid']) {
  	$sql .= "and employer_node.nid = :employer_nid ";
  	$sql_args[':employer_nid'] = $params['employer_nid'];
  }
  if ($params['job_group_nid']) {
  	$sql .= "and field_sirius_dispatch_job_group_target_id = :job_group_nid ";
  	$sql_args[':job_group_nid'] = $params['job_group_nid'];
  }
  if ($params['types']) {
  	$sql .= "and field_sirius_dispatch_type_value in (:types) ";
  	$sql_args[':types'] = $params['types'];
  }
  if ($params['status']) {
  	$sql .= "and field_sirius_dispatch_status_value = :status ";
  	$sql_args[':status'] = $params['status'];
  }
  if ($params['job_nid']) {
  	$sql .= "and field_sirius_dispatch_job_target_id = :job_nid ";
  	$sql_args[':job_nid'] = $params['job_nid'];
  }
  if ($params['statuses']) {
  	$sql .= "and field_sirius_dispatch_status_value in (:statuses) ";
  	$sql_args[':statuses'] = $params['statuses'];
  }
  if ($params['worker_name']) {
    $sql .= "and worker_node.title like :worker_name ";
    $sql_args[':worker_name'] = '%' . $params['worker_name'] . '%';
  }
  if ($params['worker_ws']) {
    $sql .= "and field_sirius_work_status_tid = :worker_ws ";
    $sql_args[':worker_ws'] = $params['worker_ws'];
  }
  if ($params['worker_ms']) {
    $sql .= "and exists ( ";
    $sql .= "  select 1 from field_data_field_sirius_member_status ";
    $sql .= "  where field_data_field_sirius_member_status.entity_type = 'node' ";
    $sql .= "  and field_data_field_sirius_member_status.entity_id = worker_node.nid ";
    $sql .= "  and field_sirius_member_status_tid = :worker_ms ";
    $sql .= ") ";
    $sql_args[':worker_ms'] = $params['worker_ms'];
  }
  if ($params['ts']) {
  	$sql .= "and field_sirius_date_start_value <= :date ";
  	$sql .= "and (field_sirius_date_end_value >= :date or field_sirius_date_end_value is null) ";
  	$sql_args[':date'] = gmdate('Y-m-d H:i:s', $params['ts']);
  }
  if ($params['return_workers'] && !$params['count_only']) {
    $sql .= "group by worker_node.nid ";
  }
  if (!$params['count_only']) {
    if ($params['order']) {
    	$order = $params['order'];
    } else {
  	  $order = "field_sirius_date_start_value desc, field_sirius_dispatch_job_target_id, node.nid desc ";
    }
    $sql .= "order by $order ";
    $limit = $params['limit'];
    if (!$limit) { $limit = 100; }
    $sql .= "limit $limit ";

    $offset = $params['offset'];
    if (!$offset) { $offset = 0; }
    $sql .= "offset $offset";
  }

  $stmt = sirius_sql_query($sql, $sql_args);

  if ($params['count_only']) {
    $hr = $stmt->fetchAssoc();
    return $hr['c'];
  }

  $nids = array();
  while ($hr = $stmt->fetchAssoc()) {
    if ($params['return_workers']) {
      $nid = $hr['worker_nid'];
    } else {
      $nid = $hr['dispatch_nid'];
    }
    if (!$nid) { continue; }
    $nids[] = $nid;
  }

  if ($params['nids_only']) { return $nids; }
	return node_load_multiple($nids);
}
