<?php

function sirius_trust_report_worker_benefit_change_info() {
  $result = array(
    'name' => t('Coverage - Benefit Change'),
    'description' => '',
    'access' => 'sirius trust staff',
    'fields' => sirius_trust_report_worker_benefit_change_fields(),
    'parameters_form_callback' => 'sirius_trust_report_worker_benefit_change_form',
    'pk_list_callback' => 'sirius_trust_report_worker_benefit_change_pks',
    'batch_callback' => 'sirius_trust_report_worker_benefit_change_batch',
    'render_callbacks' => array(
      'View' => 'sirius_trust_report_worker_benefit_change_view',
    ),
    'module' => 'sirius_trust',
    'file' => 'sirius_trust.report_worker_benefit_change.inc',
    'dashboard' => TRUE,
  );

  return $result;
}

function sirius_trust_report_worker_benefit_change_fields() {
  return array(
    'wb_nid1' => 'From WB NID',
    'wb_nid2' => 'To WB NID',
    'dependent_nid' => 'Dependent NID',
    'dependent_name' => 'Dependent Name',
    'subscriber_nid1' => 'From Subscriber NID',
    'subscriber_nid2' => 'To Subscriber NID',
    'subscriber_name1' => 'From Subscriber Name',
    'subscriber_name2' => 'To Subscriber Name',
  );
}

function sirius_trust_report_worker_benefit_change_form($report_type) {
  $form['asof'] = array(
    '#title' => t('As Of Date'),
    '#type' => 'date_popup',
    '#attributes' => array(
      'autocomplete' => 'off',
    ),
    '#date_format' => 'Y-m-d',
    '#description' => t('Leave blank to default to today\'s date.'),
  );

  $form['days'] = array(
    '#title' => t('Days Back'),
    '#type' => 'textfield',
    '#size' => 4,
    '#default_value' => 32,
    '#description' => t('Enter the number of days back to look.')
  );

  $form['from_benefit'] = array(
    '#title' => t('From Benefit'),
    '#type' => 'select',
    '#options' => array(NULL => t('-- Select --')) + sirius_trust_benefit_options(),
    '#required' => TRUE,
  );

  $form['to_benefit'] = array(
    '#title' => t('To Benefit'),
    '#type' => 'select',
    '#options' => array(NULL => t('-- Select --')) + sirius_trust_benefit_options(),
    '#required' => TRUE,
  );

  return $form;
}

function sirius_trust_report_worker_benefit_change_pks(&$form_state) {
  $domain_nid = sirius_domain_nid();
  $sql_args = array();

  $asof_date = $form_state['values']['parameters']['asof'];
  $asof_ts = strtotime($asof_date);
  if ($asof_ts === FALSE) { $asof_ts = time(); }

  $days = $form_state['values']['parameters']['days'];
  if (!($days > 0)) { return array(); }
  $start_ts = strtotime("-$days days", $asof_ts);

  $sql_args[':date_start'] = date('Y-m-d', $start_ts);
  $sql_args[':date_end'] = $asof_date;

  $sql = "select wb_node1.nid as wb_nid1, ";
  $sql .= "wb_node2.nid as wb_nid2, ";
  $sql .= "benefit2.field_sirius_trust_benefit_target_id ";
  $sql .= "from node wb_node1 ";
  $sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = wb_node1.nid ";

  $sql .= "left join field_data_field_sirius_date_start date_start1 on date_start1.entity_type = 'node' and date_start1.entity_id = wb_node1.nid ";
  $sql .= "left join field_data_field_sirius_date_end date_end1 on date_end1.entity_type = 'node' and date_end1.entity_id = wb_node1.nid ";
  $sql .= "left join field_data_field_sirius_worker worker1 on worker1.entity_type = 'node' and worker1.entity_id = wb_node1.nid ";
  $sql .= "left join field_data_field_sirius_trust_benefit benefit1 on benefit1.entity_type = 'node' and benefit1.entity_id = wb_node1.nid ";

  $sql .= "left join field_data_field_sirius_worker worker2 on worker2.entity_type = 'node' and worker2.field_sirius_worker_target_id = worker1.field_sirius_worker_target_id ";
  $sql .= "left join node wb_node2 on wb_node2.nid  = worker2.entity_id ";
  $sql .= "left join field_data_field_sirius_date_start date_start2 on date_start2.entity_type = 'node' and date_start2.entity_id = wb_node2.nid ";
  $sql .= "left join field_data_field_sirius_date_end date_end2 on date_end2.entity_type = 'node' and date_end2.entity_id = wb_node2.nid ";
  $sql .= "left join field_data_field_sirius_trust_benefit benefit2 on benefit2.entity_type = 'node' and benefit2.entity_id = wb_node2.nid ";

  $sql .= "where wb_node1.type = 'sirius_trust_worker_benefit' ";
  $sql .= "and wb_node2.type = 'sirius_trust_worker_benefit' ";

  $sql .= "and benefit1.field_sirius_trust_benefit_target_id = :from_benefit ";
  $sql_args[':from_benefit'] = $form_state['values']['parameters']['from_benefit'];
  $sql .= "and benefit2.field_sirius_trust_benefit_target_id = :to_benefit ";
  $sql_args[':to_benefit'] = $form_state['values']['parameters']['to_benefit'];

  if ($domain_nid) { 
    $sql .= "and field_sirius_domain_target_id = :domain_nid ";
    $sql_args[':domain_nid'] = $domain_nid;
  } else {
    $sql .= "and field_sirius_domain_target_id is NULL ";
  }

  $sql .= "and date_end1.field_sirius_date_end_value >= :date_start ";
  $sql .= "and date_end1.field_sirius_date_end_value <= date_start2.field_sirius_date_start_value ";
  $sql .= "and date_start2.field_sirius_date_start_value <= :date_end ";

  $sql .= "order by wb_node1.nid ";

  $stmt = sirius_sql_query($sql, $sql_args);
  $wb_nids = array();
  while ($hr = $stmt->fetchAssoc()) {
    $wb_nids[] = $hr['wb_nid1'] . '|' . $hr['wb_nid2'];
  }
  $wb_nids = array_unique($wb_nids);
  return array_chunk($wb_nids, 100, TRUE);
}

