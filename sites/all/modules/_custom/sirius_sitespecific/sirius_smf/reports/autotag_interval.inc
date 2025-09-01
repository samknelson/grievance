<?php

class Sirius_Smf_Report_Autotag_Interval extends Report_Cache_Report_Generator {
  public $type = 'sirius-smf-autotag-interval';

  public function info($result = array()) {
    $result = array(
      'name' => t('Autotag Interval Count'),
      'description' => '',
      'access' => 'sirius trust staff',
      'preview' => array(
        'default_display_fields' => array(
          'worker_nid',
          'worker_name',
          'worker_phone',
          'worker_ms_names',
          'employer_name',
          'policy_name',
          'count',
        ),
        'preview_links' => TRUE,
      ),
      'render_callbacks' => array(
        'Summary' => array(
          'method' => 'render_summary',
          'csv' => TRUE,
        ),
      ),
      'dashboard' => TRUE,
      'module' => 'sirius_smf',
      'file' => 'reports/autotag_interval.inc',
      'row_encoding' => 'json',
    );

    return parent::info($result);
  }

  public function fields() {
    $fields = array();
    $fields['worker_nid'] = 'Worker NID';
    $fields['worker_name'] = 'Worker Name';
    $fields['worker_phone'] = 'Phone';
    $fields['worker_thoroughfare'] = 'Address 1';
    $fields['worker_premise'] = 'Address 2';
    $fields['worker_locality'] = 'City';
    $fields['worker_administrative_area'] = 'State';
    $fields['worker_postal_code'] = 'Zip';
    $fields['worker_dob'] = 'DoB';
    $fields['worker_gender'] = 'Gender';
    $fields['worker_ms_tids'] = 'Member Status TIDs';
    $fields['worker_ms_names'] = 'Member Statuses';
    $fields['employer_nid'] = 'Employer NID';
    $fields['employer_name'] = 'Employer';
    $fields['policy_nid'] = 'Plan NID';
    $fields['policy_name'] = 'Plan';
    $fields['count'] = 'Count';
    return $fields;
  }

  public function preview_links($row) {
    $links = array();
    $links['node/' . $row['worker_nid']] = 'Worker';
    return $links;
  }

  public function form() {

    $employers = sirius_employer_list();
    $form['employer_nids'] = array(
      '#type' => 'select',
      '#title' => t('Current Employer'),
      '#options' => sirius_employer_list(),
      '#multiple' => TRUE,
      '#size' => 10,
    );

    $form['policy_nid'] = array(
      '#type' => 'select',
      '#title' => t('Plan'),
      '#options' => array(NULL => t('-- Select --')) + sirius_ledger_policy_options(),
    );

    $form['start_year'] = array(
      '#type' => 'textfield',
      '#title' => t('Start Year'),
      '#size' => 4,
    );

    $form['start_month'] = array(
      '#title' => t('Start Month'),
      '#type' => 'textfield',
      '#size' => 2,
    );

    $form['end_year'] = array(
      '#type' => 'textfield',
      '#title' => t('End Year'),
      '#size' => 4,
    );

    $form['end_month'] = array(
      '#title' => t('End Month'),
      '#type' => 'textfield',
      '#size' => 2,
    );

    $root_tid = sirius_taxonomy_lookup_by_anything('sirius_contact_tags', 'smf_autotag_root');
    $vid = taxonomy_vocabulary_machine_name_load('sirius_contact_tags')->vid;
    $autotags = taxonomy_get_tree($vid, $root_tid, NULL, TRUE);

    $options = array(NULL => t('-- Select --'));
    foreach ($autotags as $term) {
      $name = '';
      for ($i=0; $i<($term->depth); ++$i) { $name .= ' - '; }
      $name .= $term->name;
      $options[$term->tid] = $name;
    }

    $form['tags'] = array(
      '#title' => t('Tag'),
      '#type' => 'select',
      '#options' => $options,
    );

    return $form;
  }

  public function form_validate($form, &$form_state) {
    $parameters = $form_state['values']['parameters'];

    $start_year = $parameters['start_year'];
    $end_year = $parameters['end_year'];
    $start_month = $parameters['start_month'];
    $end_month = $parameters['end_month'];

    if (!$start_year || 
      $start_year != intval($start_year) ||
      $start_year <= 1900 || 
      $start_year >= 2100
    ) {
      form_set_error("parameters][start_year", 'Please enter a start year.');
    }

    if (!$start_month || 
      $start_month != intval($start_month) ||
      $start_month < 1 || 
      $start_month > 12
    ) {
      form_set_error("parameters][start_month", 'Please enter a start month.');
    }

    if (!$end_year ||
      $end_year != intval($end_year) ||
      $end_year <= 1900 || 
      $end_year >= 2100
    ) {
      form_set_error("parameters][end_year", 'Please enter an end year.');
    }

    if (!$end_month ||
      $end_month != intval($end_month) ||
      $end_month < 1 || 
      $end_month > 12
    ) {
      form_set_error("parameters][end_month", 'Please enter an end month.');
    }

    if ($end_month && $end_year) {
      if ( ($end_year < $start_year) ||
           ( ($end_year == $start_year) && ($end_month < $start_month) ) ) {
        form_set_error("parameters][end_year", 'The end date must be after the start date.');
      }
    }

    if (!$parameters['tags']) {
      form_set_error("parameters][tags", 'Please select a tag.');
    }
  }

