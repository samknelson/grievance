<?php

function sirius_ledger_report_discrepancy_info() {
  $items['sirius-ledger-discrepancy'] = array(
    'name' => t('A/R Month-to-Month Discrepancy Report'),
    'description' => 'Report accounts with differences in charges or payments between months',
    'access' => 'sirius ledger admin',
    'fields' => sirius_ledger_report_discrepancy_fields(),
    'form_callback' => 'sirius_ledger_report_discrepancy_form',
    'pk_list_callback' => 'sirius_ledger_report_discrepancy_pks',
    'batch_callback' => 'sirius_ledger_report_discrepancy_batch',
    'render_callbacks' => array(
      'Details' => 'sirius_ledger_report_discrepancy_view',
    ),
    'title_callback' => 'sirius_ledger_report_discrepancy_title',
  );
  return $items;
}

function sirius_ledger_report_discrepancy_form($form, &$form_state, $report_type) {
  report_cache_form_prepare($report_type, $form, $form_state);

  $this_year = date('Y');
  $years = array();
  for ($year = $this_year-10; $year <= $this_year; ++$year) { $years[$year] = $year; }

  $this_month = date('n');
  $months = array(
    '1' => 'January',
    '2' => 'February',
    '3' => 'March',
    '4' => 'April',
    '5' => 'May',
    '6' => 'June',
    '7' => 'July',
    '8' => 'August',
    '9' => 'September',
    '10' => 'October',
    '11' => 'November',
    '12' => 'December',
  );

  $last_year = $this_year;
  $last_month = $this_month-1;
  if ($last_month < 1) {
    $last_month = 12;
    $last_year--;
  }

  $form['year1'] = array(
    '#type' => 'select',
    '#title' => t('First Year'),
    '#options' => $years,
    '#default_value' => $last_year,
    '#required' => TRUE,
  );
  $form['month1'] = array(
    '#type' => 'select',
    '#title' => t('First Month'),
    '#options' => $months,
    '#default_value' => $last_month,
    '#required' => TRUE,
  );

  $form['year2'] = array(
    '#type' => 'select',
    '#title' => t('Second Year'),
    '#options' => $years,
    '#default_value' => $this_year,
    '#required' => TRUE,
  );

  $form['month2'] = array(
    '#type' => 'select',
    '#title' => t('Second Month'),
    '#options' => $months,
    '#default_value' => $this_month,
    '#required' => TRUE,
  );

  $accounts = sirius_ledger_account_list();
  $form['account'] = array(
    '#type' => 'select',
    '#title' => t('Account'),
    '#options' => $accounts,
  );

  $form['charge_or_payment'] = array(
    '#type' => 'select',
    '#title' => 'Show Charges or Payments?',
    '#options' => array(
      'both' => t('Both'),
      'charge' => t('Charges Only'),
      'payment' => t('Payments Only'),
      'range' => t('Custom Range'),
    ),
  );

  $form['ledger_amount_min'] = array(
    '#type' => 'textfield',
    '#title' => 'Minimum Value',
    '#size' => 5,
    '#description' => t('(Remember that charges are positive and payments are negative)'),
    '#states' => array('visible' => array(":input[name=\"charge_or_payment\"]" => array('value' => 'range'))),
  );

  $form['ledger_amount_max'] = array(
    '#type' => 'textfield',
    '#title' => 'Maximum Value',
    '#size' => 5,
    '#states' => array('visible' => array(":input[name=\"charge_or_payment\"]" => array('value' => 'range'))),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Generate',
  );
  
  return $form;
}

function sirius_ledger_report_discrepancy_ts($values) {
  $year1 = $values['year1'];
  $year2 = $values['year2'];
  $month1 = $values['month1'];
  $month2 = $values['month2'];

  $start_ts = strtotime("$year1-$month1-1 00:00:00 UTC");
  $t = date('t', $start_ts_1);
  $end_ts = strtotime("$year1-$month1-$t 23:59:59 UTC");
  $result[':start_ts_1'] = $start_ts;
  $result[':end_ts_1'] = $end_ts;

  $start_ts = strtotime("$year2-$month2-1 00:00:00 UTC");
  $t = date('t', $start_ts_1);
  $end_ts = strtotime("$year2-$month2-$t 23:59:59 UTC");
  $result[':start_ts_2'] = $start_ts;
  $result[':end_ts_2'] = $end_ts;

  return $result;
}

function sirius_ledger_report_discrepancy_form_submit($form, &$form_state) { 
  return report_cache_form_submit($form, $form_state);
}

function sirius_ledger_report_discrepancy_fields() {
	$fields = array();
  $fields['contact_nid'] = 'Contact NID';
  $fields['worker_nid'] = 'Worker NID';
  $fields['worker_name'] = 'Name';
  $fields['worker_ms'] = 'Member Status';
  $fields['worker_ws'] = 'Work Status';
  $fields['balance_1'] = 'First Total';
  $fields['balance_2'] = 'Second Total';
	return $fields;
}

