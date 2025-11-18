<?php

class Sirius_Smf_Report_Emptier extends Report_Cache_Report_Generator {
  public $type = 'sirius-smf-employer-tier';

  /*
  NAME - Employer Name
  WORK MONTH (CONT MON) - The month the hours were reported/contributed.  This is stupid, I need to talk to Pete about this.  We need to make this the work month.
  HN - Total number of workers at the employer who are in healthnet as subscribers.
  HN T1 - Total number of subscribers with Healthnet that have zero depdents (self only)
  HN T2 - Total number of subscribers with Healthnet that have exactly one dependent
  HN T3 - Total number of subscribers with Healthnet that have more than one dependent
  KP - Total number of workers at the employer who are in Kaiser as subscribers
  KP T1 - as above but for Kaiser
  KP T2 - as above but for Kaiser
  KP T3 - as above but for Kaiser
  MLK  - Total number of workers at the employer who are in MLK as subscribers
  M T1 - as above but for MLK
  M T2 - as above but for MLK
  M T3 - as above but for MLK
  VSP  - Total number of workers at the employer who are in VSP as subscribers
  V T1 - as above but for VSP
  V T2 - as above but for VSP
  V T3 - as above but for VSP
  DD - Total number of workers at the employer who are in Delta Dental as subscribers
  UC-Liberty - Total number of workers at the employer who are in Liberty as subscribers
  LADC - Total number of workers at the employer who are in UniteHere Dental Center as subscribers
  NON ENROLLED - Total number of Workers with dental but no health.
  HOURS - Total hours reported for the month or the ER
  NON ELIG - Total number of workers who did not have AD&D but did have hours reported
  NON ELIG HRS - Total hours reported for workers who did not have AD&D
  EL HRS - Total hours reported for workers who did have AD&D
  NON ENR NO DDS - Total number of workers with AD&D but no health and no dental
  */
  public function info($result = array()) {
    $result = array(
      'name' => t('Employer Tier'),
      'description' => '',
      'access' => 'sirius trust staff',
      'preview' => array(
        /*
        'default_display_fields' => array(
          'first_name',
          'last_name',
          'election',
          'employer',
          'life',
          'medical',
          'subscriber',
          'covered_lives',
          'tier',
          'hours_0',
          'hours_1',
          'hours_2',
          'hours_3',
          'hours_4',
        ),
        */
        'preview_links' => TRUE,
      ),
      'render_callbacks' => array(
        'By Employer' => array(
          'method' => 'render_by_employer',
          'csv' => TRUE,
        ),
      ),
      'dashboard' => TRUE,
      'module' => 'sirius_smf',
      'file' => 'reports/emptier.inc',
      'row_encoding' => 'json',
    );

    return parent::info($result);
  }

  public function fields() {
    $fields[] = 'worker_nid';
    $fields[] = 'employer_nid';
    $fields[] = 'worker_name';
    $fields[] = 'employer_name';
    $fields[] = 'employer_id';
    $fields[] = 'WORK MONTH';
    $fields[] = 'HN';
    $fields[] = 'HN T1';
    $fields[] = 'HN T2';
    $fields[] = 'HN T3';
    $fields[] = 'KP';
    $fields[] = 'KP T1';
    $fields[] = 'KP T2';
    $fields[] = 'KP T3';
    $fields[] = 'MLK';
    $fields[] = 'MLK T1';
    $fields[] = 'MLK T2';
    $fields[] = 'MLK T3';
    $fields[] = 'VSP';
    $fields[] = 'VSP T1';
    $fields[] = 'VSP T2';
    $fields[] = 'VSP T3';
    $fields[] = 'DD';
    $fields[] = 'UC-Liberty';
    $fields[] = 'LADC';
    $fields[] = 'NON ENROLLED';
    $fields[] = 'HOURS';
    $fields[] = 'TOTAL HOURS';
    $fields[] = 'MULTIPLIER';
    $fields[] = 'NON ELIG';
    $fields[] = 'NON ELIG HRS';
    $fields[] = 'EL HRS';
    $fields[] = 'NON ENR NO DDS';
    $fields[] = 'MEDICAL';
    $fields[] = 'DENTAL';
    $fields[] = 'LIFE';

    return array_combine($fields, $fields);
  }

