<?php

class Sirius_Smf_Report_Multiindustry extends Report_Cache_Report_Generator {
  public $type = 'sirius-smf-multiindustry';

  public function info($result = array()) {
    $result = array(
      'name' => t('Workers with Hours in Multiple Industries'),
      'description' => '',
      'access' => 'sirius trust staff',
      'preview' => array(
        // 'default_display_fields' => array(),
        'preview_links' => TRUE,
        'preview_links' => TRUE,
      ),
      'dashboard' => TRUE,
      'module' => 'sirius_smf',
      'file' => 'reports/multiindustry.inc',
      'row_encoding' => 'json',
    );

    return parent::info($result);
  }

  public function fields() {
    $fields = array();
    $fields['worker_nid'] = 'NID';
    $fields['worker_name'] = 'Name';

    $fields['industry_0'] = 'Industry #1';
    $fields['hours_0'] = 'Hours #1';

    $fields['industry_1'] = 'Industry #2';
    $fields['hours_1'] = 'Hours #2';

    $fields['industry_2'] = 'Industry #3';
    $fields['hours_2'] = 'Hours #3';

    $fields['worker_phone'] = 'Phone';

    $cols = sirius_addr_cols();
    foreach ($cols as $col => $title) { $fields["worker_address_$col"] = $title; }

    return $fields;
  }

  public function preview_links($row) {
    return array(
      'node/' . $row['worker_nid'] => 'Worker',
      'node/' . $row['worker_nid'] . '/sirius_worker_hours' => 'Hours',
    );
  }

  public function form() {
    $form['year'] = array(
      '#type' => 'textfield',
      '#title' => t('Year'),
      '#size' => 4,
    );

    $form['month'] = array(
      '#title' => t('Month'),
      '#type' => 'textfield',
      '#size' => 2,
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
      form_set_error("parameters][year", 'Please enter a year.');
    }

    if (!$month || 
      $month != intval($month) ||
      $month < 1 || 
      $month > 12
    ) {
      form_set_error("parameters][month", 'Please enter a start month.');
    }
  }

  public function render_parameters($parameters) {
    $parameters = $parameters['parameters'];

    $html = sirius_jsonfield_util_render_all_values_new(
      $this->form(), 
      $parameters, 
      array(
        'skip_empty' => TRUE,
        'no_fieldsets' => TRUE,
      )
    );

    return $html;
  }

  public function pks(&$form_state) {
    $parameters = $form_state['values']['parameters'];

    $year = $parameters['year'];
    $month = $parameters['month'];

    if (strlen($month) < 2) { $month = "0$month"; }
    $date = "$year-$month-01 00:00:00";

    $sql_args = array();
    $sql = "select worker_nid, count(distinct field_sirius_industry_tid) as c ";
    $sql .= "from sirius_hours_cache ";
    $sql .= "left join node payperiod_node on payperiod_node.nid = payperiod_nid ";
    $sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = payperiod_node.nid ";
    $sql .= "left join field_data_field_grievance_shop on field_data_field_grievance_shop.entity_type = 'node' and field_data_field_grievance_shop.entity_id = payperiod_node.nid ";
    $sql .= "left join node employer_node on employer_node.nid = field_grievance_shop_target_id ";
    $sql .= "left join field_data_field_sirius_industry on field_data_field_sirius_industry.entity_type = 'node' and field_data_field_sirius_industry.entity_id = employer_node.nid ";

    $sql .= "where month = :month ";
    $sql_args[':month'] = $month;

    $sql .= "and year = :year ";
    $sql_args[':year'] = $year;

    $sql .= "and field_sirius_domain_target_id = :domain_nid ";
    $sql_args[':domain_nid'] = sirius_domain_nid();

    $sql .= "group by worker_nid ";
    $sql .= "having c > 1 ";
    $sql .= "order by worker_nid ";

    // $sql .= "limit 10";

    $stmt = sirius_sql_query($sql, $sql_args);

    $pks = array();
    while ($hr = $stmt->fetchAssoc()) {
      $pks[] = $hr['worker_nid'];
    }

    return array_chunk($pks, 100);
  }

  public function batch($worker_nids, $values, &$context) {
    $employer_industry_map = &drupal_static(__FUNCTION__);


    $year = $values['parameters']['year'];
    $year += 0;
    $month = $values['parameters']['month'];
    $month += 0; 

    $rows = array();
    foreach ($worker_nids as $worker_nid) {
      $sql_args = array();
      $sql = "select * from sirius_hours_cache ";
      $sql .= "where month = :month ";
      $sql_args[':month'] = $month;
      $sql .= "and year = :year ";
      $sql_args[':year'] = $year;
      $sql .= "and worker_nid = :worker_nid ";
      $sql_args[':worker_nid'] = $worker_nid;

      $hours_by_industry = array();
      $stmt = sirius_sql_query($sql, $sql_args);
      while ($hr = $stmt->fetchAssoc()) {
        if (!isset($employer_industry_map[$hr['employer_nid']])) {
          $employer_node = node_load($hr['employer_nid']);
          $employer_industry_map[$hr['employer_nid']] = $employer_node->field_sirius_industry['und'][0]['tid'];
        }

        $hours_by_industry[$employer_industry_map[$hr['employer_nid']]] += $hr['hours'];
      }
      ksort($hours_by_industry);

      $i = 0;
      foreach ($hours_by_industry as $industry_tid => $hours) {
        $title = sirius_term_title($industry_tid);
        if (!$title) { $title = '[no industry]'; }
        $row["industry_$i"] = $title;
        $row["hours_$i"] = $hours;
        ++$i;
      }

      $worker_node = node_load($worker_nid);
      $row['worker_nid'] = $worker_nid;
      $row['worker_name'] = $worker_node->title;
      $row['worker_phone'] = $worker_node->field_sirius_phone['und'][0]['value'];

      $cols = sirius_addr_cols();
      foreach ($cols as $col => $title) {
        $row["worker_address_$col"] = $worker_node->field_sirius_address['und'][0][$col];
      }
      $rows[$worker_nid] = $row;
    }

    return array('is_multiple' => TRUE, 'rows' => $rows);
  }
}
