<?php

class Sirius_Edls_Report_Sts extends Report_Cache_Report_Generator {
  public $type = 'sirius-edls-sts';

  public function info($result = array()) {
    $result = array(
      'name' => t('EDLS - Scheduled Too Soon'),
      'access' => 'sirius admin',
      'render_callbacks' => array(
        'Summary' => array(
          'method' => 'render_summary',
          'csv' => TRUE,
        ),
      ),
      // 'render_parameters_callback' => 'sirius_trust_report_newhire_render_parameters',
      'module' => 'sirius_edls',
      'file' => 'sirius_edls.report_sts.inc',
      'dashboard' => TRUE,
      'preview' => array(
        'default_display_fields' => array(
          'worker_id',
          'worker_name',
          'worker_ms_name',
          'interval_render',
        ),
        'preview_links' => TRUE,
      ),
    );

    return parent::info($result);
  }

  public function form() {
    $form['employer'] = array(
      '#title' => t('Employer'),
      '#type' => 'select',
      '#required' => TRUE,
      '#options' => sirius_employer_list(),
    );

    $form['min_interval_hrs'] = array(
      '#title' => t('Minimum Hours Between Shift Start Times'),
      '#type' => 'textfield',
      '#required' => TRUE,
      '#description' => t('Enter the minimum number of hours between shift start times. The system only looks at start times, not end times.'),
      // '#default_value' => 12,
      '#size' => 4,
    );

    $form['start_date'] = array(
      '#title' => t('Start Date'),
      '#type' => 'date_popup',
      '#required' => TRUE,
      '#description' => t('The report will search 10 days from the date provided.'),
      '#default_value' => date('Y-m-d'),
      '#attributes' => array(
        'autocomplete' => 'off',
      ),
      '#date_format' => 'Y-m-d',
    );

    return $form;
  }

  public function fields() {
    $fields = array();
    $fields['curr_sheet_nid'] = 'Sheet NID';
    $fields['curr_sheet_date'] = 'Date';
    $fields['prev_sheet_nid'] = 'Sheet NID';
    $fields['worker_id'] = 'Worker ID';
    $fields['worker_ms'] = 'Worker MS';
    $fields['worker_name'] = 'Worker Name';
    $fields['interval_seconds'] = 'Interval (in seconds)';
    $fields['interval_render'] = 'Interval';

    return $fields;
  }

  public function preview_links($row) {
    return array(
      'node/' . $row['curr_sheet_nid'] . '/sirius_edls_sheet_view' => 'Curr Sheet',
      'node/' . $row['prev_sheet_nid'] . '/sirius_edls_sheet_view' => 'Prev Sheet',
    );
  }

  public function pks(&$form_state) {
    $dates = array();
    $start_date = $form_state['values']['parameters']['start_date'];
    $ts = strtotime($start_date);
    for ($i = 0; $i < 10; ++$i) {
      $dates[] = date('Y-m-d', $ts);
      $ts = strtotime('+1 day', $ts);
    }
    return $dates;
  }

  public function start_ts($assignment) {
    $cache = &drupal_static(__FUNCTION__);
    if (!isset($cache)) { $cache = array(); }

    $crew_uuid = $assignment['crew_uuid'];
    if (!$crew_uuid) { return NULL; }

    if (!isset($cache[$crew_uuid]['start_ts'])) {
      $sheet_node = node_load($assignment['sheet_nid']);
      $crews = sirius_edls_crews($sheet_node, 'uuid');
      $date = date('Y-m-d', strtotime($sheet_node->field_sirius_date_start['und'][0]['value']));
      foreach ($crews as $crew_uuid_other => $crew) {
        $cache[$crew_uuid_other]['start_ts'] = strtotime($date . " " . $crew['start_time']);
        $cache[$crew_uuid_other]['start_date'] = $date;
      }
    }

    $extra_json = json_decode($assignment['assignment_extra'], TRUE);
    if ($extra_json['time']) {
      return strtotime($cache[$assignment['crew_uuid']]['start_date'] . ' ' . $extra_json['time']);
    } else {
      return $cache[$assignment['crew_uuid']]['start_ts'];
    }
  }

  public function batch($date, $values, &$context) {
    $employer_nid = $values['parameters']['employer'];
    $workers = sirius_edls_worker_list($employer_nid, $date, $filters = array('has_assignment_curr' => 'include'));
    $min_interval_hrs = $values['parameters']['min_interval_hrs'];
    if (! ($min_interval_hrs > 0)) { $min_interval_hrs = 12; }
    $min_interval = $min_interval_hrs * 60 * 60;

    $rows = array('is_multiple' => TRUE, 'rows' => array());

    foreach ($workers as $worker) {
      $worker_full = sirius_edls_worker_fetch($employer_nid, $worker['worker_id'], $date);
      $curr_ts = $this->start_ts($worker_full['assignments']['curr']);
      $prev_ts = $this->start_ts($worker_full['assignments']['prev']);

      if (!$curr_ts || !$prev_ts) { continue; }
      $interval = $curr_ts - $prev_ts;
      if ($interval > $min_interval) { continue; }

      $row = array();
      $row['pk'] = $worker_full['assignments']['curr']['assignment_id'];
      $row['worker_id'] = $worker_full['worker_id'];
      $row['worker_name'] = $worker_full['worker_name'];
      $row['worker_ms'] = $worker_full['worker_ms'];
      $row['curr_sheet_nid'] = $worker_full['assignments']['curr']['sheet_nid'];
      $row['prev_sheet_nid'] = $worker_full['assignments']['prev']['sheet_nid'];
      $row['sheet_date'] = $date;
      $row['interval_seconds'] = $interval;
      $row['interval_render'] = format_interval($interval, 2);
      $rows['rows'][$row['pk']] = $row;
    }

    return $rows;
  }

  public function render_parameters($parameters, $options = array()) {
    $parameters = $parameters['parameters'];
    if ($parameters['employer'] && !$options['skip_employer']) {
      $html .= sirius_fakerender_field('Employer:', sirius_node_title($parameters['employer']));
    }
    if ($parameters['min_interval_hrs']) {
      $html .= sirius_fakerender_field('Min Interval:', $parameters['min_interval_hrs']);
    }
    if ($parameters['start_date']) {
      $html .= sirius_fakerender_field('Start date:', $parameters['start_date']);
    }
    return $html;
  }
}
