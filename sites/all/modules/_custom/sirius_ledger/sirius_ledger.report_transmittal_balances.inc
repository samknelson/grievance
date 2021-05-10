<?php

function sirius_ledger_report_transmittal_balances_info() {
  $items['sirius-ledger-transmittal-balances'] = array(
    'name' => t('Transmittal Balance Report'),
    'description' => 'Report the current transmittal balance for an account',
    'access' => 'sirius ledger admin',
    'fields' => sirius_ledger_report_transmittal_balances_fields(),
    'form_callback' => 'sirius_ledger_report_transmittal_balances_form',
    'pk_list_callback' => 'sirius_ledger_report_transmittal_balances_pks',
    'batch_callback' => 'sirius_ledger_report_transmittal_balances_batch',
    'render_callbacks' => array(
      'By Employer' => 'sirius_ledger_report_transmittal_balances_view',
    ),
    'title_callback' => 'sirius_ledger_report_transmittal_balances_title',
  );
  return $items;
}

function sirius_ledger_report_transmittal_balances_form($form, &$form_state, $report_type) {
  report_cache_form_prepare($report_type, $form, $form_state);

  $accounts = sirius_ledger_account_list();
  $form['account'] = array(
    '#type' => 'select',
    '#title' => t('Account'),
    '#options' => array(NULL => t('-- Select --')) + $accounts,
    '#required' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Generate',
  );
  
  return $form;
}

function sirius_ledger_report_transmittal_balances_form_submit($form, &$form_state) { 
  return report_cache_form_submit($form, $form_state);
}

function sirius_ledger_report_transmittal_balances_fields() {
	$fields = array();
  $fields['contact_nid'] = 'Contact NID';
  $fields['employer_nid'] = 'Employer NID';
  $fields['employer_name'] = 'Employer Name';
  $fields['balance'] = 'Balance';

	return $fields;
}

function sirius_ledger_report_transmittal_balances_pks(&$form_state) {
  $domain_nid = sirius_domain_nid();

  $sql = "select distinct nid ";
  $sql .= "from node ";
  $sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
  $sql .= "left join sirius_ledger_ar on node.nid = sirius_ledger_ar.ledger_participant ";
  $sql .= "where node.type = 'sirius_contact' ";
  $sql .= "and ledger_account = :ledger_account ";
  $sql_args[':ledger_account'] = $form_state['values']['account'];
  if ($domain_nid) {
    $sql .= "and field_sirius_domain_target_id = :domain_nid ";
    $sql_args[':domain_nid'] = $domain_nid;
  }

  $stmt = sirius_sql_query($sql, $sql_args);
  $nids = array();
  while ($hr = $stmt->fetchAssoc()) {
    $nids[] = $hr['nid'];
  }
  return array_chunk($nids, 100, TRUE);
}

