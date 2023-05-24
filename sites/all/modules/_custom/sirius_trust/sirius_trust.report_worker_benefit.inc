<?php

function sirius_trust_report_worker_benefit_info() {
  $result = array(
    'name' => t('Coverage - Overview'),
    'description' => '',
    'access' => 'sirius trust staff',
    'fields' => sirius_trust_report_worker_benefit_fields(),
    'form_callback' => 'sirius_trust_report_worker_benefit_form',
    'pk_list_callback' => 'sirius_trust_report_worker_benefit_pks',
    'batch_callback' => 'sirius_trust_report_worker_benefit_batch',
    'render_callbacks' => array(
      'View' => 'sirius_trust_report_worker_benefit_view',
    ),
    'module' => 'sirius_trust',
    'file' => 'sirius_trust.report_worker_benefit.inc',
    'dashboard' => TRUE,
  );

  return $result;
}

function sirius_trust_report_worker_benefit_fields() {
  return array(
    'wb_nid' => 'WB NID',
    'benefit_nid' => 'Benefit NID',
    'benefit_name' => 'Benefit Name',
    'subscriber_nid' => 'Subscriber NID',
    'subscriber_name' => 'Subscriber Name',
    'dependent_nid' => 'Dependent NID',
    'dependent_name' => 'Dependent Name',
    'relationship_nid' => 'Relationship NID',
    'relationship_type_tid' => 'Relationship Type TID',
    'relationship_type_name' => 'Relationship Type Name',
    'employer_nid' => 'Employer NID',
    'employer_name' => 'Employer Name',
  );
}

function sirius_trust_report_worker_benefit_form($form, &$form_state, $report_type) {
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

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Generate',
  );

  return $form;
}

function sirius_trust_report_worker_benefit_form_submit($form, &$form_state) { return report_cache_form_submit($form, $form_state); }

function sirius_trust_report_worker_benefit_pks(&$form_state) {
  if ($form_state['values']['asof']) {
    $asof = $form_state['values']['asof'] . ' 00:00:00';
  } else {
    $asof = date('Y-m-d 00:00:00');
  }

  $domain_nid = sirius_domain_nid();
  $sql_args = array();
  $sql_args[':asof'] = $asof;

  $sql = "select distinct nid from node ";
  $sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
  $sql .= "left join field_data_field_sirius_date_start on field_data_field_sirius_date_start.entity_type = 'node' and field_data_field_sirius_date_start.entity_id = node.nid ";
  $sql .= "left join field_data_field_sirius_date_end on field_data_field_sirius_date_end.entity_type = 'node' and field_data_field_sirius_date_end.entity_id = node.nid ";
  $sql .= "where node.type = 'sirius_trust_worker_benefit' ";
  if ($domain_nid) { 
    $sql .= "and field_sirius_domain_target_id = :domain_nid ";
    $sql_args[':domain_nid'] = $domain_nid;
  } else {
    $sql .= "and field_sirius_domain_target_id is NULL ";
  }
  $sql .= "and field_sirius_date_start_value <= :asof ";
  $sql .= "and (field_sirius_date_end_value >= :asof or field_sirius_date_end_value is null) ";
  $sql .= "order by nid ";

  $stmt = sirius_sql_query($sql, $sql_args);
  $wb_nids = array();
  while ($hr = $stmt->fetchAssoc()) {
    $wb_nids[] = $hr['nid'];
  }
  return array_chunk($wb_nids, 100, TRUE);
}

function sirius_trust_report_worker_benefit_batch($wb_nids, $values, $report_id, $report_action, &$context) {
  $rows = array('is_multiple' => TRUE, 'rows' => array());

  $benefit_names = sirius_trust_benefit_options(FALSE);
  $employer_names = sirius_employer_list(FALSE);
  $relationship_type_names = sirius_taxonomy_load('sirius_contact_relationship_types', 'tid', 'name');

  $wb_nodes = node_load_multiple($wb_nids);
  foreach ($wb_nodes as $wb_nid => $wb_node) {
    $row = array();
    $row['wb_nid'] = $wb_nid;
    $row['benefit_nid'] = $wb_node->field_sirius_trust_benefit['und'][0]['target_id'];
    $row['benefit_name'] = $benefit_names[$row['benefit_nid']];
    $row['dependent_nid'] = $wb_node->field_sirius_worker['und'][0]['target_id'];
    $row['dependent_name'] = sirius_node_title($row['dependent_nid']);
    $row['relationship_nid'] = $wb_node->field_sirius_contact_relation['und'][0]['target_id'];

    if ($row['relationship_nid']) {
      $relationship_node = node_load($row['relationship_nid']);
      $subscriber_contact_nid = $relationship_node->field_sirius_contact['und'][0]['target_id'];
      $subscriber_worker_node = sirius_worker_get_by_contact_nid($subscriber_contact_nid);

      $row['subscriber_nid'] = $subscriber_worker_node->nid;
      $row['subscriber_name'] = $subscriber_worker_node->title;

      $row['relationship_type_tid'] = $relationship_node->field_sirius_contact_reltype['und'][0]['tid'];
      $row['relationship_type_name'] = $relationship_type_names[$row['relationship_type_tid']];
    } else {
      $row['subscriber_nid'] = $row['dependent_nid'];
      $row['subscriber_name'] = $row['dependent_name'];
      $row['relationship_type_name'] = '[self]';
    }

    $row['employer_nid'] = $wb_node->field_grievance_shop['und'][0]['target_id'];
    $row['employer_name'] = $employer_names[$row['employer_nid']];
    $rows['rows'][] = $row;

    $context['results']['dashboard']['count']++;
    $context['results']['dashboard']['summary']['count_by_benefit'][$row['benefit_nid']]++;

    $context['results']['dedup'][$row['benefit_nid']]['subscriber'][$row['subscriber_nid']]++;
    $context['results']['dedup'][''][$row['subscriber_nid']] = TRUE;
  }
  return $rows;
}

