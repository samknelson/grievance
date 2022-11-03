<?php

function grievance_report_log_include_exclude() {
	return drupal_get_form('grievance_report_log_include_exclude_form');
}

function grievance_report_log_include_exclude_form($form, &$form_state, $report_type) {
  report_cache_form_prepare($report_type, $form, $form_state);

  $log_types = sirius_taxonomy_load('grievance_log_types', 'tid', 'name');
  $statuses = sirius_taxonomy_load('grievance_status', 'tid', 'name');

  $form['status_tids'] = array(
    '#type' => 'checkboxes',
    '#checkall' => TRUE,
    '#title' => t('Find grievances with one of the following statuses ...'),
    '#options' => $statuses,
  );

  $form['include_tids'] = array(
    '#type' => 'checkboxes',
    '#checkall' => TRUE,
    '#title' => t('Find grievances which <strong>HAVE</strong> at least one of these log entries ...'),
    '#options' => $log_types,
  );

  $form['exclude_tids'] = array(
    '#type' => 'checkboxes',
    '#checkall' => TRUE,
    '#title' => t('... and which <strong>DO NOT HAVE</strong> at least one of these log entries.'),
    '#options' => $log_types,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Generate',
  );
  
  return $form;
}

function grievance_report_log_include_exclude_form_submit($form, &$form_state) { return report_cache_form_submit($form, $form_state); }

function grievance_report_log_include_exclude_fields() {
  return array(
    'nid' => t('NID'),
    'specialist' => t('Specialist'), 
    'shop' => t('Shop'), 
    'grievance_id' => t('Grievance ID'), 
    'first_name' => t('First Name'), 
    'last_name' => t('Last Name'), 
    'current_status' => t('Current Status'),
  );
}

function grievance_report_log_include_exclude_pks($form_state, $report_type, $report_id) {
  $status_tids = array_filter(array_values($form_state['values']['status_tids']));
  $sql_args = array();
  $sql = "select distinct nid ";
  $sql .= "from node ";
  $sql .= "left join field_data_field_grievance on field_data_field_grievance.entity_type = 'node' and field_data_field_grievance.entity_id = node.nid ";
  $sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
  $sql .= "left join field_data_field_grievance_status on field_data_field_grievance_status.entity_type = 'node' and field_data_field_grievance_status.entity_id = node.nid ";
  $sql .= "where node.type = 'grievance' ";
  if ($status_tids) {
    $sql .= "and field_grievance_status_tid in (:status_tids) ";
    $sql_args[':status_tids'] = $status_tids;
  }
  $domain_nid = sirius_domain_nid();
  if ($domain_nid) {
    $sql .= "and field_sirius_domain_target_id = :domain_nid ";
    $sql_args[':domain_nid'] = $domain_nid;
  } else {
    $sql .= "and field_sirius_domain_target_id is null ";
  }
  $sql .= "order by node.nid ";

  $stmt = db_query($sql, $sql_args);
  $nids = array();
  while ($hr = $stmt->fetchAssoc()) {
    $nids[] = $hr['nid'];
  }

  return array_chunk($nids, 100, TRUE);
}