function sirius_ledger_report_transmittal_balances_batch($nids, $values, $report_id, $report_action, &$context) {
  $ms_name_lookup = sirius_taxonomy_load('sirius_member_status', 'tid', 'name');
  $ws_name_lookup = sirius_taxonomy_load('sirius_work_status', 'tid', 'name');

  $rows = array('is_multiple' => TRUE, 'rows' => array());

  $search_params = array(
    'ledger_account' => $values['account'],
    'balance' => TRUE,
  );

  $employers = sirius_employer_list();

  foreach ($nids as $contact_nid) {
    $balance = sirius_ledger_balance_fetch($contact_nid, $values['account']);
    if (!$balance) { continue; }

    // Get the employer(s)
    $sql = "select node.nid, ";
    $sql .= "field_sirius_dispatch_status_value, ";
    $sql .= "job_node.title as job_title, ";
    $sql .= "worker_node.title as worker_title, ";
    $sql .= "worker_node.nid as worker_nid, ";
    $sql .= "field_grievance_shop_target_id as employer_nid ";
    $sql .= "from node ";
    $sql .= "left join field_data_field_sirius_dispatch_job on field_data_field_sirius_dispatch_job.entity_type = 'node' and field_data_field_sirius_dispatch_job.entity_id = node.nid ";
    $sql .= "left join field_data_field_sirius_worker on field_data_field_sirius_worker.entity_type = 'node' and field_data_field_sirius_worker.entity_id = node.nid ";
    $sql .= "left join field_data_field_sirius_dispatch_status on field_data_field_sirius_dispatch_status.entity_type = 'node' and field_data_field_sirius_dispatch_status.entity_id = node.nid ";
    $sql .= "left join node job_node on job_node.nid = field_sirius_dispatch_job_target_id ";
    $sql .= "left join field_data_field_grievance_shop on field_data_field_grievance_shop.entity_type = 'node' and field_data_field_grievance_shop.entity_id = job_node.nid ";
    $sql .= "left join node worker_node on worker_node.nid = field_sirius_worker_target_id ";
    $sql .= "left join field_data_field_sirius_contact on field_data_field_sirius_contact.entity_type = 'node' and field_data_field_sirius_contact.entity_id = worker_node.nid ";
    $sql .= "where node.type = 'sirius_dispatch' ";
    $sql .= "and field_sirius_contact_target_id = :contact_nid ";
    $sql_args[':contact_nid'] = $contact_nid;
    $sql .= "and field_sirius_dispatch_status_value in ('accepted', 'accepted_secondary') ";
    $sql .= "group by worker_node.nid ";
    $stmt = sirius_sql_query($sql, $sql_args);
    $employer_nid = NULL;
    while ($hr = $stmt->fetchAssoc()) {
      $employer_nid = $hr['employer_nid'];
      $row = array();
      $row['contact_nid'] = $contact_nid;
      $row['employer_nid'] = $employer_nid;
      $row['employer_name'] = $employers[$employer_nid];
      $row['balance'] = $balance;
      $rows['rows']["$contact_nid|$employer_nid"] = $row;
    }
    if (!$employer_nid) {
      $row = array();
      $row['contact_nid'] = $contact_nid;
      $row['employer_nid'] = NULL;
      $row['employer_name'] = '[no employer]';
      $row['balance'] = $balance;
      $rows['rows']["$contact_nid|NULL"] = $row;
    }
  }

  return $rows;
}

function sirius_ledger_report_transmittal_balances_title($report_type, $report_id) {
  // Load the parameters
  $account_lookup = sirius_ledger_account_list();
  $parameters = report_cache_parameters($report_type, $report_id);
  $title = 'Trasnmittal Balance Report: ' . $account_lookup[$parameters['account']];
  return $title;
}

function sirius_ledger_report_transmittal_balances_view($report_type, $report_id) {
  $html = '<h2>' . sirius_ledger_report_transmittal_balances_title($report_type, $report_id) . '</h2>';
  $parameters = report_cache_parameters($report_type, $report_id);
  $employers = sirius_employer_list();

  $query = db_select('report_cache');
  $query->fields('report_cache');
  $query->condition('report_id', $report_id);
  $query->condition('report_type', $report_type);
  $stmt = $query->execute();
  $balances = array();
  $balance_unemployed = 0;
  while ($hr = $stmt->fetchAssoc()) {
    $hr = unserialize($hr['report_data']);
    if ($hr['employer_nid']) { $balances[$hr['employer_nid']] += $hr['balance']; } else { $balance_unemployed += $hr['balance']; }
  }

  $rows = array();
  foreach ($balances as $employer_nid => $balance) {
    $name = '';
    if ($employers[$employer_nid]) { $name = $employers[$employer_nid]; } else { $name = "[unknown employer $employer_nid]"; }

    $row = array();
    $row[] = $employers[$employer_nid];
    $row[] = '$' . number_format($balance, 2);
    $rows[] = $row;
  }

  $row = array();
  $row[] = '[no employer]';
  $row[] = '$' . number_format($balance_unemployed, 2);
  $rows[] = $row;

  $header = array(
    'Employer', 
    'Balance', 
  );

  $attributes = array();
  $html .= theme_table(array('rows' => $rows, 'header' => $header, 'attributes' => $attributes));

  return $html;
}