  public function preview_links($row) {
    $links = array();
    if ($row['worker_nid']) { $links['node/' . $row['worker_nid']] = 'Worker'; }
    return $links;
  }

  public function form() {
    $form['year'] = array(
      '#type' => 'textfield',
      '#title' => t('Year'),
      '#size' => 4,
      '#default_value' => date('Y'),
      '#required' => TRUE,
    );

    $form['month'] = array(
      '#title' => t('Month'),
      '#type' => 'textfield',
      '#default_value' => date('m'),
      '#size' => 2,
      '#required' => TRUE,
    );

    return $form;
  }

  public function form_validate($form, &$form_state) {
    $parameters = $form_state['values']['parameters'];

    $year = $parameters['year'];
    $month = $parameters['month'];

    if (!$year || 
      $year != intval($year) ||
      $year <= 1900 || 
      $year >= 2100
    ) {
      form_set_error("parameters][year", 'Please enter a start year.');
    }

    if (!$month || 
      $month != intval($month) ||
      $month < 1 || 
      $month > 12
    ) {
      form_set_error("parameters][month", 'Please enter a start month.');
    }

    if ($end_year) {
      if ($end_year != intval($end_year) ||
        $end_year <= 1900 || 
        $end_year >= 2100
      ) {
        form_set_error("parameters][start_year", 'Please enter a start year.');
      }
    }
  }

  public function render_parameters($parameters) {
    $parameters = $parameters['parameters'];

    $html = sirius_jsonfield_util_render_all_values_new(
      $this->form(), 
      $parameters, 
      array(
        'skip_empty' => TRUE,
        'fieldset_title_prefix' => '<h2>',
        'fieldset_title_suffix' => '</h2>',
        'no_fieldsets' => TRUE,
      )
    );
    return $html;
  }

  public function pks(&$form_state) {
    $parameters = $form_state['values']['parameters'];

    // Damn sirius_hours_cache doesn't let you query by date range, it's only got month and year.
    $y = $parameters['year'] + 0;
    $m = $parameters['month'] + 0;

    for ($i=0; $i<3; ++$i) {
      --$m; if ($m < 1) { $m = 12; --$y; }
    }

    $worker_nids = array();

    $sql = "select distinct employer_nid, worker_nid from sirius_hours_cache ";
    $sql .= "where year = :y ";
    $sql .= "and month = :m ";
    $stmt = sirius_sql_query($sql, array(':y' => $y, ':m' => $m));

    $format = '%010d';
    $pks = array();
    while ($hr = $stmt->fetchAssoc()) {
      $pks[] = sprintf($format, $hr['employer_nid']) . ':' . sprintf($format, $hr['worker_nid']);
    }
    return array_chunk($pks, 100);
  }

