<?php

function sirius_worker_report_skill_log_form($form, &$form_state, $report_type) {
  report_cache_form_prepare($report_type, $form, $form_state);

  $skill_options = sirius_taxonomy_load('sirius_skill', 'tid', 'name');
  $form['skill_tid'] = array(
    '#type' => 'select',
    '#options' => array(NULL => t('-- Select --')) + $skill_options,
    '#title' => t('Skill'),
  );

  $form['start_date'] = array(
    '#type' => 'date_popup',
    '#title' => t('Start Date'),
    '#attributes' => array('autocomplete' => 'off'),
    '#date_format' => 'Y-m-d',
  );

  $form['end_date'] = array(
    '#type' => 'date_popup',
    '#title' => t('End Date'),
    '#attributes' => array('autocomplete' => 'off'),
    '#date_format' => 'Y-m-d',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Generate',
  );
  
  return $form;
}

function sirius_worker_report_skill_log_form_submit($form, &$form_state) { return report_cache_form_submit($form, $form_state); }

function sirius_worker_report_skill_log_fields() {
  $fields = array();
  $fields['worker_nid'] = 'Worker NID';
  $fields['worker_title'] = 'Worker Name';
  $fields['log_nid'] = 'Log NID';
  $fields["skill_tid"] = "Skill TID";
  $fields["skill_name"] = "Skill Name";
  $fields["created_ts"] = "Created TS";
  $fields["created_date"] = "Created Date";

  return $fields;
}

function sirius_worker_report_skill_log_pks(&$form_state) {
  $domain_nid = sirius_domain_nid();
  $sql_args = array();

  // worker:skill
  // "add" or "remove"
  $skill_tid = $form_state['values']['skill_tid'];
  if ($skill_tid) {
    $skill_proxy_node = sirius_taxonomy_proxy($skill_tid);
    $skill_proxy_nid = $skill_proxy_node->nid;
  }

  if ($form_state['values']['start_date']) {
    $start_ts = strtotime($form_state['values']['start_date'] . ' GMT');
  }
  if ($form_state['values']['end_date']) {
    $end_ts = strtotime($form_state['values']['end_date'] . ' GMT') + 24*60*60;
  }

  $sql_args = array();
  $sql = "select distinct nid from node ";
  $sql .= "left join field_data_field_sirius_category on field_data_field_sirius_category.entity_type = 'node' and field_data_field_sirius_category.entity_id = node.nid ";
  $sql .= "left join field_data_field_sirius_type on field_data_field_sirius_type.entity_type = 'node' and field_data_field_sirius_type.entity_id = node.nid ";
  if ($skill_proxy_nid) {
    $sql .= "left join field_data_field_sirius_log_handler on field_data_field_sirius_log_handler.entity_type = 'node' and field_data_field_sirius_log_handler.entity_id = node.nid ";
  }
  $sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
  $sql .= "where node.type = 'sirius_log' ";
  $sql .= "and field_sirius_category_value = 'worker:skill' ";
  $sql .= "and field_sirius_type_value = 'add' ";
  if ($skill_proxy_nid) {
    $sql .= "and field_sirius_log_handler_target_id = :skill_proxy_nid ";
    $sql_args[':skill_proxy_nid'] = $skill_proxy_nid;
  }
  if ($start_ts) {
    $sql .= "and created >= :start_ts ";
    $sql_args[':start_ts'] = $start_ts;
  }
  if ($end_ts) {
    $sql .= "and created <= :end_ts ";
    $sql_args[':end_ts'] = $end_ts;
  }
  if ($domain_nid) {
    $sql .= "and field_sirius_domain_target_id = :domain_nid ";
    $sql_args[':domain_nid'] = $domain_nid;
  }
  $sql .= "order by nid ";
  $stmt = sirius_sql_query($sql, $sql_args);

  $log_nids = array();
  while ($hr = $stmt->fetchAssoc()) {
    $log_nids[] = $hr['nid'];
  }

  return array_chunk($log_nids, 100, TRUE);
}

