<?php

function sirius_trust_report_worker_benefit_multiple_fields() {
  return array(
    'worker_nid' => 'Worker NID',
    'worker_name' => 'Worker Name',
    'benefit_nid' => 'Benefit NID',
    'benefit_name' => 'Benefit Name',
    'benefit_type_tid' => 'Benefit Type TID',
    'benefit_type_name' => 'Benefit Type Name',
    'count' => 'Count',
  );
}

function sirius_trust_report_worker_benefit_multiple_form($form, &$form_state, $report_type) {
  report_cache_form_prepare($report_type, $form, $form_state);

  $form['asof'] = array(
    '#title' => t('As Of'),
    '#type' => 'date_popup',
    '#attributes' => array(
      'autocomplete' => 'off',
    ),
    '#required' => TRUE,
    '#date_format' => 'Y-m-d',
    '#default_value' => date('Y-m-d'),
  );

  $form['group_by'] = array(
    '#title' => t('Group By'),
    '#type' => 'select',
    '#required' => TRUE,
    '#options' => array(
      'benefit' => t('Benefit'),
      'benefit_type' => t('Benefit Type'),
    ),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Generate',
  );
  
  return $form;
}

function sirius_trust_report_worker_benefit_multiple_form_submit($form, &$form_state) { return report_cache_form_submit($form, $form_state); }

function sirius_trust_report_worker_benefit_multiple_pks(&$form_state) {
  $asof = $form_state['values']['asof'] . ' 00:00:00';
  if (!$asof) { $asof = date('Y-m-d 00:00:00'); }

  $group_by = $form_state['values']['group_by'];

  $domain_nid = sirius_domain_nid();
  $sql_args = array();
  $sql_args[':asof'] = $asof;

  $sql = "select field_sirius_worker_target_id, field_sirius_trust_benefit_target_id, field_sirius_trust_benefit_type_tid, count(*) as c from node ";
  $sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
  $sql .= "left join field_data_field_sirius_date_start on field_data_field_sirius_date_start.entity_type = 'node' and field_data_field_sirius_date_start.entity_id = node.nid ";
  $sql .= "left join field_data_field_sirius_date_end on field_data_field_sirius_date_end.entity_type = 'node' and field_data_field_sirius_date_end.entity_id = node.nid ";
  $sql .= "left join field_data_field_sirius_trust_benefit on field_data_field_sirius_trust_benefit.entity_type = 'node' and field_data_field_sirius_trust_benefit.entity_id = node.nid ";
  $sql .= "left join field_data_field_sirius_worker on field_data_field_sirius_worker.entity_type = 'node' and field_data_field_sirius_worker.entity_id = node.nid ";
  $sql .= "left join node benefit_node on field_sirius_trust_benefit_target_id = benefit_node.nid ";
  $sql .= "left join field_data_field_sirius_trust_benefit_type on field_data_field_sirius_trust_benefit_type.entity_type = 'node' and field_data_field_sirius_trust_benefit_type.entity_id = benefit_node.nid ";
  $sql .= "where node.type = 'sirius_trust_worker_benefit' ";
  if ($domain_nid) { 
    $sql .= "and field_sirius_domain_target_id = :domain_nid ";
    $sql_args[':domain_nid'] = $domain_nid;
  } else {
    $sql .= "and field_sirius_domain_target_id is NULL ";
  }
  $sql .= "and field_sirius_date_start_value <= :asof ";
  $sql .= "and (field_sirius_date_end_value >= :asof or field_sirius_date_end_value is null) ";
  if ($group_by == 'benefit_type') {
    $sql .= "group by field_sirius_worker_target_id, field_sirius_trust_benefit_type_tid ";
  } else {
    $sql .= "group by field_sirius_worker_target_id, field_sirius_trust_benefit_target_id ";
  }
  $sql .= "having c > 1 ";
  $sql .= "order by field_sirius_worker_target_id ";

  $stmt = sirius_sql_query($sql, $sql_args);
  $wbs = array();
  while ($hr = $stmt->fetchAssoc()) {
    $wbs[] = $hr['field_sirius_worker_target_id'] . '|' . $hr['field_sirius_trust_benefit_target_id'] . '|' . $hr['field_sirius_trust_benefit_type_tid'] . '|' . $hr['c'];
  }
  return array_chunk($wbs, 100, TRUE);
}

