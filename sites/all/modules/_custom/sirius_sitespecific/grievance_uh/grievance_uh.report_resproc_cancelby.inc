<?php

function grievance_uh_report_resproc_cancelby_fields() {
  return array(
    'grievance_nid' => 'Grievance NID',
    'grievance_id' => 'Grievance ID',
    'title' => 'Title',
    'cancelby_date' => 'Cancel By Date',
    'days' => 'Days Out',
  );
}

function grievance_uh_report_resproc_cancelby_form($form, &$form_state, $report_type) {
  report_cache_form_prepare($report_type, $form, $form_state);

  $form['days'] = array(
    '#type' => 'textfield',
    '#title' => t('Days Out'),
    '#default_value' => 60,
    '#size' => 4,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Generate',
  );
  
  return $form;
}

function grievance_uh_report_resproc_cancelby_form_validate($form, &$form_state) {
  $days = $form_state['values']['days'];
  if (!($days > 0) && !($days < 0)) {
    form_set_error('days', "Please enter an integer number of days.");
  }
}

function grievance_uh_report_resproc_cancelby_form_submit($form, &$form_state) { return report_cache_form_submit($form, $form_state); }

function grievance_uh_report_resproc_cancelby_pks(&$form_state) {
  // Get the list of resolution procedures
  $resproc_terms = sirius_taxonomy_load('grievance_resproc', 'tid', 'full');
  if (!$resproc_terms) { return array(); }

  // Get the list of open statuses with a resolution procedure
  $status_terms = array();
  $open_status_terms = grievance_status_terms_open();
  foreach ($open_status_terms as $status_tid => $status_term) {
    if (!$status_term->field_grievance_resproc['und'][0]['tid']) { continue; }
    $status_terms[$status_tid] = $status_term;
  }
  if (!$status_terms) { return array(); }

  $domain_nid = sirius_domain_nid();
  $sql = "select distinct nid from node ";
  $sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
  $sql .= "left join field_data_field_grievance_status on field_data_field_grievance_status.entity_type = 'node' and field_data_field_grievance_status.entity_id = node.nid ";
  $sql .= "where node.type = 'grievance' ";
  if ($domain_nid) { 
    $sql .= "and field_sirius_domain_target_id = :domain_nid ";
    $sql_args[':domain_nid'] = $domain_nid;
  } else {
    $sql .= "and field_sirius_domain_target_id is NULL ";
  }
  $sql .= "and field_grievance_status_tid in (:status_tids) ";
  $sql_args[':status_tids'] = array_keys($status_terms);
  $sql .= "order by nid ";

  $stmt = sirius_sql_query($sql, $sql_args);
  $grievance_nids = array();
  while ($hr = $stmt->fetchAssoc()) {
    $grievance_nids[] = $hr['nid'];
  }
  return array_chunk($grievance_nids, 25, TRUE);
}

function grievance_uh_report_resproc_cancelby_batch($grievance_nids, $values, $report_id, $report_action, &$context) {
  $rows = array('is_multiple' => TRUE, 'rows' => array());

  $resproc_terms = sirius_taxonomy_load('grievance_resproc', 'tid', 'full');
  $all_status_terms = sirius_taxonomy_load('grievance_status', 'tid', 'full');
  $days = $values['days'];

  if ($days > 0) {
    $ts_start = strtotime("-1 days");
    $ts_end = strtotime("+$days days");
  } else {
    $ts_start = strtotime("$days days");
    $ts_end = strtotime("+1 days");
  }

  $grievance_nodes = node_load_multiple($grievance_nids);
  foreach ($grievance_nodes as $grievance_nid => $grievance_node) {
    $grievance_json = sirius_json_get($grievance_node);

    $status_tid = $grievance_node->field_grievance_status['und'][0]['tid'];
    $resproc_tid = $all_status_terms[$status_tid]->field_grievance_resproc['und'][0]['tid'];
    $customfield_sets = sirius_customfield_set_list($resproc_terms[$resproc_tid]);
    if (!$customfield_sets) { continue; }

    $cancelby_date = NULL;
    foreach (array_keys($customfield_sets) as $set_nid) {
      $which = 'customfield_' . $set_nid;
      $path = ':details:cancel_by_date';
      $customfield_results = sirius_jsonfield_util_results_get($grievance_json, $which);
      $cancelby_date = sirius_jsonfield_util_get_from_path($customfield_results, $path);
      break;
    }
    if (!$cancelby_date) { continue; }
    $ts = strtotime($cancelby_date);

    if ($ts < $ts_start) { continue; }
    if ($ts > $ts_end) { continue; }

    $row['grievance_nid'] = $grievance_nid;
    $row['grievance_id'] = $grievance_node->field_grievance_id['und'][0]['value'];
    $row['title'] = $grievance_node->title;
    $row['cancelby_date'] = $cancelby_date;
    $row['days'] = round( (strtotime($cancelby_date) - time()) / (24*60*60) );
    $rows['rows'][] = $row;
    $context['results']['dashboard']['count']++;
  }

  return $rows;
}

function grievance_uh_report_resproc_cancelby_view($report_type, $report_id) {
  $info = report_cache_info($report_type);

  // Load the parameters
  $query = db_select('report_cache');
  $query->fields('report_cache');
  $query->condition('report_id', $report_id);
  $query->condition('report_type', $report_type . ' :: PARAMETERS');
  $stmt = $query->execute();
  $hr = $stmt->fetchAssoc();
  $parameters = report_cache_parameters($report_type, $report_id);

  $days = $parameters['days'];
  if (!$days) { $days = 60; }
  $html = sirius_fakerender_field('Days Out:', $days);

  $header = array();
  $header[] = 'Grievance ID';
  $header[] = 'Name';
  $header[] = 'Cancel By Date';
  $header[] = 'Days Out';
  $header[] = 'Tools';

  $rows = array();
  $query = db_select('report_cache');
  $query->fields('report_cache');
  $query->condition('report_id', $report_id);
  $query->condition('report_type', $report_type);
  $stmt = $query->execute();
  $data = array();
  while ($hr = $stmt->fetchAssoc()) {
    $hr = unserialize($hr['report_data']);
    $row = array();
    $row[] = $hr['grievance_id'];
    $row[] = $hr['title'];
    $row[] = $hr['cancelby_date'];
    $row[] = $hr['days'];
    $row[] = l('View', 'node/' . $hr['grievance_nid'], array('attributes' => array('class' => array('tool-button'))));
    $rows[] = $row;
  }

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

function grievance_uh_report_resproc_cancelby_view_table($key_names, $keys, $key_values, $data, $options, $state = array()) {
  $rows = $totals = array();
  if ($keys) {
    $key = array_shift($keys);
    foreach ($key_values[$key] as $key_value) {
      $newstate = $state;
      $newstate[$key] = $key_value;
      $options['is_first'] = $is_first;
      $result = grievance_uh_report_resproc_cancelby_view_table($key_names, $keys, $key_values, $data, $options, $newstate);
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