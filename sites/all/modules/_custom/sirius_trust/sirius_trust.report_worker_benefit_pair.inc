<?php

class Sirius_Trust_Report_Worker_Benefit_Pair extends Report_Cache_Report_Generator {
  public $type = 'sirius-trust-worker-benefit-pair';

  public function info($result = array()) {
    $result = array(
      'name' => t('Coverage - Pair'),
      'description' => '',
      'access' => 'sirius trust staff',
      'preview' => array(
        'preview_links' => TRUE,
      ),
      'dashboard' => TRUE,
      'module' => 'sirius_smf',
      'file' => 'sirius_trust.report_worker_benefit_pair.inc',
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
      '#title' => t('Find workers who:'),
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
      '#title' => t('And who also:'),
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
      $links['node/' . $row['worker_nid'] . "/sirius_trust_worker_benefits"] = 'Worker';
    }

    return $links;
  }

  public function pks(&$form_state) {
    $sql = "select distinct nid from node ";
    $sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
    $sql .= "left join field_data_field_sirius_active on field_data_field_sirius_active.entity_type = 'node' and field_data_field_sirius_active.entity_id = node.nid ";
    $sql .= "left join field_data_field_sirius_contact_reltype on field_data_field_sirius_contact_reltype.entity_type = 'node' and field_data_field_sirius_contact_reltype.entity_id = node.nid ";
    $sql .= "where node.type = 'sirius_worker' ";
    $sql .= "and field_sirius_domain_target_id = :domain_nid ";

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
      $search_params = array();
      $search_params['worker_nid'] = $worker_nid;
      $search_params['nids_only'] = TRUE;
      $search_params['ts'] = $asof_ts;

      $search_params['benefit_nids'] = $benefit_1;
      $probe = sirius_trust_worker_benefits_search($search_params);
      if ($probe && ($include_1 == 'exclude')) { continue; }
      if (!$probe && ($include_1 == 'include')) { continue; }

      $search_params['benefit_nids'] = $benefit_2;
      $probe = sirius_trust_worker_benefits_search($search_params);
      if ($probe && ($include_2 == 'exclude')) { continue; }
      if (!$probe && ($include_2 == 'include')) { continue; }

      $row['worker_nid'] = $worker_nid;
      $row['worker_name'] = sirius_node_title($worker_nid);

      $rows[$worker_nid] = $row;
    }

    return array('is_multiple' => TRUE, 'rows' => $rows);
  }
}