  public function render_parameters($parameters) {
    $parameters = $parameters['parameters'];
    $parameters['tags'] = sirius_term_title($parameters['tags']);

    $html = sirius_jsonfield_util_render_all_values_new(
      $this->form(), 
      $parameters, 
      array(
        'skip_empty' => TRUE,
        'fieldset_title_prefix' => '<h2>',
        'fieldset_title_suffix' => '</h2>',
        'no_fieldsets' => TRUE,
        'separator' => '<br />',
      )
    );
    return $html;
  }

  public function pks(&$form_state) {
    $parameters = $form_state['values']['parameters'];

    $start_year = $parameters['start_year'] + 0;
    $start_month = $parameters['start_month'] + 0;
    if (strlen($start_month) < 2) { $start_month = "0$start_month"; }
    $start_date = "$start_year-$start_month-01 00:00:00";

    $end_year = $parameters['end_year'] + 0;
    $end_month = $parameters['end_month'] + 0;
    if (strlen($end_month) < 2) { $end_month = "0$end_month"; }
    $end_date = "$end_year-$end_month-01 00:00:00";

    $sql = "select field_sirius_worker_target_id, count(*) as c ";
    $sql .= "from node ";
    $sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
    $sql .= "left join field_data_field_sirius_worker on field_data_field_sirius_worker.entity_type = 'node' and field_data_field_sirius_worker.entity_id = node.nid ";
    $sql .= "left join field_data_field_sirius_date_start on field_data_field_sirius_date_start.entity_type = 'node' and field_data_field_sirius_date_start.entity_id = node.nid ";
    $sql .= "left join field_data_field_sirius_contact_tags on field_data_field_sirius_contact_tags.entity_type = 'node' and field_data_field_sirius_contact_tags.entity_id = node.nid ";
    $sql .= "left join field_data_field_grievance_shop on field_data_field_grievance_shop.entity_type = 'node' and field_data_field_grievance_shop.entity_id = node.nid ";

    $sql .= "where node.type = 'smf_worker_month' ";

    $sql .= "and field_sirius_domain_target_id = :domain_nid ";
    $sql_args[':domain_nid'] = sirius_domain_nid();

    $sql .= "and field_sirius_date_start_value >= :start_date ";
    $sql_args[':start_date'] = $start_date;

    $sql .= "and field_sirius_date_start_value <= :end_date ";
    $sql_args[':end_date'] = $end_date;

    $sql .= "and field_sirius_contact_tags_tid = :tag ";
    $sql_args[':tag'] = $parameters['tags'];

    if ($parameters['employer_nids']) {
      $employer_nids = array_filter(array_values($parameters['employer_nids']));
    }
    if ($employer_nids) {
      $sql .= "and field_grievance_shop_target_id in (:employer_nids) ";
      $sql_args[':employer_nids'] = $employer_nids;
    }

    $sql .= "group by field_sirius_worker_target_id ";
    // $sql .= "limit 10";

    $stmt = sirius_sql_query($sql, $sql_args);
    $pks = array();
    while ($hr = $stmt->fetchAssoc()) {
      $pk = $hr['field_sirius_worker_target_id'] . '|' . $hr['c'];
      $pks[] = $pk;
    }
    return array_chunk($pks, 500);
  }

