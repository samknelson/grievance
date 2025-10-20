<?php

class Sirius_Cardcheck_Report_Acceptance extends Report_Cache_Report_Generator {
  public $type = 'sirius-cardcheck-acceptance';

  public function info($result = array()) {
    $result = array(
      'name' => t('Acceptance'),
      'description' => '',
      'access' => 'sirius cardcheck admin',
      'preview' => array(
        'preview_links' => TRUE,
      ),
      'dashboard' => TRUE,
      'module' => 'sirius_cardcheck',
      'file' => 'reports/acceptance.inc',
      'row_encoding' => 'json',
    );

    return parent::info($result);
  }

  public function fields() {
    $fields['worker_nid'] = 'Worker NID';
    $fields['worker_name'] = 'Worker Name';
    $fields['worker_id'] = 'Worker ID';
    $fields['worker_title'] = 'Worker Title';
    $fields['worker_employer_name'] = 'Employer';
    $fields['cardcheck_bu_name'] = 'BU Name';
    $fields['cardcheck_bu_id'] = 'BU ID';
    $fields['cardcheck_acceptance_date'] = 'Acceptance Date';

    return $fields;
  }

  public function preview_links($row) {
    $links = array();
    if ($row['worker_nid']) { $links['node/' . $row['worker_nid'] . '/sirius_worker_cardchecks'] = 'Worker'; }
    return $links;
  }

  public function form() {
    $form['start_date'] = array(
      '#type' => 'date_popup',
      '#title' => t('Start Date'),
      '#attributes' => array(
        'autocomplete' => 'off',
      ),
      '#date_format' => 'Y-m-d',
      '#required' => TRUE,
      '#default_value' => date('Y-m-d', strtotime('-10 days')),
    );

    $form['end_date'] = array(
      '#type' => 'date_popup',
      '#title' => t('End Date'),
      '#attributes' => array(
        'autocomplete' => 'off',
      ),
      '#date_format' => 'Y-m-d',
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
    $parameters = $form_state['values']['parameters'];

    $start_ts = strtotime($parameters['start_date']);
    if ($parameters['end_date']) {
      $end_ts = strtotime($parameters['end_date']);
    }

    $domain_nid = sirius_domain_nid();

    $sql_args = array();

    $sql = "select nid from node ";
    $sql .= "left join field_data_field_sirius_category on field_data_field_sirius_category.entity_type = 'node' and field_data_field_sirius_category.entity_id = node.nid ";
    $sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
    $sql .= "where ";

    if ($domain_nid) {
      $sql .= "field_sirius_domain_target_id = :domain_nid ";
      $sql_args[':domain_nid'] = $domain_nid;
    } else {
      $sql .= "field_sirius_domain_target_id is null ";
    }

    $sql .= "and field_sirius_category_value = 'cardcheck' ";

    $sql .= "and changed >= :start_ts ";
    $sql_args[':start_ts'] = $start_ts;

    if ($end_ts) {
      $sql .= "and changed <= :end_ts ";
      $sql_args[':end_ts'] = $end_ts;
    }
    $sql .= "order by nid ";


    $stmt = sirius_sql_query($sql, $sql_args);

    while ($hr = $stmt->fetchAssoc()) {
      $pks[] = $hr['nid'];
    }

    return array_chunk($pks, 100);
  }

  public function batch($pks, $values, &$context) {
    $parameters = $values['parameters'];
    $start_ts = strtotime($parameters['start_date']);
    if ($parameters['end_date']) {
      $end_ts = strtotime($parameters['end_date']);
    }

    $cardcheck_instance = Sirius_Cardcheck::getInstance();

    $rows = array();
    foreach ($pks as $cardcheck_nid) {
      $cardcheck_node = node_load($cardcheck_nid);
      $cardcheck_json = sirius_json_get($cardcheck_node);
      $cardcheck = $cardcheck_json['cardcheck'];

      $ts = $cardcheck['acceptance']['ts'];

      if (!$ts) { continue; }
      if ($ts < $start_ts) { continue; }
      if ($end_ts && ($end_ts < $ts)) { continue; }

      $worker_node = $cardcheck_instance->worker($cardcheck_node);
      $row['worker_employer_name'] = sirius_node_title($worker_node->field_grievance_shop['und'][0]['target_id']);

      $row = array();
      $row['worker_nid'] = $worker_node->nid;
      $row['worker_name'] = $worker_node->title;
      $row['worker_id'] = $worker_node->field_sirius_id['und'][0]['value'];
      $row['worker_title'] = $worker_node->field_sirius_title['und'][0]['value'];

      $bu_node = node_load($cardcheck['bu']);

      if ($bu_node) {
        $row['cardcheck_bu_name'] = $bu_node->title;
        $row['cardcheck_bu_id'] = $bu_node->field_sirius_id['und'][0]['value'];
      }

      $row['cardcheck_acceptance_date'] = date('Y-m-d H:i', $ts);

      $pk = $cardcheck_nid;
      $rows[$pk] = $row;
    }

     return array('is_multiple' => TRUE, 'rows' => $rows);
  }

}