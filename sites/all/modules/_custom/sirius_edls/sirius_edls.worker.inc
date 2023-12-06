<?php

function sirius_edls_worker_classification_options() {
  $options =& drupal_static(__FUNCTION__);

  if (!isset($options)) {
    $terms = sirius_taxonomy_load('grievance_job_classification', 'tid', 'full');
    $options = array();
    foreach ($terms as $tid => $term) {
      if ($term->field_sirius_name_short['und'][0]['value']) {
        $options[$tid] = $term->field_sirius_name_short['und'][0]['value'];
      } else {
        $options[$tid] = $term->name;
      }
    }
  }

  return $options;
}

function sirius_edls_worker_list($employer_nid, $date, $filters = array()) {
	// @todo: Both dispatched and outsourced
  $outsource_workers = sirius_edls_outsource_list($employer_nid, $date, $filters);
  $dispatch_workers = sirius_edls_dispatch_list($employer_nid, $date, $filters);
  return array_merge($outsource_workers, $dispatch_workers);
}

function sirius_edls_worker_fetch($employer_nid, $worker_id) {
  if (preg_match('/^O/', $worker_id)) {
    return sirius_edls_outsource_worker_fetch($employer_nid, $worker_id);
  }
  if (preg_match('/^I/', $worker_id)) {
    return sirius_edls_dispatch_worker_fetch($employer_nid, $worker_id);
  }
  return NULL;
}

function sirius_edls_dispatch_worker_fetch($employer_nid, $worker_id) {
  $real_id = preg_replace('/^I/', '', $real_id);
  $worker_node = sirius_worker_get_by_id($real_id);
  $result = array(
    'worker_id' => $worker_id, 
    'worker_name' => $worker_node->field_sirius_name['und'][0]['family'] . ', ' . $worker_node->field_sirius_name['und'][0]['given'],
  );
  return $result;
}

function sirius_edls_ms_list($employer_node, $type = 'name') {
  $insource_ms_list = sirius_taxonomy_load('sirius_member_status', 'tid', 'name');
  $outsource_ms_list = sirius_edls_outsource_ms_list($employer_node, $type);
  return $insource_ms_list + $outsource_ms_list;
}

function sirius_edls_dispatch_list($employer_nid, $date, $filters) {
  $employer_node = node_load($employer_nid);
  $industry_tids = sirius_fieldvals($employer_node, 'field_sirius_industry', 'tid');

  $sql_args = array();

  $sql = "select ";
  $sql .= "sirius_edls_assignments.*, ";
  $sql .= "concat('I', field_sirius_id_value) as worker_id, ";
  $sql .= "concat(field_sirius_name_family, ', ', field_sirius_name_given) as worker_name, ";
  $sql .= "ms_term.tid as worker_ms ";
  $sql .= "from node ";
  $sql .= "left join field_data_field_sirius_id on field_data_field_sirius_id.entity_type = 'node' and field_data_field_sirius_id.entity_id = node.nid ";
  $sql .= "left join field_data_field_sirius_name on field_data_field_sirius_name.entity_type = 'node' and field_data_field_sirius_name.entity_id = node.nid ";
  $sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
  $sql .= "left join field_data_field_sirius_member_status on field_data_field_sirius_member_status.entity_type = 'node' and field_data_field_sirius_member_status.entity_id = node.nid ";

  $sql .= "left join taxonomy_term_data ms_term on ms_term.tid = field_sirius_member_status_tid ";
  $sql .= "left join field_data_field_sirius_industry on field_data_field_sirius_industry.entity_type = 'taxonomy_term' and field_data_field_sirius_industry.entity_id = ms_term.tid ";

  $sql .= "left join field_data_field_sirius_worker on field_data_field_sirius_worker.entity_type = 'node' and field_data_field_sirius_worker.bundle = 'sirius_dispatch' and field_data_field_sirius_worker.field_sirius_worker_target_id = node.nid ";
  $sql .= "left join node dispatch_node on dispatch_node.nid = field_data_field_sirius_worker.entity_id ";

  $sql .= "left join field_data_field_sirius_dispatch_status on field_data_field_sirius_dispatch_status.entity_type = 'node' and field_data_field_sirius_dispatch_status.entity_id = dispatch_node.nid ";

  $sql .= "left join field_data_field_sirius_dispatch_job on field_data_field_sirius_dispatch_job.entity_type = 'node' and field_data_field_sirius_dispatch_job.entity_id = dispatch_node.nid ";
  $sql .= "left join node job_node on job_node.nid = field_sirius_dispatch_job_target_id ";

  $sql .= "left join field_data_field_grievance_shop on field_data_field_grievance_shop.entity_type = 'node' and field_data_field_grievance_shop.entity_id = job_node.nid ";

  $sql .= "left join sirius_edls_assignments on (";
  $sql .= "  sirius_edls_assignments.worker_id = concat('I', field_sirius_id_value) ";
  $sql .= "  and sirius_edls_assignments.employer_nid = :employer_nid ";
  $sql .= "  and sirius_edls_assignments.assignment_date = :date ";
  $sql .= ") ";
  $sql_args[':date'] = $date;

  $sql .= "where node.type = 'sirius_worker' ";

  $sql .= "and field_grievance_shop_target_id = :employer_nid ";
  $sql_args[':employer_nid'] = $employer_nid;

  $sql .= "and field_sirius_dispatch_status_value in ('accepted', 'accepted_secondary') ";

  $domain_nid = sirius_domain_nid();
  if ($domain_nid) {
    $sql .= "and field_sirius_domain_target_id = :domain_nid ";
    $sql_args[':domain_nid'] = $domain_nid;
  }

  if ($industry_tids) {
    $sql .= "and field_sirius_industry_tid in (:industry_tids) ";
    $sql_args[':industry_tids'] = $industry_tids;
  }

  $sql .= "group by node.nid ";

  $sql .= "order by field_sirius_name_family, field_sirius_name_given ";

  $workers = array();
  $stmt = sirius_sql_query($sql, $sql_args);
  while ($hr = $stmt->fetchAssoc()) {
    $workers[] = $hr;
  }
  return $workers;
}