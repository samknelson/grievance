<?php

/**
 * First appearance of hours at an employer (the "triggering" employer)
 *  - Name
 *  - ID
 *  - NID
 *  - Phone
 *  - Address
 *  - Email
 *  - Current employer (from the Worker record)
 *  - Current work status
 *  - Current memeber status
 *  - Triggering employer
 *  - Total hours within the range
 *  - Total hours at triggering employer
*/

class Sirius_Trust_Report_Newhire extends Report_Cache_Report_Generator {
  public $type = 'sirius-trust-newhire';

  public function info($result = array()) {
    $result = array(
      'name' => t('Hours - New Hires'),
      'access' => 'sirius trust staff',
      'render_callbacks' => array(
        'Summary' => array(
          'method' => 'render_summary',
          'csv' => TRUE,
        ),
      ),
      'render_parameters_callback' => 'sirius_trust_report_newhire_render_parameters',
      'module' => 'sirius_trust',
      'file' => 'sirius_trust.report_newhire.inc',
      'dashboard' => TRUE,
      'preview' => array(
        'default_display_fields' => array(
          'worker_name',
          'triggering_employer_name',
          'hours_total',
          'hours_at_tirggering_employer',
        ),
      ),
    );

    return parent::info($result);
  }

  public function form() {
    $form['start_date'] = array(
      '#title' => t('Start Date'),
      '#type' => 'date_popup',
      '#attributes' => array(
        'autocomplete' => 'off',
      ),
      '#required' => TRUE,
      '#date_format' => 'Y-m-d',
      '#default_value' => date('Y-m-', strtotime('-1 month')) . '01',
    );

    $form['end_date'] = array(
      '#title' => t('End Date'),
      '#type' => 'date_popup',
      '#attributes' => array(
        'autocomplete' => 'off',
      ),
      '#required' => TRUE,
      '#date_format' => 'Y-m-d',
      '#default_value' => date('Y-m-t', strtotime('-1 month')),
    );

    $ms_options = sirius_taxonomy_load('sirius_member_status', 'tid', 'name');
    $ws_options = sirius_taxonomy_load('sirius_work_status', 'tid', 'name');

    $form['limits'] = array(
      '#type' => 'fieldset',
      '#title' => t('Limits'),
      '#description' => '<p>' . t('Only check workers who match the following criteria.') . '</p>' . 
        '<p><em>' . t('Note: These limits apply only to the workers\' <strong>current</strong> status.') . '</em></p>',
    );

    $form['limits']['ms'] = array(
      '#type' => 'checkboxes',
      '#options' => $ms_options,
      '#title' => t('Member Status'),
      '#checkall' => TRUE,
    );

    $form['limits']['ws'] = array(
      '#type' => 'checkboxes',
      '#options' => $ws_options,
      '#title' => t('Work Status'),
      '#checkall' => TRUE,
    );

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

    $fields['triggering_employer_nid'] = 'Hiring Employer NID';
    $fields['triggering_employer_name'] = 'Hiring Employer Name';
    $fields['hours_total'] = 'Total Hours';
    $fields['hours_at_tirggering_employer'] = 'Hours at Hiring Employer';

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

    $start_date_format = date('Ymd', strtotime($form_state['values']['parameters']['start_date']));
    $end_date_format = date('Ymd', strtotime($form_state['values']['parameters']['end_date']));

    $sql = "select distinct worker_nid, employer_nid, concat(year, lpad(month, 2, 0), lpad(day, 2, 0)) as datestr from node ";
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
    $sql .= "order by worker_nid desc, employer_nid ";
    // $sql .= "limit 10 ";

    $stmt = sirius_sql_query($sql, $sql_args);
    $pks = array();
    while ($hr = $stmt->fetchAssoc()) {
      $pks[] = $hr['worker_nid'] . '|' . $hr['employer_nid'];
    }
    return array_chunk($pks, 100, TRUE);
  }

  public function batch($pks, $values, &$context) {
    $start_date_format = date('Ymd', strtotime($values['parameters']['start_date']));
    $end_date_format = date('Ymd', strtotime($values['parameters']['end_date']));

    $rows = array('is_multiple' => TRUE, 'rows' => array());
    foreach ($pks as $pk) {
      list ($worker_nid, $triggering_employer_nid) = explode('|', $pk);

      // Skip this worker if they had hours before the start date
      $sql = "select count(*) as c ";
      $sql .= " from sirius_hours_cache ";
      $sql .= "where worker_nid = :worker_nid ";
      $sql .= "and employer_nid = :triggering_employer_nid ";
      $sql .= "and concat(year, lpad(month, 2, 0), lpad(day, 2, 0)) < :start_date_format ";
      $sql_args = array();
      $sql_args[':worker_nid'] = $worker_nid;
      $sql_args[':triggering_employer_nid'] = $triggering_employer_nid;
      $sql_args[':start_date_format'] = $start_date_format;
      $stmt = sirius_sql_query($sql, $sql_args);
      $hr = $stmt->fetchAssoc();
      if ($hr['c'] > 0) { continue; }

      // Total hours in range
      $sql = "select sum(hours) as c ";
      $sql .= " from sirius_hours_cache ";
      $sql .= "where worker_nid = :worker_nid ";
      $sql .= "and concat(year, lpad(month, 2, 0), lpad(day, 2, 0)) >= :start_date_format ";
      $sql .= "and concat(year, lpad(month, 2, 0), lpad(day, 2, 0)) <= :end_date_format ";
      $sql_args = array();
      $sql_args[':worker_nid'] = $worker_nid;
      $sql_args[':start_date_format'] = $start_date_format;
      $sql_args[':end_date_format'] = $end_date_format;
      $stmt = sirius_sql_query($sql, $sql_args);
      $hr = $stmt->fetchAssoc();
      $hours = $hr['c'];

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
    $html .= sirius_fakerender_field('Start Date:', $parameters['start_date']);
    $html .= sirius_fakerender_field('End Date:', $parameters['end_date']);
    return $html;
  }

  public function render_summary($report_id, $output = 'page') {
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
      $data[$hr['triggering_employer_name']]['total']++;
      $totals['total']++;
    }
    ksort($data);

    $header = array();
    $header[] = t('Employer');
    $header[] = t('New Hires');

    $rows = array();
    foreach ($data as $employer_name => $record) {
      $row = array();
      $row[] = $employer_name;
      $row[] = sirius_number_format($record['total'], 0, '.', ',', $output);
      $rows[] = $row;
    }

    $row = array();
    $row['class'] = array('report_cache_total');
    $row['data'] = array();
    $row['data'][] = t('Total');
    $row['data'][] = sirius_number_format($totals['total'], 0, '.', ',', $output);
    $rows[] = $row;
    return array('header' => $header, 'rows' => $rows);
  }
}

function sirius_trust_report_newhire_view_form($form, &$form_state) {
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
