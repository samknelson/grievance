<?php

class Sirius_Smf_Report_Actuarial_Hours extends Report_Cache_Report_Generator {
  public $type = 'sirius-smf-actuarial_hours';

  public function info($result = array()) {
    $result = array(
      'name' => t('Actuarial Hours'),
      'description' => '',
      'access' => 'sirius trust staff',
      'preview' => array(
        'default_display_fields' => array(
          'year',
          'month',
          'employer_name',
          'total',
          'eligible',
          'noneligible',
        ),
        'preview_links' => TRUE,
      ),
      'dashboard' => TRUE,
      'module' => 'sirius_smf',
      'file' => 'reports/actuarialhours.inc',
      'row_encoding' => 'json',
    );

    return parent::info($result);
  }

  public function fields() {
    $fields = array();
    $fields['year'] = 'Year';
    $fields['month'] = 'Month';
    $fields['employer_nid'] = 'Employer NID';
    $fields['employer_name'] = 'Employer Name';
    $combos = $this->combos();
    foreach ($combos as $combo_key => $combo) {
      $fields[$combo_key] = $combo['title'];
    }
    return $fields;
  }

  public function preview_links($row) {
    $links = array();
    $links['modal'] = array(
      'type' => 'modal',
    );
    return $links;
  }

  public function form() {
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

    if ($end_year) {
      if ($end_year != intval($end_year) ||
        $end_year <= 1900 || 
        $end_year >= 2100
      ) {
        form_set_error("parameters][start_year", 'Please enter a start year.');
      }
    }

    if ($end_month) {
      if ($end_month != intval($end_month) ||
        $end_month < 1 || 
        $end_month > 12
      ) {
        form_set_error("parameters][end_month", 'Please enter an end month.');
      }
    }

    if ($end_year && !$end_month) {
      form_set_error("parameters][end_month", 'Please enter an end month.');
    }

    if ($end_month && !$end_year) {
      form_set_error("parameters][end_year", 'Please enter an end year.');
    }

    if ($end_month && $end_year) {
      if ( ($end_year < $start_year) ||
           ( ($end_year == $start_year) && ($end_month < $start_month) ) ) {
        form_set_error("parameters][end_year", 'The end date must be after the start date.');
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

    $month_limit = 100;
    $y = $parameters['start_year'];
    $m = $parameters['start_month'];

    $end_year = $parameters['end_year'];
    if (!$end_year) { $end_year = $parameters['start_year']; }

    $end_month = $parameters['end_month'];
    if (!$end_month) { $end_month = $parameters['start_month']; }

    $employer_nids = array_keys(sirius_employer_list());
    //$employer_nids = array_slice($employer_nids, 0, 3);
    $pks = array();    

    while ($y < $end_year || ($y == $end_year && $m <= $end_month)) {
      foreach ($employer_nids as $employer_nid) {
        $pks[] = "$y|$m|$employer_nid";
      }

      ++$m;
      if ($m > 12) { $m = 1; ++$y; }

      ++$count;
      if ($count > $month_limit) { break; }
    }

    return $pks;
  }

  public function combos() {
    $combos = array();

    $combos['total'] = array(
      'title' => 'Total Reported',
      'has_all_tags' => array('smf_autotag_hrs_nonzero60'),
    );

    $combos['eligible'] = array(
      'title' => 'Eligible',
      'has_any_tags' => array('smf_autotag_hrs_buildup', 'smf_autotag_hrs_buildup_complete'),
    );

    $combos['noneligible'] = array(
      'title' => 'Non-Eligible',
      'has_all_tags' => array('smf_autotag_hrs_break'),
    );

    return $combos;
  }

  public function batch($pk, $values, &$context) {
    list ($year, $month, $employer_nid) = explode('|', $pk);

    if (strlen($month) < 2) { $month = "0$month"; }
    $date = "$year-$month-01 00:00:00";

    $employer_name = sirius_node_title($employer_nid);

    $row = array();
    $row['year'] = $year;
    $row['month'] = $month;
    $row['employer_nid'] = $employer_nid;
    $row['employer_name'] = $employer_name;

    $combos = $this->combos();

    foreach ($combos as $combo_key => $combo) {

      $sql_args = array();
      $sql = "select sum(hours) as c from node ";
      $sql .= "left join field_data_field_grievance_shop on field_data_field_grievance_shop.entity_type = 'node' and field_data_field_grievance_shop.entity_id = node.nid ";
      $sql .= "left join field_data_field_sirius_worker on field_data_field_sirius_worker.entity_type = 'node' and field_data_field_sirius_worker.entity_id = node.nid ";
      $sql .= "left join field_data_field_sirius_date_start on field_data_field_sirius_date_start.entity_type = 'node' and field_data_field_sirius_date_start.entity_id = node.nid ";
      $sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
      $sql .= "left join sirius_hours_cache on (";
      $sql .= "  worker_nid = field_sirius_worker_target_id and ";
      $sql .= "  employer_nid = :employer_nid and ";
      $sql .= "  year = :year and ";
      $sql .= "  month = :month ";
      $sql .= ") ";
      $sql_args[':year'] = $year;
      $sql_args[':month'] = ($month + 0);
      $sql .= "where node.type = 'smf_worker_month' ";

      $sql .= "and field_sirius_domain_target_id = :domain_nid ";
      $sql_args[':domain_nid'] = sirius_domain_nid();

      // $sql .= "and field_grievance_shop_target_id = :employer_nid ";
      $sql_args[':employer_nid'] = $employer_nid;

      $sql .= "and field_sirius_date_start_value = :date ";
      $sql_args[':date'] = $date;

      if ($combo['has_all_tags']) {
        $count = 0;
        foreach ($combo['has_all_tags'] as $tag_id) {
          ++$count;
          $sql .= "and exists (";
          $sql .= "  select 1 from field_data_field_sirius_contact_tags sub_tags_$count ";
          $sql .= "  where sub_tags_$count.entity_type = 'node' ";
          $sql .= "  and sub_tags_$count.entity_id = node.nid ";
          $sql .= "  and sub_tags_$count.field_sirius_contact_tags_tid = :sub_tags_${count}_tid ";
          $sql .= ") ";
          $sql_args[":sub_tags_${count}_tid"] = sirius_taxonomy_lookup_by_anything('sirius_contact_tags', $tag_id);
        }
      }

      if ($combo['has_no_tags']) {
        ++$count;
        $not_tids = array();
        foreach ($combo['has_no_tags'] as $tag_id) { 
          $not_tids[] = sirius_taxonomy_lookup_by_anything('sirius_contact_tags', $tag_id);
        }

        $sql .= "and not exists (";
        $sql .= "  select 1 from field_data_field_sirius_contact_tags sub_tags_$count ";
        $sql .= "  where sub_tags_$count.entity_type = 'node' ";
        $sql .= "  and sub_tags_$count.entity_id = node.nid ";
        $sql .= "  and sub_tags_$count.field_sirius_contact_tags_tid in (:not_tids) ";
        $sql .= ") ";
        $sql_args[":not_tids"] = $not_tids;
      }

      if ($combo['has_any_tags']) {
        ++$count;
        $any_tids = array();
        foreach ($combo['has_any_tags'] as $tag_id) { 
          $any_tids[] = sirius_taxonomy_lookup_by_anything('sirius_contact_tags', $tag_id);
        }

        $sql .= "and exists (";
        $sql .= "  select 1 from field_data_field_sirius_contact_tags sub_tags_$count ";
        $sql .= "  where sub_tags_$count.entity_type = 'node' ";
        $sql .= "  and sub_tags_$count.entity_id = node.nid ";
        $sql .= "  and sub_tags_$count.field_sirius_contact_tags_tid in (:any_tids) ";
        $sql .= ") ";
        $sql_args[":any_tids"] = $any_tids;
      }

      $stmt = sirius_sql_query($sql, $sql_args);
      $hr = $stmt->fetchAssoc();
      $c = $hr['c'];
      if (!$c) { $c = 0; }
      $row[$combo_key] = $c;
    }

    return $row;
  }

  public function preview_form_alter(&$form, &$form_state, $report_id, $info) {
    $employers_active = sirius_employer_list(TRUE);
    $employers_all = sirius_employer_list(FALSE);

    $form['filters']['year'] = array(
      '#type' => 'textfield',
      '#size' => 4,
      '#title' => t('Year'),
      '#weight' => -1,
    );

    $form['filters']['month'] = array(
      '#type' => 'textfield',
      '#size' => 2,
      '#title' => t('Month'),
      '#weight' => -1,
    );

    $id = "employer_filter_which";
    $form['filters']['employers'] = array(
      '#type' => 'select',
      '#title' => t('Employers'),
      '#options' => array(
        'all' => t('All Employers'),
        'active' => t('Active Employers'),
        'selected' => t('Selected Employers'),
      ),
      '#id' => $id,
      '#weight' => -1,
    );

    $form['filters']['employers_selected_wrap'] = array(
      '#type' => 'fieldset',
      '#title' => 'Select Employers',
      '#states' => array('visible' => array(":input[id=\"$id\"]" => array('value' => 'selected'))),
      '#weight' => -1,
    );

    $form['filters']['employers_selected_wrap']['employers'] = array(
      '#type' => 'checkboxes',
      '#checkall' => TRUE,
      '#options' => $employers_all,
    );
  }

  public function preview_rows_per_page($form, $form_state, $report_id, $info) {
    return 100;
  }

  public function preview_query($form, $form_state, $report_id, $info, $count_only = FALSE) {
    $employers_active = sirius_employer_list(TRUE);
    $employers_all = sirius_employer_list(FALSE);

    $year = $form_state['values']['filters']['year'];
    $month = $form_state['values']['filters']['month'];

    if ($form_state['values']['filters']['employers'] == 'active') {
      $employer_nids = array_filter(array_keys($employers_active));
    } else if ($form_state['values']['filters']['employers'] == 'selected') {
      $employer_nids = array_filter(array_values($form_state['values']['filters']['employers_selected_wrap']['employers']));
    }
    else {
      $employer_nids = NULL;
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

    if ($year) {
      $sql .= 'and json_extract(report_data, \'$.year\') = :year ';
      $sql_args[':year'] = $year;
    }
    if ($month) {
      if (strlen($month) < 2) { $month = "0$month"; }
      $sql .= 'and json_extract(report_data, \'$.month\') = :month ';
      $sql_args[':month'] = $month;
    }
    if ($employer_nids) {
      // For some fucking reason bind variables aren't handling arrays. Dammit.

      // This is a quick-and-dirty mysql escape function. Blech.
      foreach ($employer_nids as $employer_nid) {
        if (!is_numeric($employer_nid)) { return NULL; }
      }

      $sql .= 'and json_extract(report_data, \'$.employer_nid\') in (' . join(',', $employer_nids) . ') ';
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

  public function render_modal($report_id, $info, $data) {

    $parameters = $this->parameters($report_id);
    $parameters = $parameters['parameters'];
    $year = $data['year'];
    $month = $data['month'];

    $info = $this->info();
    $fields = $info['fields'];
    $combos = $this->combos();

    $html .= '<h1>' . $data['employer_name'] . ": $month / $year" . '</h1>';

    $rows = array();

    foreach ($combos as $key => $record) {
      if ($record['parent']) { continue; }

      $row = array();

      $title = $fields[$key];

      $row = array();
      $row[] = $fields[$key];
      $row[] = number_format($data[$key]);
      $row[] = $this->drilldown_link($year, $month, $data['employer_nid'], $combos[$key]);
      $rows[] = $row;

      if ($record['children']) {
        foreach ($record['children'] as $sub_key) {
          $sub_title = $fields[$sub_key];
          $sub_title = preg_replace('/' . $title . ' AND /', '', $sub_title);
          $sub_title = "&nbsp;&nbsp;&nbsp;-&nbsp;" . $sub_title;

          $row = array();
          $row[] = $sub_title;
          $row[] = number_format($data[$sub_key]);
          $row[] = $this->drilldown_link($year, $month, $data['employer_nid'], $combos[$sub_key]);
          $rows[] = $row;
        }
      }
    }

    $header = array();
    $header[] = t('Field');
    $header[] = t('Value');
    $header[] = t('Tools');

    $attributes = array('class' => array('sirius_data_table'));
    $html .= sirius_table_render($header, $rows, $attributes);
    return $html;
  }

  function drilldown_link($year, $month, $employer_nid, $combo) {
    $month = preg_replace('/^0/', '', $month);
    $path = 'sirius/smf/worker-month';
    $query = array();
    $query['field_sirius_date_start_value']['value']['month'] = $month;
    $query['field_sirius_date_start_value']['value']['year'] = $year;
    $query['field_grievance_shop_target_id'][] = $employer_nid;

    if ($combo['has_all_tags']) {
      $query['field_sirius_contact_tags_tid_op'] = 'and';
      foreach ($combo['has_all_tags'] as $tag_id) {
        $query['field_sirius_contact_tags_tid'][] = sirius_taxonomy_lookup_by_anything('sirius_contact_tags', $tag_id);
      }
    }

    if ($combo['has_any_tags']) {
      $query['field_sirius_contact_tags_tid_op'] = 'or';
      foreach ($combo['has_any_tags'] as $tag_id) {
        $query['field_sirius_contact_tags_tid'][] = sirius_taxonomy_lookup_by_anything('sirius_contact_tags', $tag_id);
      }
    }

    if ($combo['has_no_tags']) {
      foreach ($combo['has_no_tags'] as $tag_id) {
        $query['field_sirius_contact_tags_tid_1'][] = sirius_taxonomy_lookup_by_anything('sirius_contact_tags', $tag_id);
      }
    }
    return l("Drilldown", $path, array('query' => $query, 'attributes' => array('target' => '_blank', 'class' => array('tool-button'))));
  }
}
