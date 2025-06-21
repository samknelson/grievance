<?php

class Sirius_Trust_Report_Hour extends Report_Cache_Report_Generator {
  public $type = 'sirius-trust-hour';

  public function info($result = array()) {
    $result = array(
      'name' => t('Hours - Overview'),
      'access' => 'sirius trust staff',
      'render_callbacks' => array(
        'Summary' => array(
          'method' => 'render_summary',
          'csv' => TRUE,
        ),
      ),
      'render_parameters_callback' => 'sirius_trust_report_hour_render_parameters',
      'module' => 'sirius_trust',
      'file' => 'sirius_trust.report_hour.inc',
      'dashboard' => TRUE,
      'preview' => array(
        'default_display_fields' => array(
          'worker_name',
          'triggering_employer_name',
          'hours_total',
          'hours_at_tirggering_employer',
          'hours_by_month_0',
          'hours_by_month_1',
          'hours_by_month_2',
          'hours_by_month_3',
          'hours_by_month_4',
          'hours_by_month_5',
        ),
        'preview_links' => TRUE,
      ),
    );

    return parent::info($result);
  }

  public function form() {
    $form['yearmonth'] = sirius_date_yearmonth_widget(array());

    return $form;
  }

  public function fields() {
    $fields = array();
    $fields['pk'] = 'PK';
    $fields['worker_nid'] = 'Worker NID';
    $fields['worker_id'] = 'Worker ID';
    $fields['worker_name'] = 'Worker Name';
    $fields['worker_phone'] = 'Worker Phone';
    $fields['worker_email'] = 'Worker Email';
    foreach (sirius_addr_cols() as $col => $name) {
      $fields["worker_address_$col"] = "Worker $name";
    }

    // $fields['current_employer_nid'] = 'Current Employer NID';
    // $fields['current_employer_name'] = 'Current Employer Name';

    $fields['triggering_employer_nid'] = 'Primary Employer NID';
    $fields['triggering_employer_name'] = 'Primary Employer Name';
    $fields['hours_total'] = 'Total Hours';
    $fields['hours_at_tirggering_employer'] = 'Hours at Primary Employer';
    $fields['hours_by_month_0'] = 'Hours by Month - Start Month';
    $fields['hours_by_month_1'] = 'Hours by Month - 1 Month Prev';
    $fields['hours_by_month_2'] = 'Hours by Month - 2 Months Prev';
    $fields['hours_by_month_3'] = 'Hours by Month - 3 Months Prev';
    $fields['hours_by_month_4'] = 'Hours by Month - 4 Months Prev';
    $fields['hours_by_month_5'] = 'Hours by Month - 5 Months Prev';

    return $fields;
  }

  public function preview_links($row) {
    return array(
      'node/' . $row['worker_nid'] => 'Worker',
      'node/' . $row['worker_nid'] . '/sirius_worker_hours' => 'Hours',
    );
  }

  public function pks(&$form_state) {
    $domain_nid = sirius_domain_nid();
    $sql_args = array();

    if ($form_state['values']['parameters']['limits']['ms']) { $ms = array_filter(array_values($form_state['values']['parameters']['limits']['ms'])); }
    if ($form_state['values']['parameters']['limits']['ws']) { $ws = array_filter(array_values($form_state['values']['parameters']['limits']['ws'])); }

    $year = $form_state['values']['parameters']['yearmonth']['year'];
    $month = $form_state['values']['parameters']['yearmonth']['month'];

    $end_date_format = date('Ymt', strtotime("$year-$month-1"));
    $start_date_format = date('Ym1', strtotime("-6 months", strtotime("$year-$month-1")));

    $sql = "select distinct worker_nid from node ";
    $sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
    $sql .= "left join field_data_field_sirius_member_status on field_data_field_sirius_member_status.entity_type = 'node' and field_data_field_sirius_member_status.entity_id = node.nid ";
    $sql .= "left join field_data_field_sirius_work_status on field_data_field_sirius_work_status.entity_type = 'node' and field_data_field_sirius_work_status.entity_id = node.nid ";
    $sql .= "left join sirius_hours_cache on sirius_hours_cache.worker_nid = node.nid ";
    $sql .= "where node.type = 'sirius_worker' ";
    $sql .= "and sirius_hours_cache.hours is not null ";
    $sql .= "and concat(year, lpad(month, 2, 0), lpad(day, 2, 0)) >= :start_date_format ";
    $sql_args[':start_date_format'] = $start_date_format;
    $sql .= "and concat(year, lpad(month, 2, 0), lpad(day, 2, 0)) <= :end_date_format ";
    $sql_args[':end_date_format'] = $end_date_format;
    if ($domain_nid) { 
      $sql .= "and field_sirius_domain_target_id = :domain_nid ";
      $sql_args[':domain_nid'] = $domain_nid;
    } else {
      $sql .= "and field_sirius_domain_target_id is NULL ";
    }
    if ($ms) {
      $sql .= "and field_sirius_member_status_tid in (:ms) ";
      $sql_args[':ms'] = $ms;
    }
    if ($ws) {
      $sql .= "and field_sirius_work_status_tid in (:ws) ";
      $sql_args[':ws'] = $ws;
    }
    $sql .= "order by worker_nid desc ";
    // $sql .= "limit 1000 ";

    $stmt = sirius_sql_query($sql, $sql_args);
    $pks = array();
    while ($hr = $stmt->fetchAssoc()) {
      $pks[] = $hr['worker_nid'];
    }
    return array_chunk($pks, 100, TRUE);
  }