  public function batch($pks, $values, &$context) {
    $parameters = $values['parameters'];
    $date = $parameters['year'] . '-' . $parameters['month'] . '-15';

    $ts = strtotime($date);
    $work_ts = strtotime('-3 months', $ts);
    $work_display = date('Y-m', $work_ts);
    $work_year = date('Y', $work_ts);
    $work_month = date('n', $work_ts);

    $map = array();
    $map['HN'] = array('benefit_ids' => array('H'), 'tiered' => TRUE);
    $map['KP'] = array('benefit_ids' => array('K', 'KE'), 'tiered' => TRUE);
    $map['MLK'] = array('benefit_ids' => array('M'), 'tiered' => TRUE);
    $map['VSP'] = array('benefit_ids' => array('3', '3E'), 'tiered' => TRUE);
    $map['DD'] = array('benefit_ids' => array('D'));
    $map['UC-Liberty'] = array('benefit_ids' => array('L'));
    $map['LADC'] = array('benefit_ids' => array('LADC'));
    $map['ADD'] = array('benefit_ids' => array('ADD'));

    foreach ($map as $key => $record) {
      foreach ($record['benefit_ids'] as $benefit_id) {
        $benefit_node = sirius_trust_benefit_get_by_id($benefit_id);
        if (!$benefit_node) { drupal_set_message("Unable to find benefit with ID '$benefit_id'", 'error'); return array(); }
        $map[$key]['benefit_nids'][] = $benefit_node->nid;

        $benefit_type_term = taxonomy_term_load($benefit_node->field_sirius_trust_benefit_type['und'][0]['tid']);
        $map[$key]['type'] = $benefit_type_term->field_sirius_id['und'][0]['value'];
      }
    }

    $employer_lookup = array();

    $rows = array();
    foreach ($pks as $pk) {
      list ($employer_nid, $worker_nid) = explode(':', $pk);
      $employer_nid += 0;
      $worker_nid += 0;

      $worker_node = node_load($worker_nid);

      if (!$employer_lookup[$employer_nid]) {
        $employer_lookup[$employer_nid] = node_load($employer_nid);
      }

      $row = array();

      $row['worker_nid'] = $worker_nid;
      $row['employer_nid'] = $employer_nid;
      $row['worker_name'] = $worker_node->title;
      $row['employer_name'] = $employer_lookup[$employer_nid]->title;
      $row['employer_id'] = $employer_lookup[$employer_nid]->field_sirius_id['und'][0]['value'];

      $row['WORK MONTH'] = $work_display;

      foreach ($map as $key => $record) {
        $params = array();
        $params['worker_nid'] = $worker_nid;
        $params['ts'] = $ts;
        $params['benefit_nids'] = $record['benefit_nids'];
        $probe = sirius_trust_worker_benefits_search($params);
        if ($probe) {
          $row[$key] = 1;
          $row[$record['type']] = 1;
        }

        if ($record['tiered']) {
          $params = array();
          $params['subscriber_nid'] = $worker_nid;
          $params['ts'] = $ts;
          $params['benefit_nids'] = $record['benefit_nids'];
          $probe = sirius_trust_worker_benefits_search($params);
          $count = count($probe);
          if ($count == 1) {
            $row["$key T1"] = 1;
          } else if ($count == 2) {
            $row["$key T2"] = 1;
          } else if ($count > 2) {
            $row["$key T3"] = 1;
          }
        }
      }

      $sql = "select sum(hours) as c from sirius_hours_cache ";
      $sql .= "where worker_nid = :worker_nid ";
      $sql .= "and year = :work_year ";
      $sql .= "and month = :work_month ";
      $sql .= "and employer_nid = :employer_nid ";
      $stmt = sirius_sql_query($sql, array(':worker_nid' => $worker_nid, ':work_year' => $work_year, ':work_month' => $work_month, ':employer_nid' => $employer_nid));
      $hr = $stmt->fetchAssoc();
      $employer_hours = round($hr['c'], 2);

      $sql = "select sum(hours) as c from sirius_hours_cache ";
      $sql .= "where worker_nid = :worker_nid ";
      $sql .= "and year = :work_year ";
      $sql .= "and month = :work_month ";
      $stmt = sirius_sql_query($sql, array(':worker_nid' => $worker_nid, ':work_year' => $work_year, ':work_month' => $work_month));
      $hr = $stmt->fetchAssoc();
      $total_hours = round($hr['c'], 2);

      $row['HOURS'] = $employer_hours;
      $row['TOTAL HOURS'] = $total_hours;
      if ($total_hours) {
        $row['MULTIPLIER'] = round($employer_hours / $total_hours, 2);
      } else {
        $row['MULTIPLIER'] = 0;
      }

      if ($row['ADD'] && !$row['DENTAL'] && !$row['MEDICAL']) {
        $row['NON ENROLLED'] = 1;
      }

      if ($row['ADD'] && $row['DENTAL'] && !$row['MEDICAL']) {
        $row['NON ENR NO DDS'] = 1;
      }


      if ($row['HOURS'] && !$row['ADD']) {
        $row['NON ELIG'] = 1;
        $row['NON ELIG HRS'] = $row['HOURS'];
      } else {
        $row['EL HRS'] = $row['HOURS'];
      }

      $rows[$pk] = $row;
    }

     return array('is_multiple' => TRUE, 'rows' => $rows);
  }

