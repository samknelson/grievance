<?php

class Sirius_Teamsters631_Report_Edls_Compliance extends Report_Cache_Report_Generator {
  public $type = 'sirius-teamsters631-edls-compliance';

  public function info($result = array()) {
    $result = array(
      'name' => t('Teamsters631 - EDLS - Compliance'),
      'access' => 'sirius admin',
      'render_callbacks' => array(
        'Summary' => array(
          'method' => 'render_summary',
          'csv' => TRUE,
        ),
      ),
      // 'render_parameters_callback' => 'sirius_trust_report_newhire_render_parameters',
      'module' => 'sirius_teamsters631',
      'file' => 'sirius_teamsters631.report_edls_compliance.inc',
      'dashboard' => TRUE,
      'preview' => array(
        'default_display_fields' => array(
          'worker_id',
          'worker_name',
          'worker_ms_name',
          'violation_name',
          'violation_details',
        ),
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

    $form['date'] = array(
      '#title' => t('Date'),
      '#type' => 'date_popup',
      '#attributes' => array(
        'autocomplete' => 'off',
      ),
      '#required' => TRUE,
      '#date_format' => 'Y-m-d',
      '#default_value' => date('Y-m-d', strtotime('tomorrow')),
    );

    $form['status_options'] = array(
      '#title' => t('Status Options'),
      '#type' => 'checkboxes',
      '#required' => TRUE,
      '#options' => sirius_edls_sheet_status_options(),
      '#checkall' => TRUE,
      '#default_value' => array('request', 'lock',),
    );

    return $form;
  }

  public function fields() {
    $fields = array();
    $fields['worker_nid'] = 'Worker NID';
    $fields['worker_id'] = 'Worker ID';
    $fields['worker_name'] = 'Worker Name';
    $fields['worker_ms'] = 'Member Status ID';
    $fields['worker_ms_name'] = 'Member Status';
    $fields['violation'] = 'Violation';
    $fields['violation_name'] = 'Violation Name';
    $fields['violation_details'] = 'Details';
    $fields['sheet_nid'] = 'Sheet NID';

    return $fields;
  }

  public function preview_links($row) {
    return array(
      'node/' . $row['worker_nid'] => 'Worker',
      'node/' . $row['worker_nid'] . '/sirius_worker_hours' => 'Hours',
    );
  }

  public function pks(&$form_state) {
    $workers = sirius_edls_worker_list(
      $form_state['values']['parameters']['employer'], 
      $form_state['values']['parameters']['date']
    );

    $pks = array();
    foreach ($workers as $delta => $worker) {
      $pks[] = $worker['worker_id'];
    }
    return array_chunk($pks, 100, TRUE);
  }

  public function violation_scans() {
    $violation_scans = array();
    $violation_scans['journeyman_show'] = array(
      'name' => 'Journeyman with Lower Show Priority',
      'method' => 'violation_check_journeyman_show',
    );

    $violation_scans['extraboard'] = array(
      'name' => 'Extraboard Before Journeyman',
      'method' => 'violation_check_extraboard',
    );
    return $violation_scans;
  }

  public function violation_check($scan_key, $employer_nid, $worker, $date) {
    $scan_info = $this->violation_scans()[$scan_key];
    if (!$scan_info) { return array(); }
    return $this->{$scan_info['method']}($employer_nid, $worker, $date);
  }

  public function batch($pks, $values, &$context) {
    $employer_nid = $values['parameters']['employer'];
    $employer_node = node_load($employer_nid);
    $date = $values['parameters']['date'];
    $ms_lookup = sirius_edls_ms_list($employer_node);
    $rows = array('is_multiple' => TRUE, 'rows' => array());

    $violation_scans = $this->violation_scans();
    foreach ($pks as $worker_id) {
      $worker = sirius_edls_worker_fetch($employer_nid, $worker_id, $date);
      if (!$worker) { continue; }
      $worker['worker_ms_name'] = $ms_lookup[$worker['worker_ms']];

      foreach ($violation_scans as $scan_key => $scan_info) {
        $violation = $this->violation_check($scan_key, $employer_nid, $worker, $date);
        if (!$violation) { continue; }

        $context['message'] = 'Running compliance scan for: ' . $worker['worker_name'];
        $row = array();
        $row['pk'] = "$worker_id|$scan_key";
        $row['worker_id'] = $worker['worker_id'];
        $row['worker_ms'] = $worker['worker_ms'];
        $row['worker_ms_name'] = $worker['worker_ms_name'];
        $row['worker_name'] = $worker['worker_name'];
        $row['violation'] = $scan_key;
        $row['violation_name'] = $scan_info['name'];
        $row['violation_details'] = $violation['msg'];
        $row['sheet_nid'] = $violation['sheet_nid'];
        $rows['rows']["$worker_id|$scan_key"] = $row;
      }
    }

    return $rows;
  }

  public function violation_check_journeyman_show($employer_nid, $worker, $date) {
    $worker_id = $worker['worker_id'];

    // Only journeymen can have violations. This is a kludge.
    if (!preg_match('/journey/i', $worker['worker_ms_name'])) { return array(); }

    // Scheduled workers have no violations
    $assignment = sirius_edls_assignment_fetch_worker($date, $employer_nid, $worker_id);
    if ($assignment) { return array(); }

    // Outsource workers have no violations
    if (!preg_match('/^I/', $worker_id)) { return array(); }
    $real_worker_id = preg_replace('/^I/', '', $worker_id);
    $worker_node = sirius_worker_get_by_id($real_worker_id);
    if (!$worker_node) { return array(); }
    $worker_nid = $worker_node->nid;

    // Find their dispatch
    $params = array();
    $params['date'] = $date;
    $params['worker_nid'] = $worker_nid;
    $params['employer_nid'] = $employer_nid;
    $params['statuses'] = array('accepted', 'accepted_secondary');
    $dispatches = sirius_dispatch_search($params);
    $dispatch = reset($dispatches);
    if (!$dispatch) { return array(); }

    // Find the job
    $job_node = node_load($dispatch->field_sirius_dispatch_job['und'][0]['target_id']);
    if (!$job_node) { return array(); }

    // Find the job group
    $job_group_node = node_load($job_node->field_sirius_dispatch_job_group['und'][0]['target_id']);
    if (!$job_group_node) { return array(); }

    // Only if the job group is current
    $ts = strtotime($date);
    $job_group_start_ts = sirius_fieldval($job_group_node, 'field_sirius_datetime', 'ts');
    $job_group_end_ts = sirius_fieldval($job_group_node, 'field_sirius_datetime_completed', 'ts');
    if ($job_group_start_ts >= $ts || $job_group_end_ts <= $ts) { return array(); }

    // Get the seniority list
    $seniority_list = $this->seniority_list($date, $employer_nid, $job_group_node->nid);

    // Scan the seniority list for people who are scheduled and are less senior than me.
    $my_seniority_date = NULL;
    foreach ($seniority_list as $seniority_date => $probe_worker_id) {
      if ($probe_worker_id == $real_worker_id) {
        $my_seniority_date = $seniority_date;
        continue;
      }

      if (!$my_seniority_date) { continue; }
      if ($my_seniority_date == $seniority_date) { continue; }

      $assignment = sirius_edls_assignment_fetch_worker($date, $employer_nid, 'I' . $probe_worker_id);
      if (!$assignment) { continue; }

      $msg = "For show " . $job_group_node->title . " ";
      $msg .= "this worker's seniority is $my_seniority_date GMT. At least one worker with less seniority was scheduled: ";
      $msg .= $assignment['worker_name'];
      $msg .= " (";
      $msg .= $assignment['worker_id'];
      $msg .= ") ";
      $msg .= "with seniority as of $seniority_date GMT.";
      return array('msg' => $msg, 'sheet_nid' => $assignment['sheet_nid']);
    }

    return array();
  }

  public function violation_check_extraboard($employer_nid, $worker, $date) {
    $worker_id = $worker['worker_id'];

    // Only journeymen can have violations. This is a kludge.
    if (!preg_match('/journey/i', $worker['worker_ms_name'])) { return array(); }

    // Scheduled workers have no violations
    $assignment = sirius_edls_assignment_fetch_worker($date, $employer_nid, $worker_id);
    if ($assignment) { return array(); }

    // Outsource workers have no violations
    if (!preg_match('/^I/', $worker_id)) { return array(); }
    $real_worker_id = preg_replace('/^I/', '', $worker_id);
    $worker_node = sirius_worker_get_by_id($real_worker_id);
    if (!$worker_node) { return array(); }
    $worker_nid = $worker_node->nid;

    // Find out if any extraboards are scheduled today. (Cache for speed)
    $cache = &drupal_static(__FUNCTION__);
    if (!isset($cache[$employer_nid][$date])) {
      // Find "Extraboard" member statuses
      $employer_node = node_load($employer_nid);
      $ms_list = sirius_edls_ms_list($employer_node);
      $extraboard_ms_list = array();
      foreach ($ms_list as $ms_key => $ms_name) {
        if (preg_match('/extra/i', $ms_name)) { $extraboard_ms_list[$ms_key] = $ms_name; }
      }
      if (!$extraboard_ms_list) { return array(); }

      $params = array();
      $params['date'] = $date;
      $params['employer_nid'] = $employer_nid;
      $params['statuses'] = array('request', 'lock');
      $params['mss'] = array_keys($extraboard_ms_list);
      $extraboard_assignments = sirius_edls_assignment_search($params);
      $cache[$employer_nid][$date] = reset($extraboard_assignments);
    }

    $assignment = $cache[$employer_nid][$date];
    if (!$assignment) { return array(); }

    $msg = "At least one extraboard was scheduled: ";
    $msg .= $assignment['worker_name'];
    $msg .= " (";
    $msg .= $assignment['worker_id'];
    $msg .= ") ";
    return array('msg' => $msg, 'sheet_nid' => $assignment['sheet_nid']);
  }

  public function seniority_list($date, $employer_nid, $job_group_nid) {
    $cache = &drupal_static(__FUNCTION__);
    $cache_key = "$date|$employer_nid|$job_group_nid";

    if (!isset($cache[$cache_key])) {
      // Find the list of dispatched workers
      $params = array();
      $params['date'] = $date;
      $params['employer_nid'] = $employer_nid;
      $params['statuses'] = array('accepted', 'accepted_secondary');
      $params['job_group_nid'] = $job_group_nid;
      $params['nids_only'] = TRUE;
      $dispatch_nids = sirius_dispatch_search($params);

      // Generate the seniority list
      $seniority_list = array();
      foreach ($dispatch_nids as $dispatch_nid) {
        $sql = "select worker_node.title as worker_name, ";
        $sql .= "field_sirius_id_value as worker_id, ";
        $sql .= "field_sirius_datetime_value as start_date ";
        $sql .= "from node ";
        $sql .= "left join field_data_field_sirius_dispatch_job on field_data_field_sirius_dispatch_job.entity_type = 'node' and field_data_field_sirius_dispatch_job.entity_id = node.nid ";
        $sql .= "left join field_data_field_sirius_worker on field_data_field_sirius_worker.entity_type = 'node' and field_data_field_sirius_worker.entity_id = node.nid ";

        $sql .= "left join node job_node on job_node.nid = field_sirius_dispatch_job_target_id ";
        $sql .= "left join field_data_field_sirius_datetime on field_data_field_sirius_datetime.entity_type = 'node' and field_data_field_sirius_datetime.entity_id = job_node.nid ";

        $sql .= "left join node worker_node on worker_node.nid = field_sirius_worker_target_id ";
        $sql .= "left join field_data_field_sirius_id on field_data_field_sirius_id.entity_type = 'node' and field_data_field_sirius_id.entity_id = worker_node.nid ";

        $sql .= "where node.nid = :dispatch_nid";

        $stmt = sirius_sql_query($sql, array(':dispatch_nid' => $dispatch_nid));
        $hr = $stmt->fetchAssoc();
        $seniority_list[$hr['start_date']] = $hr['worker_id'];
      }
      ksort($seniority_list);
      $cache[$cache_key] = $seniority_list;
    }

    return $cache[$cache_key];
  }

  public function render_parameters($parameters, $options = array()) {
    $parameters = $parameters['parameters'];

    if ($parameters['employer'] && !$options['skip_employer']) {
      $html .= sirius_fakerender_field('Employer:', sirius_node_title($parameters['employer']));
    }

    if ($parameters['date'] && !$options['skip_date']) {
      $html .= sirius_fakerender_field('Date:', date('l, F j, Y', strtotime($parameters['date'])));
    }

    if ($parameters['status_options'] && !$options['skip_status_options']) {
      $name_lookup = sirius_edls_sheet_status_options();

      $values = array_filter(array_values($parameters['status_options']));
      $names = array();
      foreach ($values as $delta => $value) { $names[] = $name_lookup[$value]; }
      $html .= sirius_fakerender_field('Statuses:', join(', ', $names));
    }
    return $html;
  }

  public function render($report_id, $output = 'page') {
    $info = $this->info();

    $query = db_select('report_cache');
    $query->fields('report_cache');
    $query->condition('report_id', $report_id);
    $query->condition('report_type', $this->type);
    $stmt = $query->execute();

    $header = array();
    $header[] = t('ID');
    $header[] = t('Name');
    $header[] = t('Member Status');
    $header[] = t('Potential Violations');
    $header[] = t('Details');
    if ($output != 'csv') { $header[] = t('Tools'); }

    $rows = array();
    while ($hr = $stmt->fetchAssoc()) {
      $record = unserialize($hr['report_data']);

      $row = array();
      $row[] = $record['worker_id'];
      $row[] = $record['worker_name'];
      $row[] = $record['worker_ms_name'];
      $row[] = $record['violation_name'];
      $row[] = $record['violation_details'];

      if ($output != 'csv') {
        $tools = array();
        if ($record['sheet_nid']) {
          $tools[] = l("View Sheet", "node/" . $record['sheet_nid'] . "/sirius_edls_sheet_view", array('attributes' => array('class' => array('tool-button'))));
        }
        $row[] = join('', $tools);
      }

      $rows[] = $row;
    }

    if (!$rows) {
      return '<p>' . t('No compliance violations were found.') . '</p>';
    }

    return array('header' => $header, 'rows' => $rows);
  }
}