function sirius_worker_report_skill_log_batch($log_nids, $values, $report_id, $report_action, &$context) {
  $rows = array('is_multiple' => TRUE, 'rows' => array());
  $skill_lookup = sirius_taxonomy_load('sirius_skill', 'tid', 'name');

  $log_nodes = node_load_multiple($log_nids);

  foreach ($log_nodes as $log_nid => $log_node) {
    $row = array();

    $worker_node = sirius_log_handler($log_node, 'sirius_worker');
    $skill_proxy_nodes = sirius_log_handlers($log_node, 'sirius_term_proxy');
    foreach ($skill_proxy_nodes as $skill_proxy_nid => $skill_proxy_node) {
      $skill_tid = $skill_proxy_node->field_sirius_term_proxy['und'][0]['target_id'];
      if (!$skill_lookup[$skill_tid]) { continue; }
      $row['worker_nid'] = $worker_node->nid;
      $row['worker_title'] = $worker_node->title;
      $row['log_nid'] = $log_nid;
      $row["skill_tid"] = $skill_tid;
      $row["skill_name"] = $skill_lookup[$skill_tid];
      $row["created_ts"] = $log_node->created;
      $row["created_date"] = date('Y-m-d H:i:s', $log_node->created);
      $rows['rows']["$log_nid|$skill_tid"] = $row;
    }
  }

  return $rows;
}

function sirius_worker_report_skill_log_view_summary($report_type, $report_id) {
  // Load the parameters
  $query = db_select('report_cache');
  $query->fields('report_cache');
  $query->condition('report_id', $report_id);
  $query->condition('report_type', $report_type . ' :: PARAMETERS');
  $stmt = $query->execute();
  $hr = $stmt->fetchAssoc();
  $parameters = unserialize($hr['report_data']);

  $html = '';
  $html .= '<h2>';
  $html .= "Summary Report: ";
  if ($parameters['start_date']) { $html .= $parameters['start_date']; } else { $html .= '[no start date]'; }
  $html .= " to ";
  if ($parameters['end_date']) { $html .= $parameters['end_date']; } else { $html .= '[no end date]'; }
  $html .= '</h2>';

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

  $fields = sirius_worker_report_skill_log_fields();
  $fields['tools'] = t('Tools');

  $skills_worker = $skills_log = $skills_worker_total = $skills_log_total = array();
  foreach ($data as $identifier => $subdata) {
    $skill_name = $subdata['skill_name'];
    $worker_nid = $subdata['worker_nid'];
    $month = date('Y/m', $subdata['created_ts']);

    $skills_worker[$month][$skill_name][$worker_nid] = TRUE;
    $skills_log[$month][$skill_name]++;

    $skills_worker_total[$skill_name][$worker_nid] = TRUE;
    $skills_log_total[$skill_name]++;
  }

  $months = array_keys($skills_worker);
  sort($months);

  $rows = array();
  foreach ($months as $month) {
    foreach (array_keys($skills_worker[$month]) as $skill_name) {
      $row = array();
      $row[] = $month;
      $row[] = $skill_name;
      $row[] = count($skills_worker[$month][$skill_name]);
      $row[] = $skills_log[$month][$skill_name];
      $rows[] = $row;
    }
  }

  foreach (array_keys($skills_worker_total) as $skill_name) {
    $row = array();
    $row[] = '<strong>Total</strong>';
    $row[] = $skill_name;
    $row[] = count($skills_worker_total[$skill_name]);
    $row[] = $skills_log_total[$skill_name];
    $rows[] = $row;
  }


  $header = array(
    'Month',
    'Skill',
    'Workers',
    'Log Entries',
  );

  $attributes = array();
  $html .= theme_table(array('rows' => $rows, 'header' => $header, 'attributes' => $attributes));
  return $html;
}


function sirius_worker_report_skill_log_view_detail($report_type, $report_id) {
  // Load the parameters
  $query = db_select('report_cache');
  $query->fields('report_cache');
  $query->condition('report_id', $report_id);
  $query->condition('report_type', $report_type . ' :: PARAMETERS');
  $stmt = $query->execute();
  $hr = $stmt->fetchAssoc();
  $parameters = unserialize($hr['report_data']);

  $html = '';
  $html = '';
  $html .= '<h2>';
  $html .= "Detail Report: ";
  if ($parameters['start_date']) { $html .= $parameters['start_date']; } else { $html .= '[no start date]'; }
  $html .= " to ";
  if ($parameters['end_date']) { $html .= $parameters['end_date']; } else { $html .= '[no end date]'; }
  $html .= '</h2>';

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

  $fields = sirius_worker_report_skill_log_fields();
  $fields['tools'] = t('Tools');

  $rows = array();
  foreach ($data as $identifier => $subdata) {
    $subdata['tools'] = l('Log Node', 'node/' . $subdata['log_nid'], array('attributes' => array('target' => '_blank', 'class' => array('tool-button'))));
    $rows[] = $subdata;
  }

  $header = array_values($fields);

  $attributes = array();
  $html .= theme_table(array('rows' => $rows, 'header' => $header, 'attributes' => $attributes));
  return $html;
}
