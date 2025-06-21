<?php

class Sirius_Smf_Report_Disabilitywithoutfmla extends Report_Cache_Report_Generator {
  public $type = 'sirius-smf-disabilitywithoutfmla';

  public function info($result = array()) {
    $result = array(
      'name' => t('Disability Without FMLA'),
      'description' => '',
      'access' => 'sirius trust staff',
      'preview' => array(
        // 'default_display_fields' => array(),
        'preview_links' => TRUE,
      ),
      'dashboard' => TRUE,
      'module' => 'sirius_smf',
      'file' => 'reports/disability_without_fmla.inc',
      'row_encoding' => 'json',
    );

    return parent::info($result);
  }

  public function fields() {
    $fields = array();
    $fields['worker_nid'] = 'NID';
    $fields['worker_name'] = 'Name';
    $fields['first_ym'] = "Started Disability";
    $fields['worker_phone'] = 'Phone';

    $cols = sirius_addr_cols();
    foreach ($cols as $col => $title) { $fields["worker_address_$col"] = $title; }

    return $fields;
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

  public function preview_links($row) {
    return array(
      'node/' . $row['worker_nid'] => 'Worker',
      'node/' . $row['worker_nid'] . '/sirius_worker_hours' => 'Hours',
    );
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
    $year = $parameters['year'];
    $month = $parameters['month'];
    if (strlen($month) < 2) { $month = "0$month"; }
    $date = "$year-$month-01 00:00:00";

    $disability_tid = sirius_taxonomy_lookup_by_anything('sirius_contact_tags', 'smf_autotag_status_disability');

    $sql_args = array();
    $sql = "select field_sirius_worker_target_id as worker_nid from node ";
    $sql .= "left join field_data_field_grievance_shop on field_data_field_grievance_shop.entity_type = 'node' and field_data_field_grievance_shop.entity_id = node.nid ";
    $sql .= "left join field_data_field_sirius_date_start on field_data_field_sirius_date_start.entity_type = 'node' and field_data_field_sirius_date_start.entity_id = node.nid ";
    $sql .= "left join field_data_field_sirius_contact_tags on field_data_field_sirius_contact_tags.entity_type = 'node' and field_data_field_sirius_contact_tags.entity_id = node.nid ";
    $sql .= "left join field_data_field_sirius_worker on field_data_field_sirius_worker.entity_type = 'node' and field_data_field_sirius_worker.entity_id = node.nid ";
   $sql .= "where node.type = 'smf_worker_month' ";

    $sql .= "and field_sirius_date_start_value = :date ";
    $sql_args[':date'] = "$year-$month-01 00:00:00";

    $sql .= "and field_sirius_contact_tags_tid = :disability_tid ";
    $sql_args[':disability_tid'] = $disability_tid;
    $stmt = sirius_sql_query($sql, $sql_args);

    $pks = array();
    while ($hr = $stmt->fetchAssoc()) {
      $pks[] = $hr['worker_nid'];
    }

    return array_chunk($pks, 100);
  }

  public function batch($pks, $values, &$context) {
    $year = $values['parameters']['year'];
    $year += 0;
    $month = $values['parameters']['month'];
    $month += 0; 

    $disability_tid = sirius_taxonomy_lookup_by_anything('sirius_contact_tags', 'smf_autotag_status_disability');
    $fmla_tid = sirius_taxonomy_lookup_by_anything('sirius_contact_tags', 'smf_autotag_status_fmla');

    $rows = array();
    foreach ($pks as $worker_nid) {
      $tmp = sirius_smf_wym_fetch_all($worker_nid);
      $wym_nodes = array();
      foreach ($tmp as $wym_nid => $wym_node) {
        $ts = strtotime($wym_node->field_sirius_date_start['und'][0]['value']);
        $m = date('n', $ts);
        $y = date('Y', $ts);
        $wym_nodes["$y-$m"] = $wym_node;
      }

      $done = FALSE;
      $had_fmla = FALSE;
      $had_fmla_and_disability = FALSE;
      $first_ym = '';
      $m = $month;
      $y = $year;
      while (!$done) {
        // Go back one month
        --$m;
        if ($m < 1) { $m = 12; --$y; }

        $wym_node = $wym_nodes["$y-$m"];
        $tids = sirius_fieldvals($wym_node, 'field_sirius_contact_tags', 'tid');

        // If we have disability ... 
        if (in_array($disability_tid, $tids)) {
          // Decrement "First YM"
          $first_ym = "$y-$m";

          // do we ALSO have FMLA?
          $had_fmla_and_disability = in_array($fmla_tid, $tids);

          // Keep going backwards, because we need to find the first non-disability month
          continue;
        }

        // We are now in the first month before the first disability
        if (in_array($fmla_tid, $tids)) { $had_fmla = TRUE; }

        break;
      }

      if ($had_fmla) { continue; }
      if ($had_fmla_and_disability) { continue; }

      $worker_node = node_load($worker_nid);
      $row['worker_nid'] = $worker_nid;
      $row['worker_name'] = $worker_node->title;
      $row['first_ym'] = $first_ym;
      $row['worker_phone'] = $worker_node->field_sirius_phone['und'][0]['value'];

      $cols = sirius_addr_cols();
      foreach ($cols as $col => $title) {
        $row["worker_address_$col"] = $worker_node->field_sirius_address['und'][0][$col];
      }
      $rows[$worker_nid] = $row;
    }

    return array('is_multiple' => TRUE, 'rows' => $rows);
  }

  public function load($report_id) {
    $info = $this->info();
    $report_type = $this->type;

    $query = db_select('report_cache');
    $query->fields('report_cache');
    $query->condition('report_id', $report_id);
    $query->condition('report_type', $report_type);
    $stmt = $query->execute();
    $data = array();
    while ($hr = $stmt->fetchAssoc()) {
      $data[$hr['report_row_pk']] = $this->row_decode($hr['report_data']);
    }
    return $data;
  }

  public function preview_rows_per_page($form, $form_state, $report_id, $info) {
    return 100;
  }
}
