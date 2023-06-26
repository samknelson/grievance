<?php

function sirius_worker_report_ssn_missing_form($form, &$form_state, $report_type) {
  report_cache_form_prepare($report_type, $form, $form_state);
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Generate',
  );
  
  return $form;
}

function sirius_worker_report_ssn_missing_form_submit($form, &$form_state) { return report_cache_form_submit($form, $form_state); }

function sirius_worker_report_ssn_missing_pks(&$form_state) {
  $domain_nid = sirius_domain_nid();
  $sql_args = array();

  $sql = "select distinct nid from node ";
  $sql .= "left join field_data_field_sirius_ssn on field_data_field_sirius_ssn.entity_type = 'node' and field_data_field_sirius_ssn.entity_id = node.nid ";
  $sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
  $sql .= "where node.type = 'sirius_worker' ";
  if ($domain_nid) { 
    $sql .= "and field_sirius_domain_target_id = :domain_nid ";
    $sql_args[':domain_nid'] = $domain_nid;
  } else {
    $sql .= "and field_sirius_domain_target_id is NULL ";
  }
  $sql .= "and field_sirius_ssn_value is null ";
  $sql .= "order by nid ";

  $stmt = sirius_sql_query($sql, $sql_args);
  $worker_nids = array();
  while ($hr = $stmt->fetchAssoc()) {
    $worker_nids[] = $hr['nid'];
  }
  return array_chunk($worker_nids, 100, TRUE);
}

function sirius_worker_report_ssn_missing_batch($worker_nids, $values, $report_id, $report_action, &$context) {
  $rows = array('is_multiple' => TRUE, 'rows' => array());
  $worker_nids_with_multiple = array();
  foreach ($worker_nids as $worker_nid) {
    $worker_node = node_load($worker_nid);
    $row = array();
    $row['worker_nid'] = $worker_nid;
    $row['worker_title'] = $worker_node->title;
    $row['worker_id'] = $worker_node->field_sirius_id['und'][0]['value'];
    $rows['rows']["$worker_nid"] = $row;

    $context['results']['dashboard']['count']++;
  }

  return $rows;
}

function sirius_worker_report_ssn_missing_view($report_type, $report_id) {
  $info = report_cache_info($report_type);

  // Load the parameters
  $query = db_select('report_cache');
  $query->fields('report_cache');
  $query->condition('report_id', $report_id);
  $query->condition('report_type', $report_type . ' :: PARAMETERS');
  $stmt = $query->execute();
  $hr = $stmt->fetchAssoc();
  $parameters = report_cache_parameters($report_type, $report_id);

  $html = '';

  $query = db_select('report_cache');
  $query->fields('report_cache');
  $query->condition('report_id', $report_id);
  $query->condition('report_type', $report_type);
  $stmt = $query->execute();
  $data = array();
  while ($hr = $stmt->fetchAssoc()) {
    $hr = unserialize($hr['report_data']);
    $data[] = $hr;
  }

  $fields = $info['fields'];
  $fields['tools'] = t('Tools');

  $rows = array();
  foreach ($data as $identifier => $subdata) {
    $subdata['tools'] = l('view', 'node/' . $subdata['worker_nid'], array('attributes' => array('target' => '_blank', 'class' => array('tool-button'))));
    $rows[] = $subdata;
  }

  $header = array_values($fields);

  $attributes = array();
  $html .= theme_table(array('rows' => $rows, 'header' => $header, 'attributes' => $attributes));
  return $html;
}
