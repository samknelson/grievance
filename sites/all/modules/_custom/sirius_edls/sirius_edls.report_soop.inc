<?php

class Sirius_Edls_Report_Soop extends Report_Cache_Report_Generator {
  public $type = 'sirius-edls-soop';

  public function info($result = array()) {
    $result = array(
      'name' => t('EDLS - Scheduled Out-of-Population'),
      'access' => 'sirius admin',
      'render_callbacks' => array(
        'Summary' => array(
          'method' => 'render_summary',
          'csv' => TRUE,
        ),
      ),
      // 'render_parameters_callback' => 'sirius_trust_report_newhire_render_parameters',
      'module' => 'sirius_edls',
      'file' => 'sirius_edls.report_soop.inc',
      'dashboard' => TRUE,
      'preview' => array(
        /*
        'default_display_fields' => array(
          'worker_id',
          'worker_name',
          'worker_ms_name',
          'violation_name',
          'violation_details',
        ),
        */
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

    return $form;
  }

  public function fields() {
    $fields = array();
    $fields['sheet_nid'] = 'Sheet NID';
    $fields['sheet_job'] = 'Job Number';
    $fields['sheet_date'] = 'Date';
    $fields['worker_id'] = 'Worker ID';
    $fields['worker_ms'] = 'Worker MS';
    $fields['worker_name'] = 'Worker Name';

    return $fields;
  }

  public function preview_links($row) {
    return array(
      'node/' . $row['sheet_nid'] . '/sirius_edls_sheet_view' => 'Sheet',
    );
  }

  public function pks(&$form_state) {
    $search_params = array();
    $search_params['employer_nid'] = $form_state['values']['parameters']['employer'];
    $search_params['date_min'] = date('Y-m-d');
    $search_params['nids_only'] = TRUE;
    $sheet_nids = sirius_edls_sheet_search($search_params);
    return array_chunk($sheet_nids, 10);
  }

  public function batch($sheet_nids, $values, &$context) {
    $employer_nid = $values['parameters']['employer'];

    $worker_list = sirius_edls_worker_list($employer_nid, date('Y-m-d'), array('skip_assignment' => TRUE));
    $population_map = array();
    foreach ($worker_list as $delta => $worker) { $population_map[$worker['worker_id']] = $worker; }

    $rows = array('is_multiple' => TRUE, 'rows' => array());
    foreach ($sheet_nids as $sheet_nid) {
      $sql = "select * from sirius_edls_assignments where sheet_nid = :sheet_nid";
      $stmt = sirius_sql_query($sql, array(':sheet_nid' => $sheet_nid));
      $assignments = array();
      while ($hr = $stmt->fetchAssoc()) { $assignments[] = $hr; }

      $sheet_node = NULL;
      foreach ($assignments as $assignment) {
        if ($population_map[$assignment['worker_id']]) { continue; }

        if (!$sheet_node) { $sheet_node = node_load($sheet_nid); }

        $row = array();
        $row['pk'] = $assignment['assignment_id'];
        $row['worker_id'] = $assignment['worker_id'];
        $row['worker_name'] = $assignment['worker_name'];
        $row['worker_ms'] = $assignment['worker_ms'];
        $row['sheet_nid'] = $sheet_nid;
        $row['sheet_job'] = $sheet_node->field_sirius_job_number['und'][0]['value'];
        $row['sheet_date'] = date('Y-m-d', strtotime($sheet_node->field_sirius_date_start['und'][0]['value']));
        $rows['rows'][$assignment['assignment_id']] = $row;
      }
    }

    return $rows;
  }

  public function render_parameters($parameters, $options = array()) {
    $parameters = $parameters['parameters'];
    if ($parameters['employer'] && !$options['skip_employer']) {
      $html .= sirius_fakerender_field('Employer:', sirius_node_title($parameters['employer']));
    }
    return $html;
  }
}
