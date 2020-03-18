<?php

function sirius_dispatch_report_prior_employed_list_form($form, &$form_state, $report_type) {
  report_cache_form_prepare($report_type, $form, $form_state);

  $employers = sirius_employer_list();

  $form['employer'] = array(
  	'#title' => t('Employer'),
  	'#type' => 'select',
  	'#options' => $employers,
    '#required' => TRUE,
  );

  $form['date'] = array(
    '#title' => t('Date'),
    '#type' => 'date_popup',
    '#required' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Generate',
  );
  
  return $form;
}

function sirius_dispatch_report_prior_employed_list_form_submit($form, &$form_state) { return report_cache_form_submit($form, $form_state); }

function sirius_dispatch_report_prior_employed_list_fields() {
	$fields = array();
  $fields['worker_nid'] = 'Worker: NID';
  $fields['worker_id'] = 'Worker: ID';
  $fields['worker_name_full'] = 'Worker: Full Name';
  $fields['worker_name_first'] = 'Worker: First Name';
  $fields['worker_name_last'] = 'Worker: Last Name';
  $fields['worker_phone'] = 'Worker: Phone';
  $fields['worker_email'] = 'Worker: Email';
  $fields['dispatch_nid'] = 'Dispatch: NID';

	return $fields;
}

function sirius_dispatch_report_prior_employed_list_pks(&$form_state) {
  $sql_args = array();
  $sql_args[':shop_nid'] = $form_state['values']['employer'];
  $sql_args[':ts'] = strtotime($form_state['values']['date']);
  $sql = "select nid from node ";
  $sql .= "join field_data_field_sirius_dispatch_job on field_data_field_sirius_dispatch_job.entity_type = 'node' and field_data_field_sirius_dispatch_job.entity_id = node.nid ";
  $sql .= "join field_data_field_grievance_shop on field_data_field_grievance_shop.entity_type = 'node' and field_data_field_grievance_shop.entity_id = field_sirius_dispatch_job_target_id ";
  $sql .= "where node.type = 'sirius_dispatch' ";
  $sql .= "and field_grievance_shop_target_id = :shop_nid ";
  $sql .= "and created <= :ts ";
  $sql .= "order by nid desc ";
  // $sql .= "limit 300 ";
  $stmt = db_query($sql, $sql_args);

  $nids = array();
  while ($hr = $stmt->fetchAssoc()) {
    $nids[] = $hr['nid'];
  }
  return array_chunk($nids, 100, TRUE);
}

function sirius_dispatch_report_prior_employed_list_batch($dispatch_nids, $values, $report_id, $report_action, &$context) {

  $ts = strtotime($values['date']);

  // Get the worker info for each dispatch. 
  $sql_args = array();
  $sql = "select node.nid as worker_nid, ";
  $sql .= "node.title, "; 
  $sql .= "field_data_field_sirius_worker.entity_id as dispatch_nid, ";
  $sql .= "field_sirius_name_family as worker_name_last, ";
  $sql .= "field_sirius_name_given as worker_name_first, ";
  $sql .= "node.title as worker_name_full, ";
  $sql .= "field_sirius_phone_value as worker_phone, ";
  $sql .= "field_sirius_email_value as worker_email, ";
  $sql .= "field_sirius_id_value as worker_id ";
  $sql .= "from node ";
  $sql .= "join field_data_field_sirius_worker on field_data_field_sirius_worker.entity_type = 'node' and field_sirius_worker_target_id = node.nid ";
  $sql .= "left join field_data_field_sirius_name on field_data_field_sirius_name.entity_type = 'node' and field_data_field_sirius_name.entity_id = node.nid ";
  $sql .= "left join field_data_field_sirius_phone on field_data_field_sirius_phone.entity_type = 'node' and field_data_field_sirius_phone.entity_id = node.nid ";
  $sql .= "left join field_data_field_sirius_email on field_data_field_sirius_email.entity_type = 'node' and field_data_field_sirius_email.entity_id = node.nid ";
  $sql .= "left join field_data_field_sirius_id on field_data_field_sirius_id.entity_type = 'node' and field_data_field_sirius_id.entity_id = node.nid ";
  $sql .= "where node.type = 'sirius_worker' ";
  $sql .= "and field_data_field_sirius_worker.entity_id in (:dispatch_nids) ";
  $sql_args[':dispatch_nids'] = array_values($dispatch_nids);
  $stmt = db_query($sql, $sql_args);
  $workers = array();
  while ($hr = $stmt->fetchAssoc()) {
    $workers[$hr['dispatch_nid']] = $hr;
  }

  // Get the current status for each dispatch
  $sql_args = array();
  $sql = "select field_data_field_sirius_dispatch_status.entity_id as nid, ";
  $sql .= "field_sirius_dispatch_status_value ";
  $sql .= "from field_data_field_sirius_dispatch_status ";
  $sql .= "where entity_type = 'node' ";
  $sql .= "and entity_id in (:dispatch_nids) ";
  $sql_args[':dispatch_nids'] = array_values($dispatch_nids);
  $stmt = db_query($sql, $sql_args);
  $status_now = array();
  while ($hr = $stmt->fetchAssoc()) {
    $status_raw = $hr['field_sirius_dispatch_status_value'];
    if ($status_raw == 'accepted' || $status_raw == 'accepted_secondary') { $status_now[$hr['nid']] = 'employed'; } else { $status_now[$hr['nid']] = 'layoff'; }
  }

  $rows = array('is_multiple' => TRUE, 'rows' => array());

  foreach ($dispatch_nids as $dispatch_nid) {
    // Walk the log to find our status back then
    $sql_args = array();
    $sql_args[':dispatch_nid'] = $dispatch_nid;
    $sql_args[':ts'] = $ts;
    $sql = "select nid, created, ";
    $sql .= "field_sirius_category_value, ";
    $sql .= "field_sirius_type_value ";
    $sql .= "from node ";
    $sql .= "join field_data_field_sirius_log_handler on field_data_field_sirius_log_handler.entity_type = 'node' and field_data_field_sirius_log_handler.entity_id = node.nid ";
    $sql .= "join field_data_field_sirius_category on field_data_field_sirius_category.entity_type = 'node' and field_data_field_sirius_category.entity_id = node.nid ";
    $sql .= "join field_data_field_sirius_type on field_data_field_sirius_type.entity_type = 'node' and field_data_field_sirius_type.entity_id = node.nid ";
    $sql .= "where node.type = 'sirius_log' ";
    $sql .= "and field_sirius_log_handler_target_id = :dispatch_nid ";
    $sql .= "and (";
    $sql .= "  (field_sirius_category_value like 'dispatch:%' and field_sirius_type_value = 'complete') or ";
    $sql .= "  (field_sirius_category_value = 'dispatch:vlri') ";
    $sql .= ") ";;
    $sql .= "and created <= :ts ";
    $sql .= "order by created ";
    $stmt = db_query($sql, $sql_args);
    $status_then = 'unknown';
    while ($hr = $stmt->fetchAssoc()) {
      $category = $hr['field_sirius_category_value'];
      if ($category == 'dispatch:accept') { $status_then = 'employed'; }
      if ($category == 'dispatch:resign') { $status_then = 'layoff'; }
      if ($category == 'dispatch:terminate') { $status_then = 'layoff'; }
      if ($category == 'dispatch:vlri') { $status_then = 'layoff'; }
      if ($category == 'dispatch:rollover') { $status_then = 'layoff'; }
    }
    if ($status_then == 'unknown') { $status_then = $status_now[$dispatch_nid]; }
    if (!$status_then) { $status_then = 'layoff'; }
    if ($status_then != 'employed') { continue; }

    // Ok, this worker was employed on that date. 
    $worker = $workers[$dispatch_nid];
    $worker_nid = $worker['worker_nid'];
    if ($context['results']['dedupe'][$worker_nid]) { continue; }
    $context['results']['dedupe'][$worker_nid] = $dispatch_nid;
    $rows['rows'][$worker_nid] = $worker;
  }

  return $rows;
}