function sirius_ledger_report_discrepancy_pks(&$form_state) {
  $sql_args = sirius_ledger_report_discrepancy_ts($form_state['values']);
  $domain_nid = sirius_domain_nid();

  $sql = "select distinct nid ";
  $sql .= "from node ";
  $sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
  $sql .= "left join sirius_ledger_ar on node.nid = sirius_ledger_ar.ledger_participant ";
  $sql .= "where node.type = 'sirius_contact' ";
  $sql .= "and (";
  $sql .= "   (ledger_ts >= :start_ts_1 and ledger_ts <= :end_ts_1) ";
  $sql .= "   or ";
  $sql .= "   (ledger_ts >= :start_ts_2 and ledger_ts <= :end_ts_2) ";
  $sql .= ") ";
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

function sirius_ledger_report_discrepancy_batch($nids, $values, $report_id, $report_action, &$context) {
  $ms_name_lookup = sirius_taxonomy_load('sirius_member_status', 'tid', 'name');
  $ws_name_lookup = sirius_taxonomy_load('sirius_work_status', 'tid', 'name');
  $ts = sirius_ledger_report_discrepancy_ts($values);

  $rows = array('is_multiple' => TRUE, 'rows' => array());

  $search_params = array(
    'ledger_participant' => $contact_nid,
    'ledger_account' => $values['account'],
    'balance' => TRUE,
  );

  if ($values['charge_or_payment'] == 'charge') {
    $search_params['ledger_amount_min'] = 0;
  } else if ($values['charge_or_payment'] == 'payment') {
    $search_params['ledger_amount_max'] = 0;
  } else if ($values['charge_or_payment'] == 'range') {
    if (isset($values['ledger_amount_max']) && ($values['ledger_amount_max'] !== '')) { $search_params['ledger_amount_max'] = $values['ledger_amount_max']; }
    if (isset($values['ledger_amount_min']) && ($values['ledger_amount_min'] !== '')) { $search_params['ledger_amount_min'] = $values['ledger_amount_min']; }
  }

  foreach ($nids as $contact_nid) {
    $search_params['ledger_ts_min'] = $ts[':start_ts_1'];
    $search_params['ledger_ts_max'] = $ts[':end_ts_1'];
    $balance_1 = sirius_ledger_ar_fetch($search_params);

    $search_params['ledger_ts_min'] = $ts[':start_ts_2'];
    $search_params['ledger_ts_max'] = $ts[':end_ts_2'];
    $balance_2 = sirius_ledger_ar_fetch($search_params);

    if ($balance_1 == $balance_2) { continue; }

    $contact_node = node_load($contact_nid);
    $worker_node = sirius_worker_get_by_contact_nid($contact_nid);
    
    $ms_names = array();
    $ms_tids = sirius_fieldvals($worker_node, 'field_sirius_member_status', 'tid');
    foreach ($ms_tids as $ms_tid) { $ms_names[] = $ms_name_lookup[$ms_tid]; }
    $ms_name = join(', ', $ms_names);

    $ws_name = $ws_name_lookup[$worker_node->field_sirius_work_status['und'][0]['tid']];

    $row = array();
    $row['contact_nid'] = $contact_nid;
    $row['worker_nid'] = $worker_node->nid;
    $row['worker_name'] = $worker_node->title;
    $row['worker_ms'] = $ms_name;
    $row['worker_ws'] = $ws_name;
    $row['balance_1'] = $balance_1;
    $row['balance_2'] = $balance_2;
    $rows['rows'][] = $row;
  }

  return $rows;
}

function sirius_ledger_report_discrepancy_title($report_type, $report_id) {
  // Load the parameters
  $account_lookup = sirius_ledger_account_list();
  $parameters = report_cache_parameters($report_type, $report_id);
  $title = 'Discrepancy Report for ' . $account_lookup[$parameters['account']] . ' between ' . $parameters['month1'] . '/' . $parameters['year1'] . ' and ' . $parameters['month2'] . '/' . $parameters['year2'];
  return $title;
}

function sirius_ledger_report_discrepancy_view($report_type, $report_id) {
  $html = '<h2>' . sirius_ledger_report_discrepancy_title($report_type, $report_id) . '</h2>';
  $parameters = report_cache_parameters($report_type, $report_id);

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

    $tools = l('Worker', 'node/' . $hr['worker_nid'], array('attributes' => array('class' => array('tool-button'))));
    $tools .= l('A/R', 'node/' . $hr['worker_nid'] . '/sirius_worker_ledger', array('attributes' => array('class' => array('tool-button'))));

    $row = array();
    $row[] = $hr['worker_name'];
    $row[] = $hr['worker_ms'];
    $row[] = $hr['worker_ws'];
    $row[] = '$' . number_format($hr['balance_1'], 2);
    $row[] = '$' . number_format($hr['balance_2'], 2);
    $row[] = '$' . number_format($hr['balance_2'] - $hr['balance_1'], 2);
    $row[] = $tools;
    $rows[] = $row;
  }

  $header = array(
    'Worker', 
    'Member Status',
    'Work Status',
    'Total for ' . $parameters['month1'] . '/' . $parameters['year1'],
    'Total for ' . $parameters['month2'] . '/' . $parameters['year2'],
    'Discrepancy',
    'Tools',
  );

  $attributes = array();
  $html .= theme_table(array('rows' => $rows, 'header' => $header, 'attributes' => $attributes));

  return $html;
}