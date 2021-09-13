<?php

function sirius_hour_report_by_ms_info() {
  $items['sirius-hour-by-ms'] = array(
    'name' => t('Preceding Month Hours by Member Status'),
    'description' => 'Report the total hours in the preceding month by current member status',
    'access' => 'sirius hour admin',
    'fields' => sirius_hour_report_by_ms_fields(),
    'form_callback' => 'sirius_hour_report_by_ms_form',
    'pk_list_callback' => 'sirius_hour_report_by_ms_pks',
    'batch_callback' => 'sirius_hour_report_by_ms_batch',
    'render_callbacks' => array(
      'Details' => 'sirius_hour_report_by_ms_view',
    ),
    'title_callback' => 'sirius_hour_report_by_ms_title',
  );
  return $items;
}

function sirius_hour_report_by_ms_form($form, &$form_state, $report_type) {
  report_cache_form_prepare($report_type, $form, $form_state);

  $this_year = date('Y');
  $this_month = date('n');

  --$this_month;
  if ($month < 1) { $month = 12; --$this_year; }
  $form['year'] = array('#type' => 'hidden', '#value' => $this_year);
  $form['month'] = array('#type' => 'hidden', '#value' => $this_month);

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Generate',
  );
  
  return $form;
}

function sirius_hour_report_by_ms_form_submit($form, &$form_state) { 
  return report_cache_form_submit($form, $form_state);
}

function sirius_hour_report_by_ms_fields() {
	$fields = array();
  $fields['employer_nid'] = 'Employer NID';
  $fields['ms_tid'] = 'Member Status TID';
  $fields['ms_name'] = 'Member Status Name';
  $fields['industry_tid'] = 'Industry TID';
  $fields['industry_name'] = 'Industry Name';
  $fields['hours'] = 'Total Hours';
	return $fields;
}

function sirius_hour_report_by_ms_pks(&$form_state) {
  $domain_nid = sirius_domain_nid();

  $ms_terms = sirius_taxonomy_load('sirius_member_status', 'tid', 'name');
  return array_keys($ms_terms);
}

function sirius_hour_report_by_ms_batch($ms_tid, $values, $report_id, $report_action, &$context) {
  $ms_name_lookup = sirius_taxonomy_load('sirius_member_status', 'tid', 'name');

  $month = $values['month'];
  $year = $values['year'];

  $rows = array('is_multiple' => TRUE, 'rows' => array());
  return $rows;

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
    $search_params['ledger_participant'] = $contact_nid; 
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
    $row['worker_id'] = $worker_node->field_sirius_id['und'][0]['value'];
    $row['worker_id2'] = $worker_node->field_sirius_id2['und'][0]['value'];
    $row['worker_name_family'] = $worker_node->field_sirius_name['und'][0]['family'];
    $row['worker_name_given'] = $worker_node->field_sirius_name['und'][0]['given'];
    $row['worker_ms'] = $ms_name;
    $row['worker_ws'] = $ws_name;
    $row['balance_1'] = $balance_1;
    $row['balance_2'] = $balance_2;
    $rows['rows'][$contact_nid] = $row;
  }

  return $rows;
}

function sirius_hour_report_by_ms_title($report_type, $report_id) {
  // Load the parameters
  $account_lookup = sirius_ledger_account_list();
  $parameters = report_cache_parameters($report_type, $report_id);
  dpm($parameters);
  $title = 'Total Hours by Member Status -  ' . $account_lookup[$parameters['account']] . ' between ' . $parameters['month1'] . '/' . $parameters['year1'] . ' and ' . $parameters['month2'] . '/' . $parameters['year2'];
  return $title;
}

function sirius_hour_report_by_ms_view($report_type, $report_id) {
  $html = '<h2>' . sirius_hour_report_by_ms_title($report_type, $report_id) . '</h2>';
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
    $row[] = $hr['worker_id'];
    $row[] = $hr['worker_id2'];
    $row[] = $hr['worker_name_family'];
    $row[] = $hr['worker_name_given'];
    $row[] = $hr['worker_ms'];
    $row[] = $hr['worker_ws'];
    $row[] = '$' . number_format($hr['balance_1'], 2);
    $row[] = '$' . number_format($hr['balance_2'], 2);
    $row[] = '$' . number_format($hr['balance_2'] - $hr['balance_1'], 2);
    $row[] = $tools;
    $rows[] = $row;
  }

  $header = array(
    'ID', 
    'ID2', 
    'Last Name', 
    'First Name', 
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