function grievance_report_log_include_exclude_batch($grievance_nids, $values, $report_id, $report_action, &$context) {
  $include_tids = array_filter(array_values($values['include_tids']));
  $exclude_tids = array_filter(array_values($values['exclude_tids']));
  $status_lookup = sirius_taxonomy_load('grievance_status', 'tid', 'name');

  $rows = array();
  foreach ($grievance_nids as $grievance_nid) {
    // Skip if it has exclude TIDs
    $domain_nid = sirius_domain_nid();
    if ($exclude_tids) {
      $sql_args = array();
      $sql = "select count(*) as c ";
      $sql .= "from node ";
      $sql .= "left join field_data_field_grievance on field_data_field_grievance.entity_type = 'node' and field_data_field_grievance.entity_id = node.nid ";
      $sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
      $sql .= "left join field_data_field_grievance_log_type on field_data_field_grievance_log_type.entity_type = 'node' and field_data_field_grievance_log_type.entity_id = node.nid ";
      $sql .= "where node.type = 'grievance_log' ";
      $sql .= "and field_grievance_target_id = :grievance_nid ";
      $sql_args[':grievance_nid'] = $grievance_nid;
      $sql .= "and field_grievance_log_type_tid in (:exclude_tids) ";
      $sql_args[':exclude_tids'] = $exclude_tids;
      $domain_nid = sirius_domain_nid();
      if ($domain_nid) {
        $sql .= "and field_sirius_domain_target_id = :domain_nid ";
        $sql_args[':domain_nid'] = $domain_nid;
      } else {
        $sql .= "and field_sirius_domain_target_id is null ";
      }
      $sql .= "order by node.nid ";

      $stmt = db_query($sql, $sql_args);
      $hr = $stmt->fetchAssoc();
      if ($hr['c']) { continue; }
    }

    // Skip if it does not have include TIDs
    if ($include_tids) {
      $sql_args = array();
      $sql = "select count(*) as c ";
      $sql .= "from node ";
      $sql .= "left join field_data_field_grievance on field_data_field_grievance.entity_type = 'node' and field_data_field_grievance.entity_id = node.nid ";
      $sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
      $sql .= "left join field_data_field_grievance_log_type on field_data_field_grievance_log_type.entity_type = 'node' and field_data_field_grievance_log_type.entity_id = node.nid ";
      $sql .= "where node.type = 'grievance_log' ";
      $sql .= "and field_grievance_target_id = :grievance_nid ";
      $sql_args[':grievance_nid'] = $grievance_nid;
      $sql .= "and field_grievance_log_type_tid in (:include_tids) ";
      $sql_args[':include_tids'] = $include_tids;
      if ($domain_nid) {
        $sql .= "and field_sirius_domain_target_id = :domain_nid ";
        $sql_args[':domain_nid'] = $domain_nid;
      } else {
        $sql .= "and field_sirius_domain_target_id is null ";
      }
      $sql .= "order by node.nid ";

      $stmt = db_query($sql, $sql_args);
      $hr = $stmt->fetchAssoc();
      if (!$hr['c']) { continue; }
    }

    $grievance_node = node_load($grievance_nid);
    if (!$grievance_node) { continue; }
    $specialist = user_load($grievance_node->field_grievance_rep_organizer['und'][0]['target_id']);
    if ($specialist) { $specialist_name = $specialist->name; } else { $specialist_name = ''; }
    $shop_name = sirius_node_title($grievance_node->field_grievance_shop['und'][0]['target_id']);
    $row = array();
    $row['nid'] = $grievance_nid;
    $row['specialist'] = $specialist_name;
    $row['shop'] = $shop_name;
    $row['grievance_id'] = $grievance_node->field_grievance_id['und'][0]['value'];
    $row['first_name'] = $grievance_node->field_grievance_first_name['und'][0]['value'];
    $row['last_name'] = $grievance_node->field_grievance_last_name['und'][0]['value'];
    $row['current_status'] = $status_lookup[$grievance_node->field_grievance_status['und'][0]['tid']];
    $rows[] = $row;
  }

  $result = array(
    'is_multiple' => TRUE,
    'rows' => $rows,
  );

  return $result;
}


function grievance_report_log_include_exclude_view($report_type, $report_id) {
  $info = report_cache_info($report_type);

  // Load the parameters
  $query = db_select('report_cache');
  $query->fields('report_cache');
  $query->condition('report_id', $report_id);
  $query->condition('report_type', $report_type . ' :: PARAMETERS');
  $stmt = $query->execute();
  $hr = $stmt->fetchAssoc();
  $parameters = report_cache_parameters($report_type, $report_id);


  $query = db_select('report_cache');
  $query->fields('report_cache');
  $query->condition('report_id', $report_id);
  $query->condition('report_type', $report_type);
  $stmt = $query->execute();

  $fields = grievance_report_log_include_exclude_fields();
  $fields['tools'] = 'Tools';

  $rows = array();
  while ($hr = $stmt->fetchAssoc()) {
    $hr = unserialize($hr['report_data']);

    $row = array();
    foreach ($fields as $field_name => $field_value) {
      $row[$field_name] = $hr[$field_name];
    }
    $row['tools'] = l('View', 'node/' . $row['nid'], array('attributes' => array('class' => array('tool-button'))));

    $rows[] = $row;
  }

  $header = $fields;

  if ($_REQUEST['output'] == 'csv') {
    $filename = "${report_type}_";
    $r = $report_id;
    $r = preg_replace('/[^a-zA-Z0-9_ ]/', '', $r);
    $r = preg_replace('/ +/', '_', $r);
    $filename .= $r;
    $filename .= ".csv";

    drupal_add_http_header('Content-Type', 'text/csv');
    drupal_add_http_header('Content-Disposition', 'attachment;filename=' . $filename);
    $fp = fopen('php://output', 'w');
    fputcsv($fp, $header);
    foreach ($rows as $row) {
      foreach ($row as $delta => $val) { $row[$delta] = strip_tags($val); }
      fputcsv($fp, $row);
    }
    drupal_exit();
  }

  $html .= sirius_table_render($header, $rows);
  return $html;
}
