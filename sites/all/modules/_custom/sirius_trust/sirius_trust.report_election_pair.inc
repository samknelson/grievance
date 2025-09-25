<?php

class Sirius_Trust_Report_Election_Pair extends Report_Cache_Report_Generator {
  public $type = 'sirius-trust-election-pair';

  public function info($result = array()) {
    $result = array(
      'name' => t('Election - Pair'),
      'description' => '',
      'access' => 'sirius trust staff',
      'preview' => array(
        'preview_links' => TRUE,
      ),
      'dashboard' => TRUE,
      'module' => 'sirius_smf',
      'file' => 'sirius_trust.report_election_pair.inc',
      'row_encoding' => 'json',
    );

    return parent::info($result);
  }

  public function form() {

    $form['asof'] = array(
      '#title' => t('As Of'),
      '#type' => 'date_popup',
      '#attributes' => array(
        'autocomplete' => 'off',
      ),
      '#date_format' => 'Y-m-d',
      '#default_value' => date('Y-m-d'),
    );

    $form['include_1'] = array(
      '#title' => t('Find elections which:'),
      '#type' => 'select',
      '#options' => array(
        'include' => 'have one of',
        'exclude' => 'do not have any of',
      ),
    );

    $form['benefit_1'] = array(
      // '#title' => t('one of these benefits'),
      '#type' => 'checkboxes',
      '#options' => sirius_trust_benefit_options(FALSE),
    );

    $form['include_2'] = array(
      '#title' => t('And which also:'),
      '#type' => 'select',
      '#options' => array(
        'include' => 'have one of',
        'exclude' => 'do not have any of',
      ),
    );

    $form['benefit_2'] = array(
      // '#title' => t('one of these benefits'),
      '#type' => 'checkboxes',
      '#options' => sirius_trust_benefit_options(FALSE),
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

  public function fields() {
    $fields = array();
    $fields['worker_nid'] = 'Worker NID';
    $fields['worker_name'] = 'Worker Name';

    return $fields;
  }

  public function preview_links($row) {
    $links = array();

    if ($row['worker_nid']) {
      $links['node/' . $row['worker_nid'] . "/sirius_trust_worker_election"] = 'Worker';
    }

    return $links;
  }

  public function pks(&$form_state) {
    $sql = "select distinct nid from node ";
    $sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
    $sql .= "where node.type = 'sirius_worker' ";
    $sql .= "and field_sirius_domain_target_id = :domain_nid ";

    // $_REQUEST['worker_nid'] = 2542724;
    if ($_REQUEST['worker_nid']) {
      $sql .= "and nid = :input_worker_nid ";
      $sql_args[':input_worker_nid'] = $_REQUEST['worker_nid'];
    }

    $sql_args[':domain_nid'] = sirius_domain_nid();
    $sql .= "order by nid desc";

    $stmt = sirius_sql_query($sql, $sql_args);
    $pks = array();
    while ($hr = $stmt->fetchAssoc()) {
      $pks[] = $hr['nid'];
    }
    return array_chunk($pks, 100);
  }

  public function batch($worker_nids, $values, &$context) {
    $parameters = $values['parameters'];

    $benefit_1 = array_filter(array_values($parameters['benefit_1']));
    $benefit_2 = array_filter(array_values($parameters['benefit_2']));
    $include_1 = $parameters['include_1'];
    $include_2 = $parameters['include_2'];
    $asof = $parameters['asof'];
    if ($asof) { $asof_ts = strtotime($asof); } else { $asof_ts = time(); }
    $asof_ts = strtotime($asof);

    foreach ($worker_nids as $worker_nid) {
      $worker_node = node_load($worker_nid);

      $election_node = sirius_trust_worker_election($worker_node, $asof_ts);
      if (!$election_node) { continue; }

      $benefit_nids = sirius_fieldvals($election_node, 'field_sirius_trust_benefits', 'target_id');

      $has_1 = array_intersect($benefit_nids, $benefit_1);
      $has_2 = array_intersect($benefit_nids, $benefit_2);

      if ( ($include_1 == 'exclude') && $has_1) { continue; }
      if ( ($include_1 == 'include') && !$has_1) { continue; }

      if ( ($include_2 == 'exclude') && $has_2) { continue; }
      if ( ($include_2 == 'include') && !$has_2) { continue; }

      $row['worker_nid'] = $worker_nid;
      $row['worker_name'] = $worker_node->title;

      $rows[$worker_nid] = $row;
    }

    return array('is_multiple' => TRUE, 'rows' => $rows);
  }
}
