<?php

/*
Reporting is a little challenging for two reasons.

- First, we should be counting "comm" nodes, which is the shiny new method that we have to track a communication record. But we haven't launched it yet. 

- Second, we don't have a quick an easy way to count these messages. We should be flagging the comm node as e.g. "This is an instance of AutoTag type [MLK], message [es1]". But we're not doing that, and the comm node doesn't have any obvious place to put that data.

(We are generating a unique key, to avoid resending the same message -- but I'm a little concerned about overpowering the unique key to also serve as a query and reporting tool. Apart from anything else, there are no partial indices, so it won't scale well.)

Leaving this assigned to me to think about for a while. I'll also prioritize getting comm records launched.
*/

class Sirius_Smf_Report_Automsg extends Report_Cache_Report_Generator {
  public $type = 'sirius-smf-automsg';

  public function info($result = array()) {
    $result = array(
      'name' => t('Auto-Message [UNDER DEVELOPMENT]'),
      'description' => '',
      'access' => 'sirius trust staff',
      'render_callbacks' => array(
        'View' => array(
          'method' => 'render_view',
          'csv' => TRUE,
        ),
      ),
      'dashboard' => TRUE,
      'module' => 'sirius_smf',
      'file' => 'reports/automsg.inc',
    );

    return parent::info($result);
  }

  public function fields() {
    $fields = array();
    $fields['type'] = 'Tpe';
    $fields['msg_key'] = 'Message Key';
    $fields['msg_title'] = 'Message Name';
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
    return array();
  }

  public function batch($employer_nid, $values, &$context) {
    drupal_set_message("Employer: $employer_nid");
    dpm($values);
    dpm($context);

    $year = $values['parameters']['year'];
    $month = $values['parameters']['month'];

    if (!$year) { $year = date('Y'); }
    if (!$month) { $month = date('m'); }

    $row = array();
    $row['employer_nid'] = $employer_nid;
    $row['employer_name'] = $employer_name;

    return $row;
  }

  public function render_view($report_id, $output = 'page', $filter_values) {
    $info = $this->info();
    $parameters = $this->parameters($report_id);
    $raw_data = $this->load($report_id);

    $filters = array();
    $id = $this->type . '_filter_employers';
    $filters['employers'] = array(
      '#type' => 'select',
      '#title' => t('Employers'),
      '#options' => array(
        'active' => t('Active Employers'),
        'all' => t('All Employers'),
        'selected' => t('Selected Employers'),
      ),
      '#id' => $id,
    );

    $filters['employers_selected_wrap'] = array(
      '#type' => 'fieldset',
      '#title' => 'Select Employers',
      '#states' => array('visible' => array(":input[id=\"$id\"]" => array('value' => 'selected'))),
    );

    $filters['employers_selected_wrap']['employers'] = array(
      '#type' => 'checkboxes',
      '#checkall' => TRUE,
      '#options' => $employers_all,
    );

    // Get our employer list
    if ($filter_values['employers'] == 'all') { $employer_nids = array_keys($employers_all); }
    else if ($filter_values['employers'] == 'selected') {
      $employer_nids = array_filter(array_values($filter_values['employers_selected_wrap']['employers']));

    }
    else { $employer_nids = array_keys($employers_active); }

    // Get our combos
    $combos = $this->combos();

    // Generate our rows
    $totals = array();
    foreach ($employer_nids as $employer_nid) {
      $row = array();
      $row['employer'] = $employers_all[$employer_nid];

      foreach ($combos as $combo_key => $record) {
        $totals[$combo_key] += $raw_data[$employer_nid][$combo_key];
        if ($record['parent']) { continue; }
        $row[$combo_key] = $raw_data[$employer_nid][$combo_key];

        if ($record['children']) {
          $sub_vals = array();
          foreach ($record['children'] as $sub_key) {
            $sub_vals[] = $raw_data[$employer_nid][$sub_key];
          }
          $row[$combo_key] .= '<br />' . join('/', $sub_vals);
        }
      }

      $rows[] = $row;
    }

    $row = array();
    $row['employer'] = '<strong>' . t('Total') . '</stron>';
    foreach ($combos as $combo_key => $record) {
      if ($record['parent']) { continue; }
      $row[$combo_key] = '<strong>' . $totals[$combo_key] . '</strong>';

      if ($record['children']) {
        $sub_vals = array();
        foreach ($record['children'] as $sub_key) {
          $sub_vals[] = $totals[$sub_key];
        }
        $row[$combo_key] .= '<strong>' . '<br />' . join('/', $sub_vals) . '</strong>';
      }
    }
    $rows[] = $row;

    $header = array();
    $header['employer'] = 'Employer';
    foreach ($combos as $combo_key => $record) {
      if ($record['parent']) { continue; }
      $header[$combo_key] = $record['title'];
      if ($record['children']) {
        $header[$combo_key] .= '<br />' . 'S/+1/F';
      }
    }

    return array('header' => $header, 'rows' => $rows, 'filters' => $filters);
  }

  public function dashboard_render_full($prefs, $options, $report_id) {
    $report_type = $this->type;
    $info = report_cache_info($report_type);
    $parameters = $this->parameters($report_id);
    $breakouts = $this->breakouts($parameters);
    $data = $this->load($report_id);

    $rows = array();
    foreach ($data as $hr) {
      $row = array();

      $names = array();
      foreach ($breakouts as $breakout) {
        $name = $hr[$breakout . "_name"];
        if (!$name) { 
          // if ($breakout == 'car') { $name = '[CURRENT]'; } else { $name = 'TOTAL'; }
        }
        if ($name) { $names[] = $name; }
      }
      if (!$names) { continue; }

      $details = join(' / ', $names);
      $details .= "<br />";
      $details .= "<strong>$hr[subscriber_count]</strong> subscribers / ";
      $details .= "<strong>$hr[dependent_count]</strong> covered lives";

      $row = array();
      $row['tools'] = l('View', "sirius/reports/$report_type", array('query' => array('report_id' => $report_id, 'action' => 'render', 'callback-name' => 'View'), 'attributes' => array('class' => array('tool-button'))));
      $count = $parameters['meta']['dashboard']['count'];
      $row['count'] = $hr['record_count'];
      $row['details'] = $details;

      $rows[] = $row;
    }

    return $rows;
  }

}