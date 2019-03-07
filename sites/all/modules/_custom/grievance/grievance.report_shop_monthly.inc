<?php

function grievance_report_shop_monthly() {
  return drupal_get_form('grievance_report_shop_monthly_form');
}

function grievance_report_shop_monthly_form($form, &$form_state) {
  $report_type = 'grievance-report-shop-monthly';
  report_cache_form_prepare($report_type, $form, $form_state);

  $years = array();
  for ($i=date('Y')-10; $i<date('Y')+2; ++$i) {
    $years[$i] = $i;
  }
  $months = array(
    1 => '1 - January',
    2 => '2 - February',
    3 => '3 - March',
    4 => '4 - April',
    5 => '5 - May',
    6 => '6 - June',
    7 => '7 - July',
    8 => '8 - August',
    9 => '9 - September',
    10 => '10 - October',
    11 => '11 - November',
    12 => '12 - December',
  );

  $form['start_year'] = array(
    '#type' => 'select',
    '#title' => 'Start Year',
    '#required' => TRUE,
    '#options' => $years,
    '#default_value' => date('Y'),
  );

  $form['start_month'] = array(
    '#type' => 'select',
    '#title' => 'Start Month',
    '#required' => TRUE,
    '#options' => $months,
    '#default_value' => date('n'),
  );

  $form['end_year'] = array(
    '#type' => 'select',
    '#title' => 'End Year',
    '#required' => TRUE,
    '#options' => $years,
    '#default_value' => date('Y'),
  );

  $form['end_month'] = array(
    '#type' => 'select',
    '#title' => 'End Month',
    '#required' => TRUE,
    '#options' => $months,
    '#default_value' => date('n'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Generate',
  );
  
  return $form;
}

function grievance_report_shop_monthly_form_submit($form, &$form_state) { return report_cache_form_submit($form, $form_state); }

function grievance_report_shop_monthly_fields() {
  return array(
    'shop_nid' => 'Shop NID',
    'shop_name' => 'Employer',
    'month' => 'Month',
    'year' => 'Year',
    'opened' => 'Opened',
  );
}

function grievance_report_shop_monthly_pks(&$form_state) {
  $from_ts = strtotime($form_state['values']['from_date']);
  $to_ts = strtotime($form_state['values']['to_date']);
  $from_date = date('Y-m-d 00:00:00', $from_ts);
  $to_date = date('Y-m-d 00:00:00', strtotime('+1 day', $to_ts));

  $query = new EntityFieldQuery;
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'grievance_shop');
  $stmt = $query->execute();
  $nids = array();
  if ($stmt['node']) {
    $nids = array_keys($stmt['node']);
  }
  return $nids;
}

function grievance_report_shop_monthly_batch($shop_nid, $values, $report_id, $report_action, &$context) {
	$rows = array(
		'is_multiple' => TRUE,
		'rows' => array(),
	);

  // Find the name of this shop
  $sql = "select title from node where nid = :shop_nid";
  $stmt = db_query($sql, array(':shop_nid' => $shop_nid));
  $hr = $stmt->fetchAssoc();
  $shop_name = $hr['title'];

  $start_month = $values['start_month'];
  $start_year = $values['start_year'];
  $end_month = $values['end_month'];
  $end_year = $values['end_year'];

  $this_month = $start_month;
  $this_year = $start_year;
  $count = 0;
  while ($this_year < $end_year || ($this_year == $end_year && $this_month <= $end_month)) {
    ++$count;
    if ($count > 1000) { break; }

    // Find the number of grievances opened this month
    $last_day = date('t', strtotime("$this_year-$this_month-1"));
    $start_ts = strtotime("$this_year-$this_month-1");
    $end_ts = strtotime("$this_year-$this_month-$last_day 23:59:59");

    $sql = "select count(*) as c from node ";
    $sql .= "left join field_data_field_grievance_shop on field_data_field_grievance_shop.entity_type = 'node' and field_data_field_grievance_shop.entity_id = node.nid ";
    $sql .= "where node.type = 'grievance' ";
    $sql .= "and field_grievance_shop_target_id = :shop_nid ";
    $sql .= "and node.created >= :start_ts ";
    $sql .= "and node.created <= :end_ts ";
    $stmt = db_query($sql, array(':start_ts' => $start_ts, ':end_ts' => $end_ts, ':shop_nid' => $shop_nid));
    $hr = $stmt->fetchAssoc();
    $count = $hr['c'];

    if ($count) {
      $rows['rows']["$shop_nid-$this_year_$this_month"] = array('shop_nid' => $shop_nid, 'shop_name' => $shop_name, 'month' => $this_month, 'year' => $this_year, 'opened' => $count);
    }

    ++$this_month;
    if ($this_month > 12) {
      $this_month = 1;
      ++$this_year;
    }
  }

  return $rows;
}

function grievance_report_shop_monthly_lookups() {
  return array();
}

function grievance_report_shop_monthly_render($report_type, $report_id) {
  // Load the parameters
  $query = db_select('report_cache');
  $query->fields('report_cache');
  $query->condition('report_id', $report_id);
  $query->condition('report_type', $report_type . ' :: PARAMETERS');
  $stmt = $query->execute();
  $hr = $stmt->fetchAssoc();
  $parameters = unserialize($hr['report_data']);

  $html = '';
  $html .= '<div class="grievance-report-settlement-render">';
  $html .= '<h2>';
  $html .= t('Grievances opened by shop, monthly @start_date to @end_date', array(
      '@start_date' => $parameters['start_month'] . '/' . $parameters['start_year'],
      '@end_date' => $parameters['end_month'] . '/' . $parameters['end_year'],
    )
  );
  $html .= '</h2>';

  $query = db_select('report_cache');
  $query->fields('report_cache');
  $query->condition('report_id', $report_id);
  $query->condition('report_type', $report_type);
  $stmt = $query->execute();
  $data = array();
  $grand_total = 0;
  while ($hr = $stmt->fetchAssoc()) {
    $hr = unserialize($hr['report_data']);
    $grand_total += $hr['opened'];
    $pk = $hr['shop_name'] . ' - ' . $hr['year'] . ' - ' . $hr['month'];
    $data[$pk] = $hr;
  }

  ksort($data);

  $header = array();
  $header[] = 'Employer';
  $header[] = 'Year';
  $header[] = 'Month';
  $header[] = 'Opened';

  $rows = array();
  foreach ($data as $pk => $hr) {
    $row = array();
    $row[] = $hr['shop_name'];
    $row[] = $hr['year'];
    $row[] = $hr['month'];
    $row[] = $hr['opened'];
    $rows[] = $row;
  }

  $row = array();
  $row[] = '<strong>' . t('Total') . '</strong>';
  $row[] = '';
  $row[] = '';
  $row[] = '<strong>' . $grand_total . '</strong>';
  $rows[] = $row;

  $attributes = array();

  $html .= theme_table(array('header' => $header, 'rows' => $rows, 'attributes' => $attributes));
  $html .= '</div>';

  return $html;
}


