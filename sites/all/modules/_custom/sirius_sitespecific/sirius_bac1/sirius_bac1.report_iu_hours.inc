<?php

function sirius_bac1_report_iu_hours_info() {
  $items['sirius-bac1-iu-hours'] = array(
    'name' => t('IU Hours Report'),
    'description' => 'IU hours and rate report',
    'access' => 'sirius ledger admin',
    'fields' => sirius_bac1_report_iu_hours_fields(),
    'form_callback' => 'sirius_bac1_report_iu_hours_form',
    'pk_list_callback' => 'sirius_bac1_report_iu_hours_pks',
    'batch_callback' => 'sirius_bac1_report_iu_hours_batch',
    'render_callbacks' => array(
      'Total' => 'sirius_bac1_report_iu_hours_view',
    ),
    'title_callback' => 'sirius_bac1_report_iu_hours_title',
  );
  return $items;
}

function sirius_bac1_report_iu_hours_form($form, &$form_state, $report_type) {
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

  $form['year'] = array(
    '#type' => 'select',
    '#title' => t('Year'),
    '#options' => $years,
    '#default_value' => $last_year,
    '#required' => TRUE,
  );

  $form['month'] = array(
    '#type' => 'select',
    '#title' => t('Month'),
    '#options' => $months,
    '#default_value' => $last_month,
    '#required' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Generate',
  );
  
  return $form;
}

function sirius_bac1_report_iu_hours_form_submit($form, &$form_state) { 
  return report_cache_form_submit($form, $form_state);
}

function sirius_bac1_report_iu_hours_fields() {
	$fields = array();

  $fields['contact_nid'] = 'Contact NID';
  $fields['worker_nid'] = 'Worker NID';
  $fields['worker_id'] = 'Worker ID';
  $fields['worker_name_family'] = 'Last Name';
  $fields['worker_name_given'] = 'First Name';
  $fields['classification'] = 'IU Classification';
  $fields['rate'] = 'IU Rate';
  $fields['hours'] = 'Hours';
  $fields['amt'] = 'Amount';

	return $fields;
}

function sirius_bac1_report_iu_hours_pks(&$form_state) {
  $sql_args = array();
  $sql_args['month'] = $form_state['values']['month'];
  $sql_args['year'] = $form_state['values']['year'];

  $sql = "select distinct worker_nid ";
  $sql .= "from sirius_hours_cache ";
  $sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = sirius_hours_cache.payperiod_nid ";
  $sql .= "where month = :month ";
  $sql .= "and year = :year ";
  if ($domain_nid) {
    $sql .= "and field_sirius_domain_target_id = :domain_nid ";
    $sql_args[':domain_nid'] = $domain_nid;
  }
  $sql .= "order by worker_nid ";

  $stmt = sirius_sql_query($sql, $sql_args);
  $nids = array();
  while ($hr = $stmt->fetchAssoc()) {
    $nids[] = $hr['worker_nid'];
  }

  return array_chunk($nids, 5, TRUE);
}

function sirius_bac1_report_iu_hours_batch($nids, $values, $report_id, $report_action, &$context) {
  $rates = sirius_domain_variable_get('bac1_iu_classification_rates');

  $rows = array('is_multiple' => TRUE, 'rows' => array());

  $which = 'default';
  $contact_definition_json = sirius_contact_definition();
  $contact_definition_form = sirius_jsonfield_util_form_get($contact_definition_json, $which);

  $field_name = 'bac1_iu_classification';

  foreach ($nids as $worker_nid) {
    $worker_node = node_load($worker_nid);
    $contact_node = node_load($worker_node->field_sirius_contact['und'][0]['target_id']);
    $contact_json = sirius_json_get($contact_node);
    $contact_results = sirius_jsonfield_util_results_get($contact_json, $which);

    $classification = $contact_results[$field_name];
    $rate = $rates[$classification];

    $sql = "select sum(hours) as s from sirius_hours_cache ";
    $sql .= "where worker_nid = :worker_nid ";
    $sql .= "and month = :month ";
    $sql .= "and year = :year ";
    $sql_args = array();
    $sql_args[':month'] = $values['month'];
    $sql_args[':year'] = $values['year'];
    $sql_args[':worker_nid'] = $worker_nid;
    $stmt = sirius_sql_query($sql, $sql_args);
    $hr = $stmt->fetchAssoc();
    $hours = $hr['s'];

    $row = array();
    $row['contact_nid'] = $contact_node->nid;
    $row['worker_nid'] = $worker_node->nid;
    $row['worker_id'] = $worker_node->field_sirius_id['und'][0]['value'];
    $row['worker_name_family'] = $worker_node->field_sirius_name['und'][0]['family'];
    $row['worker_name_given'] = $worker_node->field_sirius_name['und'][0]['given'];
    $row['classification'] = $classification;
    $row['rate'] = $rate;
    $row['hours'] = $hours;
    $row['amt'] = $rate * $hours;

    $rows['rows'][$worker_nid] = $row;
  }
  return $rows;
}

function sirius_bac1_report_iu_hours_title($report_type, $report_id) {
  // Load the parameters
  $account_lookup = sirius_ledger_account_list();
  $parameters = report_cache_parameters($report_type, $report_id);
  $title = 'IU Hours Report: ' . $parameters['month'] . '/' . $parameters['year'];
  return $title;
}

function sirius_bac1_report_iu_hours_view($report_type, $report_id) {
  $html = '<h2>' . sirius_bac1_report_iu_hours_title($report_type, $report_id) . '</h2>';
  $parameters = report_cache_parameters($report_type, $report_id);

  $query = db_select('report_cache');
  $query->fields('report_cache');
  $query->condition('report_id', $report_id);
  $query->condition('report_type', $report_type);
  $stmt = $query->execute();

  $data = array();
  while ($hr = $stmt->fetchAssoc()) {
    $hr = unserialize($hr['report_data']);
    $data[$hr['classification']]['amt'] += $hr['amt'];
    $data[$hr['classification']]['hours'] += $hr['hours'];
    $data['total']['amt'] += $hr['amt'];
    $data['total']['hours'] += $hr['hours'];
  }

  $rates = sirius_domain_variable_get('bac1_iu_classification_rates');

  $header = array(
    'Classification', 
    'Hours', 
    'Rate',
    'Amount', 
  );

  $rows = array();
  foreach ($rates as $classification => $rate) {
    $row = array();
    $row[] = $classification;
    $row[] = $data[$classification]['hours'];
    $row[] = $rates[$classification];
    $row[] = number_format($data[$classification]['amt'], 2);
    $rows[] = $row;
  }
  $row = array();
  $row[] = '<strong>' . t('Total') . '</strong>';
  $row[] = '<strong>' . $data['total']['hours'] . '</strong>';
  $row[] = '';
  $row[] = '<strong>' . number_format($data['total']['amt'], 2) . '</strong>';
  $rows[] = $row;

  $attributes = array();


  $html .= theme_table(array('rows' => $rows, 'header' => $header, 'attributes' => $attributes));

  return $html;
}