function sirius_trust_report_worker_benefit_view_form($form, &$form_state) {
  $form['#method'] = 'get';

  foreach (array('report_id', 'action', 'callback-name') as $key) {
    $form[$key] = array(
      '#type' => 'hidden',
      '#value' => $_REQUEST[$key],
    );
  }

  $key_names = array(
    'benefit_name' => 'Benefit',
    'employer_name' => 'Employer',
    'relationship_type_name' => 'Relationship',
  );

  for ($i=1; $i<=3; ++$i) {
    $form["key$i"] = array(
      '#title' => "Breakout Field $i",
      '#type' => 'select',
      '#options' => array(NULL => t('-- Select --')) + $key_names,
      '#default_value' => $_GET["key$i"],
      '#prefix' => '<div class="container-inline">',
      '#suffix' => '</div>',
    );
  }

  $form["output"] = array(
    '#title' => "Output",
    '#type' => 'select',
    '#options' => array(
      NULL => t('-- Select --'),
      'screen' => t('Screen'),
      'csv' => t('CSV')
    ),
    '#default_value' => $_GET['output'],
    '#prefix' => '<div class="container-inline">',
    '#suffix' => '</div>',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  return $form;
}

function sirius_trust_report_worker_benefit_view($report_type, $report_id) {
  $info = report_cache_info($report_type);

  $key_names = array(
    'benefit_name' => 'Benefit',
    'employer_name' => 'Employer',
    'relationship_type_name' => 'Relationship',
  );

  $html .= drupal_render(drupal_get_form('sirius_trust_report_worker_benefit_view_form'));

  $keys = array();
  for ($i=1; $i<=3; ++$i) {
    if ($_REQUEST["key$i"]) {
      $keys[] = $_REQUEST["key$i"];
    }
  }
  $keys = array_unique($keys);
  if (!$keys) { 
    $html .= '<p>' . t('Select one or more breakout fields.') . '</p>';
    return $html;
  }

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
  while ($hr = $stmt->fetchAssoc()) {
    $hr = unserialize($hr['report_data']);

    $fullkey = array();
    foreach ($keys as $key) {
      $key_value = $hr[$key];
      if (!$key_value) { $key_value = '[no value]'; }
      $fullkey[] = $key_value;
      $key_values[$key][$key_value] = TRUE;
    }
    $fullkey = join('|', $fullkey);

    $data[$fullkey]['count']++;
    $data[$fullkey]['dependents'][] = $hr['dependent_nid'];
    $data[$fullkey]['subscribers'][] = $hr['subscriber_nid'];
  }

  foreach ($keys as $key) {
    $key_values[$key] = array_keys($key_values[$key]);
    sort($key_values[$key]);
  }

  $options = array();
  $options['output'] = $_GET['output'];
  $result = sirius_trust_report_worker_benefit_view_table($key_names, $keys, $key_values, $data, $options);
  $rows = $result['rows'];

  $header = array();
  foreach ($keys as $key) {
    $header[] = $key_names[$key];
  }
  $header[] = 'Coverage records';
  $header[] = 'Dependents';
  $header[] = 'Subscribers';

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

function sirius_trust_report_worker_benefit_view_table($key_names, $keys, $key_values, $data, $options, $state = array()) {
  $rows = $totals = array();
  if ($keys) {
    $key = array_shift($keys);
    foreach ($key_values[$key] as $key_value) {
      $newstate = $state;
      $newstate[$key] = $key_value;
      $options['is_first'] = $is_first;
      $result = sirius_trust_report_worker_benefit_view_table($key_names, $keys, $key_values, $data, $options, $newstate);
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