<?php

class Sirius_Worker_Report_Ssn_Duplicate extends Report_Cache_Report_Generator {
  public $type = 'sirius-worker-ssn-duplicate';

  public function info($result = array()) {
    $result = array(
      'name' => t('SSN Duplicate'),
      'access' => 'sirius worker view',
      'module' => 'sirius_worker',
      'file' => 'sirius_worker.report_ssn_duplicate.inc',
      'dashboard' => TRUE,
      'preview' => array(
        'preview_links' => TRUE,
      )
    );

    return parent::info($result);
  }

  public function fields() {
    return array(
      'worker_nid' => 'Worker NID',
      'worker_title' => 'Worker Name',
      'ssn' => 'SSN',
    );
  }

  public function preview_links($row) {
    return array(
      'node/' . $row['worker_nid'] => 'Worker',
    );
  }

  public function form() {
    return array();
  }

  public function pks(&$form_state) {
    $domain_nid = sirius_domain_nid();
    $sql_args = array();

    $sql = "select field_sirius_ssn_value, ";
    $sql .= "count(*) as c ";
    $sql .= "from node ";
    $sql .= "left join field_data_field_sirius_ssn on field_data_field_sirius_ssn.entity_type = 'node' and field_data_field_sirius_ssn.entity_id = node.nid ";
    $sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
    $sql .= "where node.type = 'sirius_worker' ";
    if ($domain_nid) { 
      $sql .= "and field_sirius_domain_target_id = :domain_nid ";
      $sql_args[':domain_nid'] = $domain_nid;
    } else {
      $sql .= "and field_sirius_domain_target_id is NULL ";
    }
    $sql .= "and field_sirius_ssn_value is not null ";
    $sql .= "and field_sirius_ssn_value != '' ";
    $sql .= "group by field_sirius_ssn_value ";
    $sql .= "having c > 1 ";
    $sql .= "order by field_sirius_ssn_value ";

    $stmt = sirius_sql_query($sql, $sql_args);
    $ssns = array();
    while ($hr = $stmt->fetchAssoc()) {
      $ssns[] = $hr['field_sirius_ssn_value'];
    }
    return array_chunk($ssns, 100, TRUE);
  }

  public function batch($ssns, $values, &$context) {
    $domain_nid = sirius_domain_nid();

    $rows = array('is_multiple' => TRUE, 'rows' => array());
    foreach ($ssns as $ssn) {
      $sql_args = array();
      $sql = "select nid, title ";
      $sql .= "from node ";
      $sql .= "left join field_data_field_sirius_ssn on field_data_field_sirius_ssn.entity_type = 'node' and field_data_field_sirius_ssn.entity_id = node.nid ";
      $sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
      $sql .= "where node.type = 'sirius_worker' ";
      if ($domain_nid) { 
        $sql .= "and field_sirius_domain_target_id = :domain_nid ";
        $sql_args[':domain_nid'] = $domain_nid;
      } else {
        $sql .= "and field_sirius_domain_target_id is NULL ";
      }
      $sql .= "and field_sirius_ssn_value = :ssn ";
      $sql_args[':ssn'] = $ssn;
      $sql .= "order by title ";
      $stmt = sirius_sql_query($sql, $sql_args);
      while ($hr = $stmt->fetchAssoc()) {
        $row = array();
        $row['worker_nid'] = $hr['nid'];
        $row['worker_title'] = $hr['title'];
        $row['ssn'] = $ssn;
        $rows['rows'][$hr['nid']] = $row;
        $context['results']['dashboard']['count']++;
      }
    }
    return $rows;
  }
}