function sirius_trust_report_worker_benefit_multiple_batch($wbs, $values, $report_id, $report_action, &$context) {
  $rows = array('is_multiple' => TRUE, 'rows' => array());
  $group_by = $values['group_by'];

  $benefit_names = sirius_trust_benefit_options(FALSE);

  foreach ($wbs as $wb) {
    list($worker_nid, $benefit_nid, $benefit_type_tid, $count) = explode('|', $wb);

    $row = array();
    $row['worker_nid'] = $worker_nid;
    $row['worker_name'] = sirius_node_title($worker_nid);
    $row['benefit_nid'] = $benefit_nid;
    $row['benefit_name'] = $benefit_names[$benefit_nid];
    $row['benefit_type_tid'] = $benefit_type_tid;
    $row['benefit_type_name'] = sirius_term_title($benefit_type_tid);
    $row['count'] = $count;
    $rows['rows'][$wb] = $row;
    $context['results']['dashboard']['count']++;
  }
  return $rows;
}

function sirius_trust_report_worker_benefit_multiple_view($report_type, $report_id) {
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
  $data = array();
  $rows = array();
  while ($hr = $stmt->fetchAssoc()) {
    $hr = unserialize($hr['report_data']);
    $row = array();

    $tools = array();
    $tools[] = l('Benefits', 'node/' . $hr['worker_nid'] . '/sirius_trust_worker_benefits', array('attributes' => array('class' => array('tool-button'))));
    $row[] = $hr['worker_name'];

    if ($parameters['group_by'] == 'benefit') {
      $row[] = $hr['benefit_name'];
    } else {
      $row[] = $hr['benefit_type_name'];
    }
    $row[] = join('', $tools);
    $rows[] = $row;
  }

  $header[] = 'Worker';
  if ($parameters['group_by'] == 'benefit') {
    $header[] = 'Benefit';
  } else {
    $header[] = 'Benefit Type';
  }
  $header[] = 'Tools';

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


  $attributes = array();
  $html .= theme_table(array('rows' => $rows, 'header' => $header, 'attributes' => $attributes));
  return $html;
}

function sirius_trust_report_worker_benefit_multiple_view_table($key_names, $keys, $key_values, $data, $options, $state = array()) {
  $rows = $totals = array();
  if ($keys) {
    $key = array_shift($keys);
    foreach ($key_values[$key] as $key_value) {
      $newstate = $state;
      $newstate[$key] = $key_value;
      $options['is_first'] = $is_first;
      $result = sirius_trust_report_worker_benefit_multiple_view_table($key_names, $keys, $key_values, $data, $options, $newstate);
      $rows = array_merge($rows, $result['rows']);
    }

    $total_count = $total_count_dependents = $total_count_subscribers = 0;
    foreach ($rows as $row) {
      $total_count += $row['count'];
      $total_count_dependents += $row['count_dependents'];
      $total_count_subscribers += $row['count_subscribers'];
    }
    $total_row = array();
    foreach ($state as $state_value) {
      $total_row[] = '<strong>' . $state_value . '</strong>';
    }
    $total_row[] = '<strong>' . t('Total') . '</strong>';
    foreach ($keys as $key) { 
      $total_row[] = '';
    }
    $total_row['total_count'] = number_format($total_count, 0);
    $total_row['total_count_dependents'] = number_format($total_count_dependents, 0);
    $total_row['total_count_subscribers'] = number_format($total_count_subscribers, 0);
    $rows[] = $total_row;
  } else {
    $fullkey = join('|', $state);

    $row = $state;

    if ($data[$fullkey]) {
      $row['count'] = number_format($data[$fullkey]['count'], 0);
      $row['count_dependents'] = number_format(count(array_unique($data[$fullkey]['dependents'])));
      $row['count_subscribers'] = number_format(count(array_unique($data[$fullkey]['subscribers'])));
    } else {
      $row['count'] = 0;
      $row['count_dependents'] = 0;
      $row['count_subscribers'] = 0;
    }
    $rows[] = $row;
  }

  return array('rows' => $rows, 'totals' => $totals);
}