function sirius_trust_report_worker_benefit_change_batch($keys, $values, $report_id, $report_action, &$context) {
  $rows = array('is_multiple' => TRUE, 'rows' => array());

  foreach ($keys as $key) {
    list ($wb_nid1, $wb_nid2) = explode('|', $key);

    $wb_node1 = node_load($wb_nid1);
    $wb_node2 = node_load($wb_nid2);

    $row = array();
    $row['wb_nid1'] = $wb_nid1;
    $row['wb_nid2'] = $wb_nid2;
    $row['dependent_nid'] = $wb_node1->field_sirius_worker['und'][0]['target_id'];
    $row['dependent_name'] = sirius_node_title($row['dependent_nid']);

    $relationship_nid1 = $wb_node1->field_sirius_contact_relation['und'][0]['target_id'];
    if ($relationship_nid1) {
      $relationship_node = node_load($relationship_nid1);
      $subscriber_contact_nid = $relationship_node->field_sirius_contact['und'][0]['target_id'];
      $subscriber_worker_node = sirius_worker_get_by_contact_nid($subscriber_contact_nid);
      $row['subscriber_nid1'] = $subscriber_worker_node->nid;
      $row['subscriber_name1'] = $subscriber_worker_node->title;
    } else {
      $row['subscriber_nid1'] = $row['dependent_nid'];
      $row['subscriber_name1'] = $row['dependent_name'];
      $row['relationship_type_name'] = '[self]';
    }

    $relationship_nid2 = $wb_node2->field_sirius_contact_relation['und'][0]['target_id'];
    if ($relationship_nid2) {
      $relationship_node = node_load($relationship_nid2);
      $subscriber_contact_nid = $relationship_node->field_sirius_contact['und'][0]['target_id'];
      $subscriber_worker_node = sirius_worker_get_by_contact_nid($subscriber_contact_nid);
      $row['subscriber_nid2'] = $subscriber_worker_node->nid;
      $row['subscriber_name2'] = $subscriber_worker_node->title;
    } else {
      $row['subscriber_nid2'] = $row['dependent_nid'];
      $row['subscriber_name2'] = $row['dependent_name'];
      $row['relationship_type_name'] = '[self]';
    }

    $rows['rows'][$key] = $row;

    $context['results']['dashboard']['count']++;
  }
  return $rows;
}

function sirius_trust_report_worker_benefit_change_view($report_type, $report_id) {
  $info = report_cache_info($report_type);
  $parameters = report_cache_parameters($report_type, $report_id);

  $query = db_select('report_cache');
  $query->fields('report_cache');
  $query->condition('report_id', $report_id);
  $query->condition('report_type', $report_type);
  $stmt = $query->execute();
  $data = array();
  while ($hr = $stmt->fetchAssoc()) {
    $data[] = unserialize($hr['report_data']);
  }

  foreach ($data as $hr) {
    $row = array();
    $row[] = $hr['dependent_name'];
    $row[] = $hr['subscriber_name1'];
    $row[] = $hr['subscriber_name2'];

    $tools = array();
    $tools[] = l('Dependent', 'node/' . $hr['dependent_nid'], array('attributes' => array('class' => array('tool-button'))));
    $tools[] = l('Coverage 1', 'node/' . $hr['wb_nid1'], array('attributes' => array('class' => array('tool-button'))));
    $tools[] = l('Coverage 2', 'node/' . $hr['wb_nid2'], array('attributes' => array('class' => array('tool-button'))));
    $tools[] = l('Subscriber 1', 'node/' . $hr['subscriber_nid1'], array('attributes' => array('class' => array('tool-button'))));
    $tools[] = l('Subscriber 1', 'node/' . $hr['subscriber_nid2'], array('attributes' => array('class' => array('tool-button'))));
    $row[] = join('', $tools);
    $rows[] = $row;
  }

  $header[] = 'Dependent';
  $header[] = 'Subscriber 1';
  $header[] = 'Subscriber 2';
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