  public function batch($pks, $values, &$context) {
    $year = $values['parameters']['yearmonth']['year'];
    $month = $values['parameters']['yearmonth']['month'];

    $end_date_format = date('Ymt', strtotime("$year-$month-1"));
    $start_date_format = date('Ym1', strtotime("-6 months", strtotime("$year-$month-1")));

    $rows = array('is_multiple' => TRUE, 'rows' => array());
    foreach ($pks as $pk) {
      $worker_nid = $pk;

      // Total hours in range, by employer
      $sql = "select sum(hours) as c, employer_nid ";
      $sql .= " from sirius_hours_cache ";
      $sql .= "where worker_nid = :worker_nid ";
      $sql .= "and concat(year, lpad(month, 2, 0), lpad(day, 2, 0)) >= :start_date_format ";
      $sql .= "and concat(year, lpad(month, 2, 0), lpad(day, 2, 0)) <= :end_date_format ";
      $sql .= "group by employer_nid ";
      $sql .= "order by employer_nid ";
      $sql_args = array();
      $sql_args[':worker_nid'] = $worker_nid;
      $sql_args[':start_date_format'] = $start_date_format;
      $sql_args[':end_date_format'] = $end_date_format;
      $stmt = sirius_sql_query($sql, $sql_args);

      $hours = $hours_at_tirggering_employer = $triggering_employer_nid = 0;
      $hours_by_employer = array();
      while ($hr = $stmt->fetchAssoc()) {
        $hours_by_employer[$hr['employer_nid']] = $hr['c'];
        $hours += $hr['c'];
      }
      $triggering_employer_nid = array_keys($hours_by_employer, max($hours_by_employer))[0];
      $hours_at_tirggering_employer = $hours_by_employer[$triggering_employer_nid];

      // Number of hours in each of the preceding six months
      $delta_month = $month;
      $delta_year = $year;
      for ($delta = 0; $delta < 6; ++$delta) {
        // Total hours in range
        $sql = "select sum(hours) as c ";
        $sql .= " from sirius_hours_cache ";
        $sql .= "where worker_nid = :worker_nid ";
        $sql .= "and year = :delta_year ";
        $sql .= "and month = :delta_month ";
        $sql_args = array();
        $sql_args[':worker_nid'] = $worker_nid;
        $sql_args[':delta_year'] = $delta_year;
        $sql_args[':delta_month'] = $delta_month;
        $stmt = sirius_sql_query($sql, $sql_args);
        $hr = $stmt->fetchAssoc();
        $hours_by_month[$delta] = $hr['c'];

        --$delta_month;
        if ($delta_month < 1) { $delta_month = 12; --$delta_year; }
      }

      // Total hours in range
      $sql = "select sum(hours) as c ";
      $sql .= " from sirius_hours_cache ";
      $sql .= "where worker_nid = :worker_nid ";
      $sql .= "and employer_nid = :triggering_employer_nid ";
      $sql .= "and concat(year, lpad(month, 2, 0), lpad(day, 2, 0)) >= :start_date_format ";
      $sql .= "and concat(year, lpad(month, 2, 0), lpad(day, 2, 0)) <= :end_date_format ";
      $sql_args = array();
      $sql_args[':worker_nid'] = $worker_nid;
      $sql_args[':triggering_employer_nid'] = $triggering_employer_nid;
      $sql_args[':start_date_format'] = $start_date_format;
      $sql_args[':end_date_format'] = $end_date_format;
      $stmt = sirius_sql_query($sql, $sql_args);
      $hr = $stmt->fetchAssoc();
      $hours_at_tirggering_employer = $hr['c'];

      // Looks good!
      $worker_node = node_load($worker_nid);
      $triggering_employer_name = sirius_node_title($triggering_employer_nid);
      $current_employer_nid = $worker_node->field_grievance_shop['und'][0]['target_id'];
      $current_employer_name = sirius_node_title($current_employer_nid);

      $row = array();
      $row['pk'] = $pk;
      $row['worker_nid'] = $worker_nid;
      $row['worker_id'] = $worker_node->field_sirius_id['und'][0]['value'];
      $row['worker_name'] = $worker_node->title;
      $row['worker_phone'] = $worker_node->field_sirius_phone['und'][0]['value'];
      $row['worker_email'] = $worker_node->field_sirius_email['und'][0]['value'];
      foreach (sirius_addr_cols() as $col => $name) {
        $row["worker_address_$col"] = $worker_node->field_sirius_address['und'][0][$col];
      }
      $row['hours_total'] = round($hours, 2);
      $row['hours_at_tirggering_employer'] = round($hours_at_tirggering_employer, 2);
      for ($delta = 0; $delta<6; ++$delta) {
        $row["hours_by_month_$delta"] = round($hours_by_month[$delta], 2);
      }

      // $row['current_employer_nid'] = $current_employer_nid;
      // $row['current_employer_name'] = $current_employer_name;

      $row['triggering_employer_nid'] = $triggering_employer_nid;
      $row['triggering_employer_name'] = $triggering_employer_name;

      $rows['rows'][$pk] = $row;
    }

    return $rows;
  }

