<?php

class Sirius_Worker_Report_Notification_Media extends Report_Cache_Report_Generator {
  public $type = 'sirius-worker-ssn-missing';

  public function info($result = array()) {
    $result = array(
      'name' => t('Notification Media'),
      'access' => 'sirius worker view',
      'module' => 'sirius_worker',
      'file' => 'sirius_worker.report_notification_media.inc',
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
      'worker_id' => 'Worker ID',
      'employer_nid' => 'Employer NID',
      'employer_title' => 'Employer Name',
      'medium_0' => 'Medium 1 (Primary Dispatch)',
      'medium_1' => 'Medium 2 (Secondary Dispatch)',
      'medium_2' => 'Medium 3 (All Other Notifications)',
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

    $sql_args = array();

    $sql = "select distinct nid from node ";
    $sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
    $sql .= "where node.type = 'sirius_worker' ";
    if ($domain_nid) { 
      $sql .= "and field_sirius_domain_target_id = :domain_nid ";
      $sql_args[':domain_nid'] = $domain_nid;
    } else {
      $sql .= "and field_sirius_domain_target_id is NULL ";
    }
    $sql .= "order by nid ";

    $stmt = sirius_sql_query($sql, $sql_args);
    $worker_nids = array();
    while ($hr = $stmt->fetchAssoc()) {
      $worker_nids[] = $hr['nid'];
    }
    return array_chunk($worker_nids, 100, TRUE);
  }

  public function batch($worker_nids, $values, &$context) {
    $rows = array('is_multiple' => TRUE, 'rows' => array());
    $worker_nids_with_multiple = array();
    foreach ($worker_nids as $worker_nid) {
      $worker_node = node_load($worker_nid);
      $row = array();
      $row['worker_nid'] = $worker_nid;
      $row['worker_title'] = $worker_node->title;
      $row['worker_id'] = $worker_node->field_sirius_id['und'][0]['value'];
      $row['employer_nid'] = $worker_node->field_grievance_shop['und'][0]['target_id'];
      $row['employer_title'] = sirius_node_title($row['employer_nid']);

      $row['medium_0'] = $worker_node->field_sirius_dispatch_medium['und'][0]['value'];
      $row['medium_1'] = $worker_node->field_sirius_dispatch_medium['und'][1]['value'];
      $row['medium_2'] = $worker_node->field_sirius_dispatch_medium['und'][2]['value'];

      $rows['rows'][$worker_nid] = $row;

      $context['results']['dashboard']['count']++;
    }
    return $rows;
  }
}