  public function render_by_employer($report_id, $output = 'page') {
    $info = $this->info();
    $parameters = $this->parameters($report_id);
    $report_type = $this->type;


    $summary_fields = array();
    // $summary_fields[] = 'WORK MONTH';
    $summary_fields[] = 'HN';
    $summary_fields[] = 'HN T1';
    $summary_fields[] = 'HN T2';
    $summary_fields[] = 'HN T3';
    $summary_fields[] = 'KP';
    $summary_fields[] = 'KP T1';
    $summary_fields[] = 'KP T2';
    $summary_fields[] = 'KP T3';
    $summary_fields[] = 'MLK';
    $summary_fields[] = 'MLK T1';
    $summary_fields[] = 'MLK T2';
    $summary_fields[] = 'MLK T3';
    $summary_fields[] = 'VSP';
    $summary_fields[] = 'VSP T1';
    $summary_fields[] = 'VSP T2';
    $summary_fields[] = 'VSP T3';
    $summary_fields[] = 'DD';
    $summary_fields[] = 'UC-Liberty';
    $summary_fields[] = 'LADC';
    $summary_fields[] = 'NON ENROLLED';
    $summary_fields[] = 'HOURS';
    $summary_fields[] = 'NON ELIG';
    $summary_fields[] = 'NON ELIG HRS';
    $summary_fields[] = 'EL HRS';
    $summary_fields[] = 'NON ENR NO DDS';
    $summary_fields[] = 'MEDICAL';
    $summary_fields[] = 'DENTAL';
    $summary_fields[] = 'LIFE';

    $query = db_select('report_cache');
    $query->fields('report_cache');
    $query->condition('report_id', $report_id);
    $query->condition('report_type', $report_type);
    $stmt = $query->execute();
    $data = array();

    while ($hr = $stmt->fetchAssoc()) {
      $row = $this->row_decode($hr['report_data']);
      $multiplier = $row['MULTIPLIER'];
      if (!$multiplier) { continue; }
      $employer_name = $row['employer_name'];
      if (!$employer_name) { continue; }

      if (!$data[$employer_name]) { $data[$employer_name] = array('employer_id' => $row['employer_id']); }

      foreach ($summary_fields as $field_name) {
        $val = $row[$field_name];
        if (!$val) { $val = 0; }

        if (!in_array($field_name, array('HOURS', 'NON ELIG HRS', 'EL HRS'))) {
          $val = $multiplier * $val;
        }
        $data[$employer_name][$field_name] += $val;
      }

      // One month at a time
      $month = $row['WORK MONTH'];
    }

    ksort($data);

    $rows = array();
    foreach ($data as $employer_name => $employer_data) {
      $row = array();
      $row[] = $employer_name;
      $row[] = $employer_data['employer_id'];
      $row[] = $month;
      foreach ($summary_fields as $field_name) {
        $val = $employer_data[$field_name];
        if (!$val) { $val = 0; }
        $row[] = round($val, 2);
      }
      $rows[] = $row;
    }

    $header = array();
    $header[] = 'Employer';
    $header[] = 'Employer ID';
    $header[] = 'WORK MONTH';
    foreach ($summary_fields as $field_name) { $header[] = $field_name; }

    return array('header' => $header, 'rows' => $rows);
  }}
