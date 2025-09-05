<?php

class Sirius_Smf_Report_Emppop extends Report_Cache_Report_Generator {
  public $type = 'sirius-smf-employer-population';

  public function info($result = array()) {
    $result = array(
      'name' => t('Employer Population'),
      'description' => '',
      'access' => 'sirius trust staff',
      'preview' => array(
        // 'default_display_fields' => array(),
        'preview_links' => TRUE,
      ),
      'dashboard' => TRUE,
      'module' => 'sirius_smf',
      'file' => 'reports/emppop.inc',
      'row_encoding' => 'json',
    );

    return parent::info($result);
  }

  public function fields() {
    $fields = array();
    $fields['worker_nid'] = 'Worker NID';
    $fields['first_name'] = 'Frst Name';
    $fields['last_name'] = 'Last Name';
    $fields['life'] = 'Life';
    $fields['medical'] = 'Medical';
    $fields['dependent_count'] = 'Dependent Count';
    $fields['hours_0'] = 'Hours This Month';
    $fields['hours_1'] = 'Hours Prev 1';
    $fields['hours_2'] = 'Hours Prev 2';
    $fields['hours_3'] = 'Hours Prev 3';
    $fields['hours_4'] = 'Hours Prev 4';
    return $fields;
  }

  public function preview_links($row) {
    $links = array();
    if ($row['worker_nid']) { $links['node/' . $row['worker_nid']] = 'Worker'; }
    return $links;
  }

  public function form() {
    $form['employer_nid'] = array(
      '#type' => 'select',
      '#title' => t('Employer'),
      '#options' => array(NULL => t('-- Select --')) + sirius_employer_list(),
      '#required' => TRUE,
    );

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
    $date = $parameters['year'] . '-' . $parameters['month'] . '-15';
    $employer_nid = $parameters['employer_nid'];

    // Damn sirius_hours_cache doesn't let you query by date range, it's only got month and year.
    $y = $parameters['year'] + 0;
    $m = $parameters['month'] + 0;

    $worker_nids = array();
    for ($i=0; $i<5; ++$i) {
      $sql = "select distinct worker_nid from sirius_hours_cache ";
      $sql .= "where employer_nid = :employer_nid ";
      $sql .= "and year = :y ";
      $sql .= "and month = :m ";
      $stmt = sirius_sql_query($sql, array(':employer_nid' => $employer_nid, ':y' => $y, ':m' => $m));
      while ($hr = $stmt->fetchAssoc()) {
        $worker_nids[] = $hr['worker_nid'];
      }

      --$m;
      if ($m < 1) { $m = 12; --$y; }
    }
    $worker_nids = array_unique($worker_nids);
    return array_chunk($worker_nids, 10);
  }

  public function batch($worker_nids, $values, &$context) {
    $parameters = $values['parameters'];
    $date = $parameters['year'] . '-' . $parameters['month'] . '-15';

    $life_tid = sirius_find_tid_by_id('LIFE', 'sirius_trust_benefit_type');
    if (!$life_tid) {
      drupal_set_message("Please make sure that there is a benefit type with the ID = 'LIFE'.", 'error');
      return array();
    }

    $medical_tid = sirius_find_tid_by_id('MEDICAL', 'sirius_trust_benefit_type');
    if (!$medical_tid) {
      drupal_set_message("Please make sure that there is a benefit type with the ID = 'MEDICAL'.", 'error');
      return array();
    }

    $ts = strtotime($date);

    $rows = array();
    foreach ($worker_nids as $worker_nid) {
      $row = array();

      $worker_node = node_load($worker_nid);
      $row['worker_nid'] = $worker_nid;
      $row['last_name'] = $worker_node->field_sirius_name['und'][0]['family'];
      $row['first_name'] = $worker_node->field_sirius_name['und'][0]['given'];


      $wb_node = $benefit_node = NULL;
      $params = array();
      $params['worker_nid'] = $worker_nid;
      $params['ts'] = $ts;
      $params['benefit_type'] = $life_tid;
      $probe = sirius_trust_worker_benefits_search($params);
      $wb_node = reset($probe);
      if ($wb_node) {
        $benefit_node = node_load($wb_node->field_sirius_trust_benefit['und'][0]['target_id']);
        $row['life'] = $benefit_node->title;
      }

      $wb_node = $benefit_node = NULL;
      $params = array();
      $params['worker_nid'] = $worker_nid;
      $params['ts'] = $ts;
      $params['benefit_type'] = $medical_tid;
      $probe = sirius_trust_worker_benefits_search($params);
      $wb_node = reset($probe);
      if ($wb_node) {
        $benefit_node = node_load($wb_node->field_sirius_trust_benefit['und'][0]['target_id']);
        $row['medical'] = $benefit_node->title;
      }

      $params = array();
      $params['subscriber_nid'] = $worker_nid;
      $params['ts'] = $ts;
      $params['benefit_type'] = $medical_tid;
      $probe = sirius_trust_worker_benefits_search($params);
      $count = count($probe);
      $dependent_nids = array();
      foreach ($probe as $wb_nid => $wb_node) {
        $dependent_nid = $wb_node->field_sirius_worker['und'][0]['target_id'];
        $dependent_nids[$dependent_nid]++;
      }
      $count = count($dependent_nids) - 1;
      if ($count < 0) { $count = 0; }
      $row['dependent_count'] = $count;

      $y = $parameters['year']+0;
      $m = $parameters['month']+0;
      for ($i=0; $i<5; ++$i) {
        $sql = "select sum(hours) as c from sirius_hours_cache ";
        $sql .= "where worker_nid = :worker_nid ";
        $sql .= "and year = :y ";
        $sql .= "and month = :m ";
        $stmt = sirius_sql_query($sql, array(':worker_nid' => $worker_nid, ':y' => $y, ':m' => $m));
        $hr = $stmt->fetchAssoc();
        $row['hours_' . $i] = roun($hr['c'], 2);
        --$m;
        if ($m < 1) { $m = 12; --$y; }
      }

      $rows[$worker_nid] = $row;
    }

     return array('is_multiple' => TRUE, 'rows' => $rows);
  }
}