  public function render_parameters($parameters) {
    $parameters = $parameters['parameters'];
    $html .= sirius_fakerender_field('As Of:', $parameters['yearmonth']['month'] . ' / ' . $parameters['yearmonth']['year']);
    return $html;
  }

  public function render_summary($report_id, $output = 'page', $filter_values) {
    $default_threshold = 60;
    if (isset($filter_values['threshold'])) {
      $threshold = $filter_values['threshold'];
    } else {
      $threshold = $default_threshold;
    }

    $filters = array();
    $filters['threshold'] = array(
      '#type' => 'textfield',
      '#title' => t('Hours Threshold'),
      '#size' => 4,
      '#default_value' => $default_threshold,
    );

    $info = $this->info();

    $query = db_select('report_cache');
    $query->fields('report_cache');
    $query->condition('report_id', $report_id);
    $query->condition('report_type', $this->type);
    $stmt = $query->execute();

    $data = array();
    $totals = array();
    while ($hr = $stmt->fetchAssoc()) {
      $hr = unserialize($hr['report_data']);
      $employer = $hr['triggering_employer_name'];
      for ($delta=0; $delta<6; ++$delta) {

        if ($hr["hours_by_month_$delta"] >= $threshold) {
          if ($delta == 0) {
            ++$data[$employer]['above']; 
            ++$totals['above'];
          }
          break;
        }

        ++$data[$employer]["below_$delta"];
        ++$totals["below_$delta"];
      }
      $hr = unserialize($hr['report_data']);
      // $data[$employer]['total']++;
      $totals['total']++;
    }
    ksort($data);

    $header = array();
    $header[] = t('Employer');
    $header[] = t('Above Threshold');
    $header[] = t('Below Threshold 1 Month');
    $header[] = t('Below Threshold 2 Months');
    $header[] = t('Below Threshold 3 Months');
    $header[] = t('Below Threshold 4 Months');
    $header[] = t('Below Threshold 5 Months');
    $header[] = t('Below Threshold 6 Months');

    $rows = array();
    foreach ($data as $employer_name => $record) {
      $row = array();
      $row[] = $employer_name;
      $row[] = sirius_number_format($record['above'], 0, '.', ',', $output);
      for ($delta = 0; $delta < 6; ++$delta) {
        $row[] = sirius_number_format($record["below_$delta"], 0, '.', ',', $output);
      }
      $rows[] = $row;
    }

    $row = array();
    $row['class'] = array('report_cache_total');
    $row['data'] = array();
    $row['data'][] = t('Total');
    $row['data'][] = sirius_number_format($totals['above'], 0, '.', ',', $output);
    for ($delta = 0; $delta < 6; ++$delta) {
      $row['data'][] = sirius_number_format($totals["below_$delta"], 0, '.', ',', $output);
    }
    $rows[] = $row;
    return array('header' => $header, 'rows' => $rows, 'filters' => $filters);
  }
}

function sirius_trust_report_hour_view_form($form, &$form_state) {
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