  public function batch($pks, $values, &$context) {
    $rows = array();
    $term_name_cache = array();
    foreach ($pks as $pk) {
      list ($worker_nid, $count) = explode('|', $pk);

      $worker_node = node_load($worker_nid);
      $policy_node = sirius_ledger_policy_lookup($worker_node);
      if ($values['parameters']['policy_nid']) {
        if ($values['parameters']['policy_nid'] != $policy_node->nid) { continue; }
      }

      $row = array();
      $row['count'] = $count;

      $row['worker_nid'] = $worker_nid;

      $dob = $worker_node->field_sirius_dob['und'][0]['value'];
      if ($dob) { $dob = date('m/d/Y', strtotime($dob)); }
      $row['worker_dob'] = $dob;

      $row['worker_name'] = $worker_node->title;
      $row['worker_thoroughfare'] = $worker_node->field_sirius_address['und'][0]['thoroughfare'];
      $row['worker_premise'] = $worker_node->field_sirius_address['und'][0]['premise'];
      $row['worker_locality'] = $worker_node->field_sirius_address['und'][0]['locality'];
      $row['worker_administrative_area'] = $worker_node->field_sirius_address['und'][0]['administrative_area'];
      $row['worker_postal_code'] = $worker_node->field_sirius_address['und'][0]['postal_code'];

      $row['worker_gender'] = $worker_node->field_sirius_gender_nota_calc['und'][0]['value'];

      $row['worker_phone'] = $worker_node->field_sirius_phone['und'][0]['value'];

      $ms_tids = sirius_fieldvals($worker_node, 'field_sirius_member_status', 'tid');
      $ms_names = array();
      foreach ($ms_tids as $ms_tid) {
        if (!$term_name_cache[$ms_tid]) { $term_name_cache[$ms_tid] = sirius_term_title($ms_tid); }
        if (!$term_name_cache[$ms_tid]) { continue; }
        $ms_names[] = $term_name_cache[$ms_tid];
      }
      $row['worker_ms_tids'] = join(' / ', $ms_tids);
      $row['worker_ms_names'] = join(' / ', $ms_names);

      $employer_nid = $worker_node->field_grievance_shop['und'][0]['target_id'];
      $employer_name = sirius_node_title($employer_nid);
      $row['employer_nid'] = $employer_nid;
      $row['employer_name'] = $employer_name;

      if ($policy_node) {
        $row['policy_nid'] = $policy_node->nid;
        $row['policy_name'] = $policy_node->title;
      }

      $rows[$worker_nid] = $row;
      sirius_minilog_reset();
    }
    return array('is_multiple' => TRUE, 'rows' => $rows);
  }

  public function preview_form_alter(&$form, &$form_state, $report_id, $info) {
    $form['filters']['count_min'] = array(
      '#type' => 'textfield',
      '#size' => 2,
      '#title' => t('Minimum number of months'),
      '#weight' => -1,
    );

    $form['filters']['count_max'] = array(
      '#type' => 'textfield',
      '#size' => 2,
      '#title' => t('Maximum number of months'),
      '#weight' => -1,
    );
  }

  public function preview_rows_per_page($form, $form_state, $report_id, $info) {
    return 100;
  }

  public function preview_query($form, $form_state, $report_id, $info, $count_only = FALSE) {
    $count_min = $form_state['values']['filters']['count_min'];
    if ($count_min && ($count_min == intval($count_min)) && ($count_min > 0)) {
      $count_min = intval($count_min);
    } else {
      $count_min = NULL;
    }

    $count_max = $form_state['values']['filters']['count_max'];
    if ($count_max && ($count_max == intval($count_max)) && ($count_max > 0)) {
      $count_max = intval($count_max);
    } else {
      $count_max = NULL;
    }

    $domain_nid = sirius_domain_nid(); if (!$domain_nid) { $domain_nid = 0; }

    $sql_args = array();

    if ($count_only) {
      $sql = "select count(*) as c from report_cache ";
    } else {
      $sql .= "select * from report_cache ";
    }

    $sql .= "where report_id = :report_id ";
    $sql_args[':report_id'] = $report_id;

    $sql .= "and report_type = :report_type ";
    $sql_args[':report_type'] = $this->type;

    $sql .= "and report_domain_nid = :domain_nid ";
    $sql_args[':domain_nid'] = $domain_nid; 

    if ($count_min > 0) {
      $sql .= 'and cast(json_unquote(json_extract(report_data, \'$.count\')) as UNSIGNED) >= :count_min ';
      $sql_args[':count_min'] = $count_min;
    }

    if ($count_max > 0) {
      $sql .= 'and cast(json_unquote(json_extract(report_data, \'$.count\')) as UNSIGNED) <= :count_max ';
      $sql_args[':count_max'] = $count_max;
    }

    if (!$count_only) {
      $limit = $this->preview_rows_per_page($form, $form_state, $report_id, $info);
      $page = $form_state['values']['filters']['page'];
      if (is_numeric($page)) {
        $offset = $page * $limit;
      } else {
        $offset = 0;
      }
      $sql .= "limit $limit offset $offset ";
    }

    $stmt = sirius_sql_query($sql, $sql_args);

    if ($count_only) {
      $hr = $stmt->fetchAssoc();
      return $hr['c'];
    }

    return $stmt;
  }


  public function render_summary($report_id, $output = 'page') {
    $info = $this->info();
    $parameters = $this->parameters($report_id);

    $query = db_select('report_cache');
    $query->fields('report_cache');
    $query->condition('report_id', $report_id);
    $query->condition('report_type', $this->type);
    $stmt = $query->execute();
    $counts = array(); // I'm using the word "count" in too many stupid ways :(
    while ($hr = $stmt->fetchAssoc()) {
      $data = $this->row_decode($hr['report_data']);
      $counts[$data['count']]++;
    }
    ksort($counts);

    $rows = array();
    foreach ($counts as $key => $value) {
      $row = array();
      $row[] = $key;
      $row[] = $value;
      $rows[] = $row;
    }

    $header[] = 'Number of Months';
    $header[] = 'Number of Workers';

    return array('header' => $header, 'rows' => $rows);
  }
}
