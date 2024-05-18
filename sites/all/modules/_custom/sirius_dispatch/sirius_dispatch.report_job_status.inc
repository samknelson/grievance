<?php

class Sirius_Dispatch_Report_Job_Status extends Report_Cache_Report_Generator {
  public $type = 'sirius-dispatch-job-status';

  public function info($result = array()) {
    $result = array(
      'name' => t('Job Status'),
      'access' => 'sirius dispatch staff',
      'render_callbacks' => array(
        'View' => array(
          'method' => 'render',
          'csv' => TRUE,
        ),
      ),
      'module' => 'dispatch',
      'file' => 'sirius_dispatch.report_job_status.inc',
      'dashboard' => FALSE,
      'preview' => array(
        'default_display_fields' => array(
          'job_title',
          'employer_title',
          'job_status',
          'job_type_name',
          'worker_count',
          'worker_count_yes',
          'worker_count_no',
        ),
        'preview_links' => TRUE,
      ),
    );
    return parent::info($result);
  }

  public function fields() {
    return array(
      'job_nid' => 'Job NID',
      'job_title' => 'Job',
      'job_status' => 'Status',
      'job_type_tid' => 'Job Type TID',
      'job_type_name' => 'Job Type',
      'employer_nid' => 'Employer NID',
      'employer_title' => 'Employer',
      'worker_count' => 'Requested',
      'worker_count_yes' => 'Accepted',
      'worker_count_no' => 'Declined',
    );
  }

  public function form() {
    $form['from_date'] = array(
      '#type' => 'date_popup',
      '#title' => t('Start date'),
      '#required' => TRUE,
      '#date_format' => 'm/d/Y',
      '#description' => t('The report date must be after this date.'),
    );

    $form['to_date'] = array(
      '#type' => 'date_popup',
      '#title' => t('End Date'),
      '#required' => TRUE,
      '#date_format' => 'm/d/Y',
      '#description' => t('The report date must be before this date.'),
    );

    return $form;
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
    $from_ts = strtotime($form_state['values']['parameters']['from_date']);
    $to_ts = strtotime($form_state['values']['parameters']['to_date']);
    $from_date = date('Y-m-d 00:00:00', $from_ts);
    $to_date = date('Y-m-d 00:00:00', strtotime('+1 day', $to_ts));

    $domain_nid = sirius_domain_nid();
    $query = new EntityFieldQuery;
    $query->entityCondition('entity_type', 'node');
    $query->entityCondition('bundle', 'sirius_dispatch_job');
    $query->fieldCondition('field_sirius_datetime', 'value', $from_date, '>=');
    $query->fieldCondition('field_sirius_datetime', 'value', $to_date, '<');
    if ($domain_nid) { $query->fieldCondition('field_sirius_domain', 'target_id', $domain_nid); }
    $stmt = $query->execute();
    $nids = array();
    if ($stmt['node']) {
      $nids = array_keys($stmt['node']);
    }
  	return array_chunk($nids, 100, TRUE);
  }

  public function preview_links($row) {
    return array(
      'node/' . $row['job_nid'] => 'Job',
    );
  }

  public function batch($job_nids, $values, &$context) {
  	$rows = array(
  		'is_multiple' => TRUE,
  		'rows' => array(),
  	);

  	foreach ($job_nids as $job_nid) {
  		$row = $this->batch_record($job_nid, $lookups, $values);
  		if ($row) {
  			$rows['rows'][$job_nid] = $row;
  		}
  	}
  	return $rows;
  }

  function batch_record($job_nid, $lookups, $values) {
    $parameters = $values['parameters'];

    // Load the job
    $job_node = node_load($job_nid);

    // Our result row
    $row = array();
    $row['job_nid'] = $job_nid;
    $row['job_title'] = $job_node->title;
    $row['job_status'] = $job_node->field_sirius_dispatch_job_status['und'][0]['value'];
    $row['job_type_tid'] = $job_node->field_sirius_dispatch_job_type['und'][0]['tid'];
    $row['job_type_name'] = sirius_term_title($row['job_type_tid']);

    $row['employer_nid'] = $job_node->field_grievance_shop['und'][0]['target_id'];
    $row['employer_title'] = sirius_node_title($row['employer_nid']);

    $row['worker_count'] = $job_node->field_sirius_count['und'][0]['value'];
    $row['worker_count_yes'] = $job_node->field_sirius_count_yes['und'][0]['value'];
    $row['worker_count_no'] = $job_node->field_sirius_count_no['und'][0]['value'];

    return $row;
  }

  public function render($report_id, $output = 'page') {
    $query = db_select('report_cache');
    $query->fields('report_cache');
    $query->condition('report_id', $report_id);
    $query->condition('report_type', $this->type);
    $stmt = $query->execute();
    $data = array();
    $grand_total = 0;
    while ($hr = $stmt->fetchAssoc()) {
      $hr = unserialize($hr['report_data']);

      $employer_title = $hr['employer_title'];
      $status = $hr['job_status'];
      if (!in_array($status, array('Filled', 'Unfilled'))) { continue; }

      $data[$employer_title]['worker_count'] += $hr['worker_count'];
      $data[$employer_title]['worker_count_yes'] += $hr['worker_count_yes'];
      $data[$employer_title][$status]++;
    }

    ksort($data);

    $header = array();
    $header[] = t('Shop');
    $header[] = t('Workers Requested');
    $header[] = t('Workers Provided');
    $header[] = t('Short');
    $header[] = t('Number of Filled Jobs');
    $header[] = t('Number of Unfilled Jobs');

    $rows = array();
    $totals = array(
      'worker_count' => 0,
      'worker_count_yes' => 0,
      'filled_count' => 0,
      'unfilled_count' => 0,
    );
    foreach ($data as $employer_title => $record) {
      $row = array();
      $worker_count = $record['worker_count'];
      if (!$worker_count) { $worker_count = 0; }
      $worker_count_yes = $record['worker_count_yes'];
      if (!$worker_count_yes) { $worker_count_yes = 0; }

      $filled_count = $record['Filled'];
      if (!$filled_count) { $filled_count = 0; }
      $unfilled_count = $record['Unfilled'];
      if (!$unfilled_count) { $unfilled_count = 0; }

      $totals['worker_count'] += $worker_count;
      $totals['worker_count_yes'] += $worker_count_yes;
      $totals['filled_count'] += $filled_count;
      $totals['unfilled_count'] += $unfilled_count;

      $row[] = $employer_title;
      $row[] = $worker_count;
      $row[] = $worker_count_yes;
      $row[] = $worker_count - $worker_count_yes;
      $row[] = $filled_count;
      $row[] = $unfilled_count;

      $rows[] = $row;
    }

    $row = array();
    $row[] = '<strong>' . t('Total') . '</strong>';
    $row[] = '<strong>' . $totals['worker_count'] . '</strong>';
    $row[] = '<strong>' . $totals['worker_count_yes'] . '</strong>';
    $row[] = '<strong>' . ($totals['worker_count'] - $totals['worker_count_yes']) . '</strong>';
    $row[] = '<strong>' . $totals['filled_count'] . '</strong>';
    $row[] = '<strong>' . $totals['unfilled_count'] . '</strong>';
    $rows[] = $row;

    return array('header' => $header, 'rows' => $rows);
  }
}