function sirius_dispatch_report_prior_employed_list_view($report_type, $report_id) {
  // Load the parameters
  $query = db_select('report_cache');
  $query->fields('report_cache');
  $query->condition('report_id', $report_id);
  $query->condition('report_type', $report_type . ' :: PARAMETERS');
  $stmt = $query->execute();
  $hr = $stmt->fetchAssoc();
  $parameters = unserialize($hr['report_data']);
  $ratio_1_tid = $parameters['ratio_1'];
  $ratio_2_tid = $parameters['ratio_2'];
  $ratio_1_term = taxonomy_term_load($ratio_1_tid);
  $ratio_2_term = taxonomy_term_load($ratio_2_tid);

  $html = '';
  $html .= '<h2>';
  $html .= 'Ratio of employed workers with the member status <strong>' . $ratio_1_term->name . '</strong> to <strong>' . $ratio_2_term->name . '</strong>';
  $html .= '</h2>';

  $query = db_select('report_cache');
  $query->fields('report_cache');
  $query->condition('report_id', $report_id);
  $query->condition('report_type', $report_type);
  $stmt = $query->execute();
  $data = array();
  $grand_total_1 = 0;
  $grand_total_2 = 0;
  while ($hr = $stmt->fetchAssoc()) {
    $hr = unserialize($hr['report_data']);

    $count_1 = $hr['status_' . $ratio_1_tid];
    $count_2 = $hr['status_' . $ratio_2_tid];

    $grand_total_1 += $count_1;
    $grand_total_2 += $count_2;

    $total = $count_1 + $count_2; 
    if (!$total) { continue; }

    $key = $hr['shop_name'];

    $row = array();
    $row[] = $key;
    $row[] = $total;
    $row[] = $count_1;
    $row[] = (100 *number_format($count_1 / $total, 2)) . ' %';
    $row[] = $count_2;
    $row[] = (100 *number_format($count_2 / $total, 2)) . ' %';



    $data[$key] = $row;
  }

  ksort($data);
  $rows = array_values($data);

  $grand_total = $grand_total_1 + $grand_total_2;
  if ($grand_total) {
  	$row = array();
  	$row[] = '<strong>' . 'Total' . '</strong>';
    $row[] = '<strong>' . $grand_total . '</strong>';
    $row[] = '<strong>' . $grand_total_1 . '</strong>';
    $row[] = '<strong>' . (100 *number_format($grand_total_1 / $grand_total, 2)) . ' %' . '</strong>';
    $row[] = '<strong>' . $grand_total_2;
    $row[] = '<strong>' . (100 *number_format($grand_total_2 / $grand_total, 2)) . ' %' . '</strong>';
    $rows[] = $row;
  }

  $header = array(
  	'Shop Name',
  	'Total Workers',
  	'Count: ' . $ratio_1_term->name,
  	'Fraction: ' . $ratio_1_term->name,
  	'Count: ' . $ratio_2_term->name,
  	'Fraction: ' . $ratio_2_term->name,
  );

  $attributes = array();
  $html .= theme_table(array('rows' => $rows, 'header' => $header, 'attributes' => $attributes));

  return $html;
}